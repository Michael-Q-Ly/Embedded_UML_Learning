#ifndef MAIN_H_
#define MAIN_H_

#include <Arduino.h>

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

#endif  /* MAIN_H_ */