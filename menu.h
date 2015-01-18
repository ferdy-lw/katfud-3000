// menu.h

// mt __flash typedef struct
typedef struct PROGMEM
{
    unsigned char state;
    unsigned char input;
    unsigned char nextstate;
} MENU_NEXTSTATE;


// mt __flash typedef struct
typedef struct PROGMEM
{
    unsigned char state;
    // char __flash *pText;
    PGM_P pText;
    char (*pFunc)(char input);
} MENU_STATE;

/*
 *

 Kat Fud
 Start XXXh  ->  Set
 Period XXXh ->  Set
 Run XXXs -> Set
 GO
 OPTIONS -> Bootloader


 */

// Menu text
const char MT_AVRBF[] PROGMEM                     =	{'K','A','T',' ','F','U','D',' ','3'+0x80,'0'+0x80,'0'+0x80,'0'+0x80,'\0'};
//const char MT_START[] PROGMEM                     = "Start";
const char MT_START_SET[] PROGMEM                 = "Set St";
//const char MT_PERIOD[] PROGMEM                    = "Period";
const char MT_PERIOD_SET[] PROGMEM                = "Set Pd";
//const char MT_RUN[] PROGMEM                       = "Run";
const char MT_RUN_SET[] PROGMEM                   = "Set Rn";
const char MT_GO[] PROGMEM                        = "GO NOW";
const char MT_OPTIONS[] PROGMEM                   = "Options";
const char MT_OPTIONS_DISPLAY[] PROGMEM           = "Display";
const char MT_OPTIONS_DISPLAY_CONTRAST[] PROGMEM  = "Adjust contrast";
const char MT_OPTIONS_BOOT[] PROGMEM              = "Bootloader";


const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {
//  STATE                       INPUT       NEXT STATE
    {ST_AVRBF,                  KEY_PLUS,   ST_OPTIONS},
    {ST_AVRBF,                  KEY_NEXT,   ST_AVRBF_REV},
    {ST_AVRBF,                  KEY_MINUS,  ST_START},

    {ST_AVRBF_REV,              KEY_PREV,   ST_AVRBF},

//    {ST_START,                  KEY_PLUS,   ST_AVRBF},
//    {ST_START,                  KEY_NEXT,   ST_START_SET},
//    {ST_START,                  KEY_PREV,   ST_AVRBF},
//    {ST_START,                  KEY_MINUS,  ST_PERIOD},

    {ST_START_SET,              KEY_ENTER,  ST_START_SET_FUNC},
    {ST_START_SET,              KEY_PREV,   ST_START},

//    {ST_PERIOD,                 KEY_PLUS,   ST_START},
//    {ST_PERIOD,                 KEY_NEXT,   ST_PERIOD_SET},
//    {ST_PERIOD,                 KEY_PREV,   ST_AVRBF},
//    {ST_PERIOD,                 KEY_MINUS,  ST_RUN},

    {ST_PERIOD_SET,             KEY_ENTER,  ST_PERIOD_SET_FUNC},
    {ST_PERIOD_SET,             KEY_PREV,   ST_PERIOD},

//    {ST_RUN,                    KEY_PLUS,   ST_PERIOD},
//    {ST_RUN,                    KEY_NEXT,   ST_PERIOD_SET},
//    {ST_RUN,                    KEY_PREV,   ST_AVRBF},
//    {ST_RUN,                    KEY_MINUS,  ST_GONOW},

    {ST_RUN_SET,                KEY_ENTER,  ST_RUN_SET_FUNC},
    {ST_RUN_SET,                KEY_PREV,   ST_RUN},

    {ST_GONOW,                  KEY_PLUS,   ST_RUN},
    {ST_GONOW,                  KEY_ENTER,  ST_GONOW_FUNC},
    {ST_GONOW,                  KEY_PREV,   ST_AVRBF},
    {ST_GONOW,                  KEY_MINUS,  ST_OPTIONS},

    {ST_OPTIONS,                KEY_PLUS,   ST_GONOW},
    {ST_OPTIONS,                KEY_NEXT,   ST_OPTIONS_DISPLAY},
    {ST_OPTIONS,                KEY_PREV,   ST_AVRBF},
    {ST_OPTIONS,                KEY_MINUS,  ST_AVRBF},

    {ST_OPTIONS_DISPLAY,        KEY_PLUS,   ST_OPTIONS_BOOT},
    {ST_OPTIONS_DISPLAY,        KEY_NEXT,   ST_OPTIONS_DISPLAY_CONTRAST},
    {ST_OPTIONS_DISPLAY,        KEY_PREV,   ST_OPTIONS},
    {ST_OPTIONS_DISPLAY,        KEY_MINUS,  ST_OPTIONS_BOOT},

    {ST_OPTIONS_DISPLAY_CONTRAST, KEY_ENTER,    ST_OPTIONS_DISPLAY_CONTRAST_FUNC},
    {ST_OPTIONS_DISPLAY_CONTRAST, KEY_PREV,     ST_OPTIONS_DISPLAY},

    {ST_OPTIONS_BOOT,             KEY_PLUS,     ST_OPTIONS_DISPLAY},
    {ST_OPTIONS_BOOT,             KEY_NEXT,     ST_OPTIONS_BOOT_FUNC},
    {ST_OPTIONS_BOOT,             KEY_PREV,     ST_OPTIONS},
    {ST_OPTIONS_BOOT,             KEY_MINUS,    ST_OPTIONS_DISPLAY},

    {0,                         0,          0}
};


// mt MENU_STATE menu_state[] = {
const MENU_STATE menu_state[] PROGMEM = {
//  STATE                               STATE TEXT                  STATE_FUNC
    {ST_AVRBF,                          MT_AVRBF,                   NULL},
    {ST_AVRBF_REV,                      NULL,                       Revision},
    {ST_START,                          NULL,                       StartShow},
    {ST_START_SET,                      MT_START_SET,               NULL},
    {ST_START_SET_FUNC,                 NULL,                       StartSetFunc},
    {ST_PERIOD,                         NULL,                       PeriodShow},
    {ST_PERIOD_SET,                     MT_PERIOD_SET,              NULL},
    {ST_PERIOD_SET_FUNC,                NULL,                       PeriodSetFunc},
    {ST_RUN,                            NULL,                       RunShow},
    {ST_RUN_SET,                        MT_RUN_SET,                 NULL},
    {ST_RUN_SET_FUNC,                   NULL,                       RunSetFunc},
    {ST_GONOW,                          MT_GO,                      NULL},
    {ST_GONOW_FUNC,                     NULL,                       GoNowFunc},
    {ST_GONOWWAIT_FUNC,                 NULL,                       GoNowWaitFunc},
    {ST_GONOWMOTOR_FUNC,                NULL,                       GoNowMotorFunc},
    {ST_OPTIONS,                        MT_OPTIONS,                 NULL},
    {ST_OPTIONS_DISPLAY,                MT_OPTIONS_DISPLAY,         NULL},
    {ST_OPTIONS_DISPLAY_CONTRAST,       MT_OPTIONS_DISPLAY_CONTRAST,NULL},
    {ST_OPTIONS_DISPLAY_CONTRAST_FUNC,  NULL,                       SetContrast},
    {ST_OPTIONS_BOOT,                   MT_OPTIONS_BOOT,            NULL},
    {ST_OPTIONS_BOOT_FUNC,              NULL,                       BootFunc},

    {0,                                 NULL,                       NULL},

};
