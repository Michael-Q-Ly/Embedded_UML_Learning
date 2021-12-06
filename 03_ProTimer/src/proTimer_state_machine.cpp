#include "main.h"

void proTimer_init( Protimer_t *mobj ) {
    mobj -> active_state    = IDLE ;
    mobj -> productive_time = 0 ;
}

Event_Status_t protimer_state_machine( Protimer_t *mobj, Event_t *e ) {
    switch ( mobj -> active_state ) {
        case IDLE : {
            return protimer_state_handler_IDLE( mobj, e ) ;
        }
        case TIME_SET :{
            return protimer_state_handler_TIME_SET( mobj, e ) ;   
        }
        
        default :
            break ;
    }
}
Event_Status_t protimer_state_handler_IDLE( Protimer_t *mobj, Event_t *e ) {
    switch( e -> sig ) {
        case ENTRY : {
            mobj -> current_time = 0 ;
            mobj -> elapsed_time = 0 ;
            display_time( 0 ) ;                 // TODO: implement display time function
            display_message( "Set Time" ) ;     // TODO: implement message function
            return EVENT_HANDLED ;
        }
        case EXIT : {
            display_clear() ;                   // TODO: implement display clear function
            return EVENT_HANDLED ;
        }
        case INC_TIME : {
            mobj -> current_time += 60 ;
            mobj -> active_state = TIME_SET ;
            return EVENT_TRANSITION ;
        }
        case DEC_TIME : {

        }
        case START_PAUSE : {
            mobj -> active_state = STAT ;
            return EVENT_TRANSITION ;
        }
        case TIME_TICK : {
            if ( (( Protimer_Tick_Event_t * )( e )) -> ss == 5 ) {
                do_beep() ;                 // TODO: Implement do_beep function
                return EVENT_HANDLED ;
            }
            else {
                return EVENT_IGNORED ;
            }
        }
    }   /* SWITCH */

    return EVENT_IGNORED ;
}



Event_Status_t protimer_state_handler_TIME_SET( Protimer_t *mobj, Event_t *e ) {

}