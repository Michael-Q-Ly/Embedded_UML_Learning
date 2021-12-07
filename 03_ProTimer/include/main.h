#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>
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