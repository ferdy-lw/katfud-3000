/*
 * LedBlinker.c
 *
 *  Created on: Jan 31, 2009
 *      Author: ferdy
 */


#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "main.h"
#include "Ports.h"

void OUTS_init(void)
{

  // JTAG runs on PORTF - we are going to reuse these pins for our output

  // Reset the JTAG pins, this must be done twice
  cli();
  MCUCR |= (1<<JTD);
  MCUCR |= (1<<JTD);
  MCUCR |= (1<<JTD);
  MCUCR |= (1<<JTD);

  //        8   7   6  5  4  3  2  1
  // PORTF            oM oL

  // PF4 TCK (led blink)
  // PF6 TDO (input for running motor)
  // PF5 TMS (drives motor)
  // PF7 TDI

  DDRF = 0xFF & ~(1<<6);     // Set up PORT for out
//  DDRF &= ~(1<<6);  // Change PIN 5 for in

  //PORTF = 0x00; // Tie low.  LED on if using high line
  PORTF = 0xFF; // It seems that we can only drive low properly now?  shorted the board?
                // So rewire the relay and trigger when we drive low.
  sei();

}

void blink_led(void)
{
  PORTF ^= (1<<4);
}


void motor(unsigned char on)
{
  if (on) {
//    PORTF |= (1<<6);
    PORTF &= ~(1<<5);

//    PORTF |= (1<<5);

  } else {
//    PORTF &= ~(1<<6);
    PORTF |= (1<<5);

//    PORTF &= ~(1<<5);
  }
}


BOOL isInputLow(void) {
  return (~PINF) & (1<<PINF6);
}
