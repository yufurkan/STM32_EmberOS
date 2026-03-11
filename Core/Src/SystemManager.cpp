/*
 * SystemManager.cpp
 *
 *  Created on: Feb 12, 2026
 *      Author: yufur
 */

// To avoid confusing the main.c file, I won't create it from the interface. Instead, the definition will be made here. If an interrupt occurs while the control data is being written and the PID controller reads half correct and half incorrect data, a dangerous maneuver may begin in the air.


#include "SystemManager.h"
#include "cmsis_os.h"

// Gerekli yerler deftere not alındı kontrol et taskENTER_CRITICAL()


static RCState rcCommands;
static osMutexId_t key_1;

static const osMutexAttr_t sysStateMutex_attributes = {
  .name = "sysStateMutex"
};

// ATTENTİON: I remove the mutex using and start useing Critical Sections.

// --- Start Func INIT ---
void System_Init(void)
{
	//key_1 =osMutexNew(NULL);

	//if(key_1==NULL)return;
}

// Safety Write
void System_SetRCCommands(RCState newCommands)
{


	taskENTER_CRITICAL();// stop  interrupts
		//I want to define the struct structure globally and replace the parameters using the struct.
	rcCommands=newCommands;

	taskEXIT_CRITICAL();// open interrupts


}

RCState System_GetState(void)
{
	RCState copyValue;

	    taskENTER_CRITICAL();

	    copyValue = rcCommands;

	    taskEXIT_CRITICAL();

	    return copyValue;
}
