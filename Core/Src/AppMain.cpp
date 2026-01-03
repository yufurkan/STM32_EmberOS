/*
 * AppMain.cpp
 *
 *  Created on: Jan 3, 2026
 *      Author: yufur
 */


#include "AppMain.h"
#include "main.h"      // Donanım tanımlarını getirmek için
#include "cmsis_os.h"  // RTOS fonksiyonları


void App_Main_Start(void)
{


    // Test:
	HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
}
