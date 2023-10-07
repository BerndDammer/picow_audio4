
#define HEARTBEAT_DELAY_TICK pdMS_TO_TICKS(800)
#define HEARTBEAT_VALID_TIMEOUT_TICK pdMS_TO_TICKS(3333)

#include "panic.h"
#include "global_signal.h"
#include "task_heartbeat.h"
#include "task_prio.h"
#include "can.h"


#include "pico/stdlib.h"
#include "pico/types.h"
#include <stdio.h>

static TaskHandle_t heartbeat_taskhandle;

void heartbeat_thread(MainEnvironement_t *MainEnvironement)
{
	TickType_t last_incomming_hertbeat_time = 0;
	TickType_t nextTransmitt = xTaskGetTickCount() + HEARTBEAT_DELAY_TICK;

	short last_incomming_counter = -1;
	short my_heartbeat_counter = 1;

	while(true)
	{
		can_msg_t msg;
		BaseType_t result;
		TickType_t delay = nextTransmitt - xTaskGetTickCount();
		if( delay < 1) delay = 1;
		result = xQueueReceive( MainEnvironement->to_heartbeat, &msg, delay);
		if( result == pdPASS)
		{
			last_incomming_counter = msg.data[0] | msg.data[1] << 8;
			last_incomming_hertbeat_time = xTaskGetTickCount();
		}
		else if( result == errQUEUE_EMPTY )
		{
			msg.id = CAN___ID_HEARTBEAT;
			msg.len = 8;
			msg.data[0] = my_heartbeat_counter;
			msg.data[1] = my_heartbeat_counter >> 8;

			if(last_incomming_hertbeat_time + HEARTBEAT_VALID_TIMEOUT_TICK > xTaskGetTickCount())
			{
				xEventGroupSetBits(MainEnvironement->mainEventGroup, EVENT_MASK_HEARTBEAT);
				msg.data[2] = last_incomming_counter;
				msg.data[3] = last_incomming_counter >> 8;
			}
			else
			{
				xEventGroupClearBits(MainEnvironement->mainEventGroup, EVENT_MASK_HEARTBEAT);
				msg.data[2] = 0XFF;
				msg.data[3] = 0XFF;
			}
			// put event bits in heartbeat
			{
				EventBits_t bits =xEventGroupGetBits(MainEnvironement->mainEventGroup);

				msg.data[4] = bits;
				msg.data[5] = bits >> 8;
				msg.data[6] = bits >> 16;
				msg.data[7] = bits >> 24;
			}

			xQueueSend( MainEnvironement-> to_host, &msg, XDELAY);
			nextTransmitt += HEARTBEAT_DELAY_TICK;
			my_heartbeat_counter++;
		}
		else
		{
			puts("You should not be here");
		}
	}
}

void heartbeat_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) heartbeat_thread, "HEARTBEAT",
			configMINIMAL_STACK_SIZE, MainEnvironement, HEARTBEAT_TASK_PRIO,
			&heartbeat_taskhandle);
}

