/*
 * runtimes.c
 *
 * Setting and displaying of all the run time details.
 *
 *  Created on: Jan 1, 2009
 *      Author: ferdy
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>

#include "button.h"
#include "main.h"
#include "runtimes.h"
#include "LCD_functions.h"
#include "BCD.h"
#include "usart.h"
#include "RTC.h"
#include "Ports.h"
#include "eeprom.h"

// The actual counts
volatile uint8_t gStart; // hours
volatile uint8_t gPeriod; // hours
volatile uint8_t gRun; // seconds

//extern volatile uint32_t gCOUNTER; // Seconds
//extern volatile uint8_t gInGo; // true if we are in the go state
//extern volatile uint8_t gSECOND; // Seconds

// These vars contain the currently editing BCD being displayed
uint8_t editTens, editOnes, editingTens, editCurChar;

// Figure out when to turn off the motor
BOOL waitForFinish;

// Privates
//==========
char editTime(char *pEnter, volatile uint8_t *pTimeToEdit, uint8_t *eepromAddr, char input, const uint8_t doneState, const uint8_t thisState);


/*****************************************************************************
*
*   Function name : InitTimes
*
*   Returns :       void
*
*   Parameters :
*
*   Purpose :       Load the times from EEPROM, or use reasonable defaults
*
*****************************************************************************/
void RunTimes_Init(void)
{

  gStart = eeprom_read_byte(EEPROM_START);
  gPeriod = eeprom_read_byte(EEPROM_PERIOD);
  gRun = eeprom_read_byte(EEPROM_RUN);

}



/*****************************************************************************
*
*   Function name : StartShow
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Shows the start hour countdown on the LCD
*
*****************************************************************************/
char StartShow(char input)
{

  uint8_t TH,TL;

  TH = CHAR2BCD2(gStart);

  TL = (TH & 0x0F) + '0';
  TH = (TH >> 4) + '0';

  LCD_putc(0, 'S');
  LCD_putc(1, 'T');
  LCD_putc(2, ' ');
  LCD_putc(3, TH + 0x80);
  LCD_putc(4, TL + 0x80);
  LCD_putc(5, 'h');
  LCD_putc(6, '\0');

  LCD_UpdateRequired(TRUE, 0);

  if (input == KEY_PLUS) {
    return ST_AVRBF;
  } else if (input == KEY_NEXT) {
    return ST_START_SET;
  } else if (input == KEY_PREV) {
    return ST_AVRBF;
  } else if (input == KEY_MINUS) {
    return ST_PERIOD;
  }

  return ST_START;
}


/*****************************************************************************
*
*   Function name : StartSetFunc
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Set the start hour countdown on the LCD
*
*****************************************************************************/
char StartSetFunc(char input)
{

  static char enter = 1;

  return editTime(&enter, &gStart, EEPROM_START, input, ST_START, ST_START_SET_FUNC);

//  if (enter) {
//
//    Usart_Tx(2);
//
//    // Break up the var being edited
//    editTens = CHAR2BCD2(gStart);
//
//    editOnes = (editTens & 0x0F) + '0';
//    editTens = (editTens >> 4) + '0';
//    editingTens = TRUE;
//
//    enter = 0;
//
//  } else  {
//
//    LCD_putc(0, 'E');
//    LCD_putc(1, 'N');
//    LCD_putc(2, ' ');
//    if (editingTens) {
//      LCD_putc(3, editTens|0x80 );
//      LCD_putc(4, editOnes);
//    } else {
//      LCD_putc(3, editTens);
//      LCD_putc(4, editOnes|0x80);
//    }
//    LCD_putc(5, 'h');
//    LCD_putc(6, '\0');
//
//    if (input != KEY_NULL) {
//      LCD_FlashReset();
//    }
//
//    LCD_UpdateRequired(TRUE, 0);
//  }
//
//
//  if (input != KEY_NULL) {
//    LCD_FlashReset();
//  }
//
//  if ( input == KEY_MINUS || input == KEY_PLUS) {
//
//    if (input == KEY_MINUS) {
//      Usart_Tx(0x10);
//      editCurChar--;
//    } else {
//      Usart_Tx(0x11);
//      editCurChar++;
//    }
//
//    if (editCurChar < '0') {
//      editCurChar = '9';
//    } else if (editCurChar > '9') {
//      editCurChar = '0';
//    }
//
//    if (editingTens) {
//      editTens = editCurChar;
//    } else {
//      editOnes = editCurChar;
//    }
//
//  } else if (input == KEY_PREV || input == KEY_NEXT) {
//
//    if (editingTens) {
//      Usart_Tx(0x20);
//      editingTens = FALSE;
//      editCurChar = editOnes;
//    } else {
//      Usart_Tx(0x21);
//      editingTens = TRUE;
//      editCurChar = editTens;
//    }
//
//  } else if (input == KEY_ENTER) {
//
//    Usart_Tx(0x50);
//
//    gStart = BCD22CHAR(((editTens-'0') << 4)+(editOnes-'0'));
//
//    eeprom_write_byte(EEPROM_START, gStart);
//
//    enter = 1;
//    return ST_START;
//  }
//
////  Usart_Tx('X');
//
//  return ST_START_SET_FUNC;
}

