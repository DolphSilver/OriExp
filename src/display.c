/*
 * display.c
 *
 *  Created on: 10 dec. 2017
 *      Author: perpettersson
 */


#include "display.h"
#include "signals.h"

uint8_t line_1[LINE_LENGTH];
uint8_t line_2[LINE_LENGTH];



void display_init(void){
	;
}

void display_process(void){
while(1){
	if(access_allowed_Get() == 1){
	// Read signals, disable interupts
		accessing_Set();

		accessing_Reset();
	//perform task
	}
	else{
		; //Permission denied by kernel. If kernel interrups during reading (accessing set)
		// the the kernel will set access_allowed false and schedule more time for process
		// this way the process get one and ONLY one more chance to finish. The kernel will
		// set access_allowed to true when the next process get to run.
	}

}
}
