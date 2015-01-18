//*****************************************************************************
//
//  File........: RTC.h
//
//  Author(s)...: ATMEL Norway
//
//  Target(s)...: ATmega169
//
//  Description.: Functions for RTC.c
//
//  Revisions...: 1.0
//
//  YYYYMMDD - VER. - COMMENT                                       - SIGN.
//
//  20021015 - 1.0  - File created                                  - LHM
//  20031009          port to avr-gcc/avr-libc                      - M.Thomas
//
//*****************************************************************************


extern volatile uint8_t  gSECOND;
extern volatile uint8_t  gMINUTE;
extern volatile uint8_t  gHOUR;
extern volatile uint8_t  gDAY;
extern volatile uint8_t  gMONTH;
extern volatile uint16_t gYEAR;

extern volatile uint8_t  gPowerSaveTimer;
//mtE

// decrement counter of period/start/run operations
extern volatile uint32_t gCOUNTER; // Seconds
extern volatile uint8_t gInGo; // true if we are in the go state

//  Function declarations
void RTC_init(void);            //initialize the Timer Counter 2 in asynchron operation

void setCounterMinCheckpoint(uint16_t counter);
uint16_t getCounterMinCheckpoint();

#define CLOCK_24    1
#define CLOCK_12    0
