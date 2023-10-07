#include <stdio.h>
#include <stdbool.h>

#include "task_drive.h"
#include "task_prio.h"
#include "can.h"
#include "hardware_lights.h"
#include "global_signal.h"

static TaskHandle_t lights_taskhandle;

#define POWER_OFF_DELAY_TICK pdMS_TO_TICKS(1000)

void check_connected(bool has_control, bool *last,
		MainEnvironement_t *MainEnvironement) {
	if (has_control) {
		if (!*last) {
			hardware_lights_off();
			*last = true;
		}
	} else {
		hardware_lights_off();
		EventBits_t mask = xEventGroupGetBits(MainEnvironement->mainEventGroup);
		if ((mask & 0X800007) != 7) {
			bool set;
			*last = false;
			set = (mask & 0x800000) != 0;
			hardware_lights_set_single(7, set ? 1 : 0, set ? 0 : 1, 0);

			set = (mask & 0x4) != 0;
			hardware_lights_set_single(2, set ? 0 : 1, set ? 1 : 0, 0);
			set = (mask & 0x2) != 0;
			hardware_lights_set_single(1, set ? 0 : 1, set ? 1 : 0, 0);
			set = (mask & 0x1) != 0;
			hardware_lights_set_single(0, set ? 0 : 1, set ? 1 : 0, 0);

		} else {
			*last = true;
		}
	}
}
void lights_thread(MainEnvironement_t *MainEnvironement) {
	bool connected = false;

	hardware_lights_init();

	while (true) {
		can_msg_t msg;
		BaseType_t result;

		result = xQueueReceive(MainEnvironement->to_lights, &msg,
		POWER_OFF_DELAY_TICK);
		if (result == pdPASS) {
			check_connected(true, &connected, MainEnvironement);
			switch (msg.id) {
			case CAN___ID_LIGHTS:
				if (msg.len == 4 || msg.len == 8) {
					hardware_lights_set_single(msg.data[0], msg.data[1],
							msg.data[2], msg.data[3]);

				} else {
					puts("Wrong light len!");
				}
				break;
			default:
				puts("You should not be here");
				break;
			}
		} else if (result == errQUEUE_EMPTY) {
			check_connected(false, &connected, MainEnvironement);
		}
	}
}

void lights_init(MainEnvironement_t *MainEnvironement) {
	xTaskCreate((CALLEE) lights_thread, "LIGHTS", configMINIMAL_STACK_SIZE,
			MainEnvironement, LIGHTS_TASK_PRIO, &lights_taskhandle);
}