/*****************************************************************************
*
*   Function name : PeriodShow
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Shows the period hour countdown on the LCD
*
*****************************************************************************/
char PeriodShow(char input)
{

  uint8_t TH,TL;

  TH = CHAR2BCD2(gPeriod);

  TL = (TH & 0x0F) + '0';
  TH = (TH >> 4) + '0';

  LCD_putc(0, 'P');
  LCD_putc(1, 'D');
  LCD_putc(2, ' ');
  LCD_putc(3, TH + 0x80);
  LCD_putc(4, TL + 0x80);
  LCD_putc(5, 'h');
  LCD_putc(6, '\0');

  LCD_UpdateRequired(TRUE, 0);

  if (input == KEY_PLUS) {
    return ST_START;
  } else if (input == KEY_NEXT) {
    return ST_PERIOD_SET;
  } else if (input == KEY_PREV) {
    return ST_AVRBF;
  } else if (input == KEY_MINUS) {
    return ST_RUN;
  }

  return ST_PERIOD;
}


/*****************************************************************************
*
*   Function name : PeriodSetFunc
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Set the period hour countdown on the LCD
*
*****************************************************************************/
char PeriodSetFunc(char input)
{
  static char enter = 1;

  return editTime(&enter, &gPeriod, EEPROM_PERIOD, input, ST_PERIOD, ST_PERIOD_SET_FUNC);
}


/*****************************************************************************
*
*   Function name : RunShow
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Shows the run seconds countdown on the LCD
*
*****************************************************************************/
char RunShow(char input)
{

  uint8_t TH,TL;

  TH = CHAR2BCD2(gRun);

  TL = (TH & 0x0F) + '0';
  TH = (TH >> 4) + '0';

  LCD_putc(0, 'R');
  LCD_putc(1, 'N');
  LCD_putc(2, ' ');
  LCD_putc(3, TH+0x80);
  LCD_putc(4, TL+0x80);
  LCD_putc(5, 's');
  LCD_putc(6, '\0');

  LCD_UpdateRequired(TRUE, 0);

  if (input == KEY_PLUS) {
      return ST_PERIOD;
  } else if (input == KEY_NEXT) {
      return ST_RUN_SET;
  } else if (input == KEY_PREV) {
      return ST_AVRBF;
  } else if (input == KEY_MINUS) {
      return ST_GONOW;
  }

    return ST_RUN;
}


/*****************************************************************************
*
*   Function name : RunSetFunc
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Set the run second countdown on the LCD
*
*****************************************************************************/
char RunSetFunc(char input)
{
  static char enter = 1;

  return editTime(&enter, &gRun, EEPROM_RUN, input, ST_RUN, ST_RUN_SET_FUNC);
}



/*****************************************************************************
*
*   Function name : RunSetFunc
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    char input (from joystick)
*
*   Purpose :       Set the run second countdown on the LCD
*
*****************************************************************************/
char GoNowFunc(char input)
{

    LCD_putc(0, 'G');
    LCD_putc(1, 'O');
    LCD_putc(2, ' ');
    LCD_putc(3, 'x'+0x80);
    LCD_putc(4, 'x'+0x80);
    LCD_putc(5, 'x'+0x80);
    LCD_putc(6, '\0');

    LCD_UpdateRequired(TRUE, 0);

    if (input == KEY_PREV) {
        gInGo = FALSE;
        return ST_GONOW;
    }

    if (input == KEY_PLUS) {

      //  Start the count down to the Start time, and checkpoint it
      gCOUNTER = ((uint32_t)gStart)*3600;
//      gCOUNTER /= 3600;  // DEBUG back to seconds
      setCounterMinCheckpoint(gCOUNTER/60);
      gInGo = TRUE;

      return ST_GONOWWAIT_FUNC;
    }

    return ST_GONOW_FUNC;
}



char GoNowWaitFunc(char input)
{

  if (input == KEY_PREV) {
      gInGo = FALSE;
      return ST_GONOW;
  }

  uint8_t TH,TL,unit;

  if (gCOUNTER > 99) {
    if ((gCOUNTER/60) > 99) {
      TH = CHAR2BCD2(gCOUNTER/3600);  // Put back to hours
      unit = 'h';
    } else {
      TH = CHAR2BCD2(gCOUNTER/60);  // Put back to minutes
      unit = 'm';
    }
  } else {
    TH = CHAR2BCD2(gCOUNTER);
    unit = 's';
  }

  TL = (TH & 0x0F) + '0';
  TH = (TH >> 4) + '0';

  LCD_putc(0, 'I');
  LCD_putc(1, 'N');
  LCD_putc(2, ' ');
  LCD_putc(3, TH);
  LCD_putc(4, TL);
  LCD_putc(5, unit);
  LCD_putc(6, '\0');
  LCD_UpdateRequired(TRUE, 0);

  // Once we've started we need to run the motor
  if (gCOUNTER <= 0) {

      gCOUNTER = gRun;

      motor(1);

      waitForFinish = FALSE;

      return ST_GONOWMOTOR_FUNC;
  }

  return ST_GONOWWAIT_FUNC;
}



