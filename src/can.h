/*
 * can.h
 *
 *  Created on: 04.05.2023
 *      Author: manni4
 */

#ifndef CAN_H_
#define CAN_H_

#define MASK_IDLEN 0X80000000
#define VAL_IDLEN_29 0X80000000
#define VAL_IDLEN_11 0X00000000

typedef struct
{
	unsigned int id;
	unsigned int len;
	unsigned char data[8];
} can_msg_t;

typedef struct
{
	unsigned int mask;
	unsigned int value;
} can_filter_t;


///////////////////// Message groups
#define CAN_MASK_HEARTBEAT 0X1FFFFF00
#define CAN__VAL_HEARTBEAT 0X01041300
#define CAN___ID_HEARTBEAT 0X81041301

#define HEARTBEAT_VALID_TIMEOUT_US (long)(2500*1000)

// drive
#define CAN_MASK_DRIVE 0X1FFFFF00
#define CAN__VAL_DRIVE 0X02050100
#define CAN___ID_DRIVE 0X82050101

// lights
#define CAN_MASK_LIGHTS 0X1FFFFF00
#define CAN__VAL_LIGHTS 0X02060100
#define CAN___ID_LIGHTS 0X82060101


#endif /* CAN_H_ */
