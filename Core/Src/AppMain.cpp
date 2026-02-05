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

extern I2C_HandleTypeDef hi2c1;// for line of ImuSensor mpu_6050(&hi2c1);
extern UART_HandleTypeDef huart2;

StatusLed led1(LD2_GPIO_Port, LD2_Pin);
ImuSensor mpu_6050(&hi2c1);

void App_Main_Start(void)
{

}


void App_Sensor_Task(void)
{
static bool scan=false;
static bool isInitialized = false;

    if (!isInitialized) {
        if(mpu_6050.init()) {
            printf("MPU6050 OK! Veri akisi basliyor...\r\n");
            isInitialized = true;
        } else {
            printf("MPU6050 BULUNAMADI! Kablolari kontrol et.\r\n");
            osDelay(1000); // Hata varsa 1 saniye bekle tekrar dene
            return;        // Fonksiyondan çık
        }
    }

if(!scan){
	scan=true;
	I2C_Scanner_Baslat(&hi2c1, &huart2, (char*)"I2C1 HATTI");
}

    led1.toggle();
    osDelay(150);
    mpu_6050.readAccel();
    mpu_6050.readGyro();


    MPU_DATA data = mpu_6050.getData();


    printf("X: %.2f | Y: %.2f | Z: %.2f | Total: %.2f\r\n",
           data.AccX, data.AccY, data.AccZ, data.totalforce);

    printf("Gyro: X:%.1f Y:%.1f Z:%.1f\r\n",
           data.gyX, data.gyY, data.gyZ);
}
