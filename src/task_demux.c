/*
 * task_demux.c
 *
 *  Created on: 20.06.2023
 *      Author: manni4
 */

#include "global_signal.h"
#include "task_demux.h"
#include "task_prio.h"
#include "can.h"

#include <stdio.h>

static TaskHandle_t demux_taskhandle;

static bool check_can_filter(can_filter_t *filter, can_msg_t *msg)
{
	bool result;
	result = (msg->id & filter->mask) == filter->value;
	return result;
}
void demux_thread(MainEnvironement_t *MainEnvironement)
{
	can_msg_t msg;
	BaseType_t result;

	can_filter_t can_filter_heartbeat =
	{ CAN_MASK_HEARTBEAT, CAN__VAL_HEARTBEAT };
	can_filter_t can_filter_drive =
	{ CAN_MASK_DRIVE, CAN__VAL_DRIVE };
	can_filter_t can_filter_lights =
	{ CAN_MASK_LIGHTS, CAN__VAL_LIGHTS };

	while (true)
	{
		result = xQueueReceive(MainEnvironement->from_host, &msg, XDELAY);
		if (result == pdPASS)
		{
			if (check_can_filter(&can_filter_drive, &msg))
			{
				xQueueSend(MainEnvironement->to_drive, &msg, XDELAY);
			}
			else if (check_can_filter(&can_filter_heartbeat, &msg))
			{
				xQueueSend(MainEnvironement->to_heartbeat, &msg, XDELAY);
			}
			else if (check_can_filter(&can_filter_lights, &msg))
			{
				xQueueSend(MainEnvironement->to_lights, &msg, XDELAY);
			}
			else
			{
				puts("Unknown Message!");
			}
		}
		else
		{
			// nothing to do ... then do nothing
		}
	}
}

void demux_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) demux_thread, "DEMUX", configMINIMAL_STACK_SIZE,
			MainEnvironement, DEMUX_TASK_PRIO, &demux_taskhandle);
}
