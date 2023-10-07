/*
 * task_drive.c
 *
 *  Created on: 20.06.2023
 *      Author: manni4
 */

#include <stdio.h>

#include "task_drive.h"
#include "task_prio.h"
#include "can.h"
#include "hardware_drive.h"

static TaskHandle_t drive_taskhandle;

#define POWER_OFF_DELAY_TICK pdMS_TO_TICKS(1000)

void drive_thread(MainEnvironement_t *MainEnvironement)
{
	motor_init();

	while (true)
	{
		can_msg_t msg;
		BaseType_t result;

		result = xQueueReceive(MainEnvironement->to_drive, &msg,
				POWER_OFF_DELAY_TICK);
		if (result == pdPASS)
		{
			switch (msg.id)
			{
			case CAN___ID_DRIVE:
				motor_set(FRONT_LEFT, *(signed short*) &msg.data[0]);
				motor_set(FRONT_RIGHT, *(signed short*) &msg.data[2]);
				motor_set(REAR_LEFT, *(signed short*) &msg.data[4]);
				motor_set(REAR_RIGHT, *(signed short*) &msg.data[6]);
				break;
			default:
				puts("You should not be here");
				break;
			}
		}
		else if (result == errQUEUE_EMPTY)
		{
			// control timeout switches engines off
			motor_set(FRONT_LEFT, 0);
			motor_set(FRONT_RIGHT, 0);
			motor_set(REAR_LEFT, 0);
			motor_set(REAR_RIGHT, 0);
		}
	}
}

void drive_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) drive_thread, "DRIVE", configMINIMAL_STACK_SIZE,
			MainEnvironement, DRIVE_TASK_PRIO, &drive_taskhandle);
}
