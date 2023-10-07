/*
 * task_prio.h
 *
 *  Created on: 12.06.2023
 *      Author: manni4
 */

#ifndef TASK_PRIO_H_
#define TASK_PRIO_H_


#define BLINKER_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define CONSOLE_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define CYW43_INIT_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define NETWORK_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define HEARTBEAT_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define DEMUX_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define DRIVE_TASK_PRIO (tskIDLE_PRIORITY + 1)
#define LIGHTS_TASK_PRIO (tskIDLE_PRIORITY + 1)




#endif /* TASK_PRIO_H_ */
