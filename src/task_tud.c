/*
 * task_demux.c
 *
 *  Created on: 20.06.2023
 *      Author: manni4
 */

#include "global_signal.h"
#include "task_tud.h"
#include "task_prio.h"

#include <stdio.h>

#include "tusb.h"

static TaskHandle_t tud_taskhandle;

void tud_thread(MainEnvironement_t *MainEnvironement)
{
	tusb_init();

	while (true)
	{
		tud_task();
	}
}

void task_tud_init(MainEnvironement_t *MainEnvironement)
{
	xTaskCreate((CALLEE) tud_thread, "TUD", configMINIMAL_STACK_SIZE,
			MainEnvironement, TUD_TASK_PRIO, &tud_taskhandle);
}
