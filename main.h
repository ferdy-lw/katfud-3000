//Revisions number
#define SWHIGH  0
#define SWLOW   0
#define SWLOWLOW 2

// main.h

void Initialization(void);
unsigned char StateMachine(char state, unsigned char stimuli);
char BootFunc(char input);
//char PowerSaveFunc(char input);
//char AutoPower(char input);
//char KeyClick(char input);
void Delay(unsigned int millisec);
char Revision(char input);
void OSCCAL_calibration(void);
void OSCCAL_retrocal(void);

#define BOOL    char

#define FALSE   0
#define TRUE    (!FALSE)
#define NULL    0

#define AUTO    3

// Macro definitions
//mtA -
// sbi and cbi are not longer supported by the avr-libc
// to avoid version-conflicts the macro-names have been
// changed to sbiBF/cbiBF "everywhere"
#define sbiBF(port,bit)  (port |= (1<<bit))   //set bit in port
#define cbiBF(port,bit)  (port &= ~(1<<bit))  //clear bit in port
//mtE

// Menu state machine states
#define ST_AVRBF                        10
#define ST_AVRBF_REV                    11
#define ST_START                        20
#define ST_START_SET                    21
#define ST_START_SET_FUNC               22
#define ST_PERIOD                       30
#define ST_PERIOD_SET                   31
#define ST_PERIOD_SET_FUNC              32
#define ST_RUN                          40
#define ST_RUN_SET                      41
#define ST_RUN_SET_FUNC                 42
#define ST_GONOW                        50
#define ST_GONOW_FUNC                   51
#define ST_GONOWWAIT_FUNC               52
#define ST_GONOWMOTOR_FUNC              53
#define ST_OPTIONS                      90
#define ST_OPTIONS_DISPLAY              91
#define ST_OPTIONS_DISPLAY_CONTRAST     92
#define ST_OPTIONS_DISPLAY_CONTRAST_FUNC 93
#define ST_OPTIONS_BOOT                 94
#define ST_OPTIONS_BOOT_FUNC            95