char GoNowMotorFunc(char input)
{

  if (input == KEY_PREV) {

      motor(0);

      gInGo = FALSE;
      return ST_GONOW;
  }


  uint8_t TH,TL;

  TH = CHAR2BCD2(gCOUNTER);

  TL = (TH & 0x0F) + '0';
  TH = (TH >> 4) + '0';

  LCD_putc(0, 'M');
  LCD_putc(1, 'R');
  LCD_putc(2, ' ');
  LCD_putc(3, TH);
  LCD_putc(4, TL);
  LCD_putc(5, 's');
  LCD_putc(6, '\0');
  LCD_UpdateRequired(TRUE, 0);

  // Wait for the drop to low, then when it hits high again reset the motor
  // and the time back over to the period
  if (waitForFinish) {

    if (isInputLow()) {
      // Reset the counter back to the period, and checkpoint it
      gCOUNTER = ((uint32_t)gPeriod)*3600;
//      gCOUNTER /= 3600;  // DEBUG back to seconds
      setCounterMinCheckpoint(gCOUNTER/60);
      motor(0);

      return ST_GONOWWAIT_FUNC;
    }
  } else {

    if (! isInputLow()) {
      waitForFinish = TRUE;
    }
  }

/*** This is based on a time run
  // Once we've hit zero again, reset the motor and the time back over to the period
  if (gCOUNTER <= 0)
  {
      // Reset the counter back to the period, and checkpoint it
      gCOUNTER = ((uint32_t)gPeriod)*3600;
      setCounterMinCheckpoint(gCOUNTER/60);
      motor(0);

      return ST_GONOWWAIT_FUNC;
  }
***/

  return ST_GONOWMOTOR_FUNC;
}





/***************
  PRIVATE
****************/

/*****************************************************************************
*
*   Function name : editTime
*
*   Returns :       char ST_state (to the state-machine)
*
*   Parameters :    enter == 1 will read the time from
*
*   Purpose :       Set the run second countdown on the LCD
*
*****************************************************************************/
char editTime(char *pEnter, volatile uint8_t *pTimeToEdit, uint8_t *eepromAddr, char input, const uint8_t doneState, const uint8_t thisState) {

  if (*pEnter) {

    Usart_Tx(2);

    // Break up the var being edited
    editTens = CHAR2BCD2(*pTimeToEdit);

    editOnes = (editTens & 0x0F) + '0';
    editTens = (editTens >> 4) + '0';
    editingTens = TRUE;

    *pEnter = 0;

  } else  {

    LCD_putc(0, 'E');
    LCD_putc(1, 'N');
    LCD_putc(2, ' ');
    if (editingTens) {
      LCD_putc(3, editTens|0x80 );
      LCD_putc(4, editOnes);
    } else {
      LCD_putc(3, editTens);
      LCD_putc(4, editOnes|0x80);
    }
    if (eepromAddr == EEPROM_RUN) {
      LCD_putc(5, 's');
    } else {
      LCD_putc(5, 'h');
    }
    LCD_putc(6, '\0');

    if (input != KEY_NULL) {
      LCD_FlashReset();
    }

    LCD_UpdateRequired(TRUE, 0);
  }


  if (input != KEY_NULL) {
    LCD_FlashReset();
  }

  if ( input == KEY_MINUS || input == KEY_PLUS) {

    if (input == KEY_MINUS) {
      Usart_Tx(0x10);
      editCurChar--;
    } else {
      Usart_Tx(0x11);
      editCurChar++;
    }

    if (editCurChar < '0') {
      editCurChar = '9';
    } else if (editCurChar > '9') {
      editCurChar = '0';
    }

    if (editingTens) {
      editTens = editCurChar;
    } else {
      editOnes = editCurChar;
    }

  } else if (input == KEY_PREV || input == KEY_NEXT) {

    if (editingTens) {
      Usart_Tx(0x20);
      editingTens = FALSE;
      editCurChar = editOnes;
    } else {
      Usart_Tx(0x21);
      editingTens = TRUE;
      editCurChar = editTens;
    }

  } else if (input == KEY_ENTER) {

    Usart_Tx(0x50);

    *pTimeToEdit = BCD22CHAR(((editTens-'0') << 4)+(editOnes-'0'));

    eeprom_write_byte(eepromAddr, *pTimeToEdit);

    *pEnter = 1;
    return doneState;
  }

//  Usart_Tx('X');

  return thisState;

}

