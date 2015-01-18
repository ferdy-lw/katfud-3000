//*****************************************************************************
//
//  File........: RTC.c
//
//  Author(s)...: ATMEL Norway
//
//  Target(s)...: ATmega169
//
//  Compiler....: AVR-GCC 4.1.1; avr-libc 1.4.5
//
//  Description.: Real Time Clock (RTC)
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20021015 - 1.0  - Created                                       - LHM
//  20031009          port to avr-gcc/avr-libc                      - M.Thomas
//  20051107          minior correction (volatiles)                 - mt
//  20070129          SIGNAL->ISR                                   - mt
//*****************************************************************************

//mtA
#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "main.h"
#include "RTC.h"
#include "Ports.h"
#include "eeprom.h"


volatile uint8_t  gSECOND;
volatile uint8_t  gMINUTE;
volatile uint8_t  gHOUR;
volatile uint8_t  gDAY;
volatile uint8_t  gMONTH;
volatile uint16_t gYEAR;

volatile uint32_t gCOUNTER; // seconds
volatile uint8_t gInGo; // true if we are in the go state


//char gPowerSaveTimer = 0;
//char dateformat = 0;
volatile uint8_t gPowerSaveTimer = 0;
uint8_t dateformat = 0;
// mtE

// Lookup table holding the length of each mont. The first element is a dummy.
// mt this could be placed in progmem too, but the arrays are accessed quite
//    often - so leaving them in RAM is better...
char MonthLength[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

char TBL_CLOCK_12[] =   // table used when displaying 12H clock
{12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

char clockformat = CLOCK_24;    // set initial clock format to 24H




/******************************************************************************
*
*   Function name:  RTC_init
*
*   returns:        none
*
*   parameters:     none
*
*   Purpose:        Start Timer/Counter2 in asynchronous operation using a
*                   32.768kHz crystal.
*
*******************************************************************************/
void RTC_init(void)
{
    Delay(1000);            // wait for 1 sec to let the Xtal stabilize after a power-on,

    cli(); // mt __disable_interrupt();  // disabel global interrupt

    cbiBF(TIMSK2, TOIE2);             // disable OCIE2A and TOIE2

    ASSR = (1<<AS2);        // select asynchronous operation of Timer2

    TCNT2 = 0;              // clear TCNT2A
    TCCR2A |= (1<<CS22) | (1<<CS20);             // select precaler: 32.768 kHz / 128 = 1 sec between each overflow

    while((ASSR & 0x01) | (ASSR & 0x04));       // wait for TCN2UB and TCR2UB to be cleared

    TIFR2 = 0xFF;           // clear interrupt-flags
    sbiBF(TIMSK2, TOIE2);     // enable Timer2 overflow interrupt

    sei(); // mt __enable_interrupt();                 // enable global interrupt

    // initial time and date setting
    gSECOND  = 0;
    gMINUTE  = 0;
    gHOUR    = 12;

//    gCOUNTER = 0;
}

#define HOUR       0
#define MINUTE     1
#define SECOND     2


/******************************************************************************
*
**
*   Function name:  setCounterCheckpoint
*
*   returns:        none
*
*   parameters:     Counter
*
*   Purpose:        Save the counter checkpoint to eeprom
*
*******************************************************************************/
void setCounterMinCheckpoint(uint16_t counter) {
  eeprom_write_word(EEPROM_COUNTER, counter);
}


/******************************************************************************
*
**
*   Function name:  getCounterCheckpoint
*
*   returns:        Counter from previous checkpoint
*
*   parameters:     none
*
*   Purpose:        Get a previously checkpointed counter
*
*******************************************************************************/
uint16_t getCounterMinCheckpoint() {
  return eeprom_read_word(EEPROM_COUNTER);
}


/******************************************************************************
*
*   Timer/Counter2 Overflow Interrupt Routine
*
*   Purpose: Increment the real-time clock
*            The interrupt occurs once a second (running from the 32kHz crystal)
*
*******************************************************************************/
// mtA
// #pragma vector = TIMER2_OVF_vect
// __interrupt void TIMER2_OVF_interrupt(void)
// SIGNAL(SIG_OVERFLOW2)
ISR(TIMER2_OVF_vect)
// mtE
{
    static char LeapMonth;

    gCOUNTER--; // Update our total second counter for use by the RUN function
    if (gInGo) {
      blink_led();
    }

    gSECOND++;               // increment second

    if (gSECOND == 60)
    {
        gSECOND = 0;
        gMINUTE++;

        gPowerSaveTimer++;

        if (gMINUTE > 59)
        {
            gMINUTE = 0;
            gHOUR++;
            // Checkpoint the counter so that during a power failure we start from this time
            setCounterMinCheckpoint(gCOUNTER/60);

            if (gHOUR > 23)
            {

                gHOUR = 0;
                gDAY++;

                // Check for leap year if month == February
                if (gMONTH == 2)
                    if (!(gYEAR & 0x0003))              // if (gYEAR%4 == 0)
                        if (gYEAR%100 == 0)
                            if (gYEAR%400 == 0)
                                LeapMonth = 1;
                            else
                                LeapMonth = 0;
                        else
                            LeapMonth = 1;
                    else
                        LeapMonth = 0;
                else
                    LeapMonth = 0;

                // Now, we can check for month length
                if (gDAY > (MonthLength[gMONTH] + LeapMonth))
                {
                    gDAY = 1;
                    gMONTH++;

                    if (gMONTH > 12)
                    {
                        gMONTH = 1;
                        gYEAR++;
                    }
                }
            }
        }
    }
}
