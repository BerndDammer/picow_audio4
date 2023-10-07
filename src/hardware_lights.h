/*
 * spi_lights.h
 *
 *  Created on: 09.03.2023
 *      Author: manni4
 */

#ifndef HARDWARE_LIGHTS_H_
#define HARDWARE_LIGHTS_H_

void hardware_lights_init(void);
void hardware_lights_set_single(int index, int r, int g, int b);
void hardware_lights_off(void);

#endif /* HARDWARE_LIGHTS_H_ */
