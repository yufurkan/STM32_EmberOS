/*
 * SystemManager.cpp
 *
 *  Created on: Feb 12, 2026
 *      Author: yufur
 */

// To avoid confusing the main.c file, I won't create it from the interface. Instead, the definition will be made here. If an interrupt occurs while the control data is being written and the PID controller reads half correct and half incorrect data, a dangerous maneuver may begin in the air.


#include "SystemManager.h"
#include "cmsis_os.h"

// Gerekli yerler deftere not alındı kontrol et


static RCState rcCommands;
static osMutexId_t key_1;

static const osMutexAttr_t sysStateMutex_attributes = {
  .name = "sysStateMutex"
};


// --- Start Func INIT ---
void System_Init(void)
{
	key_1 =osMutexNew(NULL);

	if(key_1==NULL)return;
}

// Safety Write
void System_SetRCCommands(RCState newCommands)
{

	if(osMutexAcquire(key_1,10== osOK)){
		//I want to define the struct structure globally and replace the parameters using the struct.
		rcCommands=newCommands;

		osMutexRelease(key_1);
	}

}

RCState System_GetState(void)
{
	RCState copyValue = {0};

	if(osMutexAcquire(key_1,10)){

		copyValue = rcCommands;
		osMutexRelease(key_1);
	}

	return copyValue;
}
