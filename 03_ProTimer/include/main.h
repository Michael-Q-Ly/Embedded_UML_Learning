#ifndef MAIN_H_
#define MAIN_H_
#include <Arduino.h>
#include <stdint.h>

#define BAUD_RATE      115200

/* Button Pins */
byte const PIN_BUTTON1  = 2 ;
byte const PIN_BUTTON2  = 3 ;
byte const PIN_BUTTON3  = 4 ;
/* Buzzer Pin */
byte const PIN_BUZZER   = 12 ;
/* LCD Pins */
byte const PIN_LCD_RS   = 5 ;
byte const PIN_LCD_RW   = 6 ;
byte const PIN_LCD_EN   = 7 ;
byte const PIN_LCD_D4   = 8 ;
byte const PIN_LCD_D5   = 9 ;
byte const PIN_LCD_D6   = 10 ;
byte const PIN_LCD_D7   = 11 ;

/* Button Encodings */
#define BTN_PAD_VALUE_INC_TIME      B100
#define BTN_PAD_VALUE_DEC_TIME      B010
#define BTN_PAD_VALUE_ABRT          B110
#define BTN_PAD_VALUE_SP            B001

/* Constants */
uint8_t const SEC2Min_Conversion_Factor = 60 ;


/* Signals of the application */
typedef enum {
    INC_TIME ,
    DEC_TIME ,
    TIME_TICK ,
    START_PAUSE ,
    ABRT ,
    /* Internal activity signals */
    ENTRY ,
    EXIT
} Protimer_Signal_t ;

/* Various states of the application */
typedef enum {
    IDLE ,
    TIME_SET ,
    COUNTDOWN ,
    PAUSE ,
    STAT
} Protimer_State_t ;

/* Main application structure */
typedef struct {
    uint32_t current_time ;
    uint32_t elapsed_time ;
    uint32_t productive_time ;
    Protimer_State_t active_state ;
} Protimer_t ;

/* Generic (Super) event structure */
typedef struct {
    uint8_t sig ;
} Event_t ;

/* For user-generated events */
typedef struct {
    Event_t super ;
} Protimer_User_Event_t ;

/* For tick event */
typedef struct {
    Event_t super ;
    uint8_t ss ;
} Protimer_Tick_Event_t ;

/* For event handling */
typedef enum {
    EVENT_HANDLED ,
    EVENT_IGNORED ,
    EVENT_TRANSITION
} Event_Status_t ;

/* Function prototypes */
extern void proTimer_init                       ( Protimer_t *const mobj ) ;
extern Event_Status_t proTimer_state_machine    ( Protimer_t *const mobj, Event_t const *const e ) ;                // Pointer cannot be modified, but data can be

#endif  /* MAIN_H_ */