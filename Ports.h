/*
 * LedBlinker.h
 *
 *  Created on: Jan 31, 2009
 *      Author: ferdy
 */

#ifndef LEDBLINKER_H_
#define LEDBLINKER_H_

//  Function declarations
void OUTS_init(void);            //the output ports

void blink_led(void);  // blink led

void motor(unsigned char on);

BOOL isInputLow(void);

#endif /* LEDBLINKER_H_ */
