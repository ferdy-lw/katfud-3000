/*
 * runtimes.h
 *
 *  Created on: Jan 1, 2009
 *      Author: ferdy
 */

#ifndef RUNTIMES_H_
#define RUNTIMES_H_

#define __need_size_t
#include <stddef.h>
#include <inttypes.h>

extern volatile uint8_t gStart; // hours
extern volatile uint8_t gPeriod; // hours
extern volatile uint8_t gRun; // seconds

void RunTimes_Init(void);
char StartShow(char input);
char StartSetFunc(char input);
char PeriodShow(char input);
char PeriodSetFunc(char input);
char RunShow(char input);
char RunSetFunc(char input);
char GoNowFunc(char input);
char GoNowWaitFunc(char input);
char GoNowMotorFunc(char input);

#endif /* RUNTIMES_H_ */
