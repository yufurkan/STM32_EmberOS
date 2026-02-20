/*
 * SystemManager.h
 *
 *  Created on: Feb 12, 2026
 *      Author: yufur
 */

#ifndef INC_SYSTEMMANAGER_H_
#define INC_SYSTEMMANAGER_H_
#include <stdint.h>
#include <stdbool.h>



struct RCState{
	float pitch;
	float roll;
	float yaw;
	float throttle;
	bool armed;
	// ı can expannd later
};

void System_Init(void); // Inıt
void System_SetRCCommands( RCState newCommands);
  RCState System_GetState(void);



#endif /* INC_SYSTEMMANAGER_H_ */
