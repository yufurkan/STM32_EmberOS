/*
 * AppMain.cpp
 *
 *  Created on: Jan 3, 2026
 *      Author: yufur
 */

#include <stdio.h>
#include "AppMain.h"
#include "main.h"
#include "cmsis_os.h"
#include "StatusLed.h"
#include "ImuSensor.h"
#include "i2c_scanner.h"
#include "Pid.h"
#include "SystemManager.h"
#include "Mixer.h"
#include "IbusReader.h"

extern I2C_HandleTypeDef hi2c1;// for line of ImuSensor mpu_6050(&hi2c1);
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart6;//using for fs ia6b reciver line

StatusLed led1(LD2_GPIO_Port, LD2_Pin);
ImuSensor mpu_6050(&hi2c1);

float servo_minout= -20.0f;
float servo_maxout=  20.0f;

//FİXED _______(I will return here DANGER)-----
float motor_minout= 0.0f;
float motor_maxout= 100.0f;
//FİXED _______------------------------------

Pid pid_roll(1.5f, 0.01f, 0.5f ,servo_minout, servo_maxout);
Pid pid_pitch(1.8f, 0.02f, 0.6f,servo_minout, servo_maxout);
ActuatorMixer mixer(servo_minout, servo_maxout,motor_minout,motor_maxout);

RCState currentState;
PIDOuts pidCommands;
IbusReader ibus;



void App_Main_Start(void)
{
	System_Init();

	//I will transfer into fuct later----
	//dummy commands erased. uart6 dma way is open
	//------------------------------------

	if(mpu_6050.init()) {
		printf("MPU6050 OK! Veri akisi basliyor...\r\n");
	} else {
		printf("MPU6050 BULUNAMADI! Kablolari kontrol et.\r\n");
		HAL_Delay(500);

	}

	// uart6 DMA start command----------

	HAL_UART_Receive_DMA(&huart6, ibus.rx_buffer, 32);

	//----------------------------------
}



void App_Led_Task(void)
{
	if(currentState.armed)led1.on();
	else led1.toggle();
}

void App_Sensor_Task(void)
{

	static bool scan=false;

	if(!scan){
		scan=true;
		I2C_Scanner_Baslat(&hi2c1, &huart2, (char*)"I2C1 HATTI");
	}


    mpu_6050.readAccel();
    mpu_6050.readGyro();
    mpu_6050.angleMeasurement();
    MPU_DATA data = mpu_6050.getData();

    float dt=0.004f;//osDelay(4)->0.004f

    currentState=System_GetState();


    pidCommands.servo_roll_out = pid_roll.compute(currentState.roll, data.roll, dt);
    pidCommands.servo_pitch_out= pid_pitch.compute(currentState.pitch, data.pitch, dt);

    mixer.compute(currentState, pidCommands);
    ActuatorState_t pwm_outputs = mixer.getState();

    //this messages keeps processor bussy a lot . These are cancelled.
    //printf("X: %.2f | Y: %.2f | Z: %.2f | Total: %.2f\r\n",data.AccX, data.AccY, data.AccZ, data.totalforce);
    //printf("Gyro: X:%.1f Y:%.1f Z:%.1f\r\n",data.gyX, data.gyY, data.gyZ);

    //For now Im using a plain printf with a standard UART baud rate of 115200. It keeps the processor busy for about 2.5ms per message. With an osdelay of 4ms, there's still enough time for processing. Im skipping DMA usage for now I'll come back to work on it.
    printf("AIL1:%u | AIL2:%u | ELEV:%u | MOT:%u\r\n",
    		pwm_outputs.aileron1_pwm,
			pwm_outputs.aileron2_pwm,
			pwm_outputs.elevator_pwm,
			pwm_outputs.motor_pwm);


}



// huart6 DMA INterrupt !!
extern "C" void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {

	//checking is it uart6
    if (huart->Instance == USART6) {

        ibus.parse();
        RCState pilotCommands = ibus.getRCState();

        // Write to the rcCommands with Mutex/Critical Section protected function
        System_SetRCCommands(pilotCommands);
    }
}

