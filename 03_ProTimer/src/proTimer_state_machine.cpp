#include "main.h"
#include "lcd.h"

// Prototypes of state handlers
static Event_Status_t protimer_state_handler_IDLE       ( Protimer_t *const mobj, Event_t const *const e ) ;        // Make mobj a constant pointer with changeable data,
static Event_Status_t protimer_state_handler_TIME_SET   ( Protimer_t *const mobj, Event_t const *const e ) ;        // but make the event e a constant pointer with
static Event_Status_t protimer_state_handler_PAUSE      ( Protimer_t *const mobj, Event_t const *const e ) ;        // constant data.
static Event_Status_t protimer_state_handler_COUNTDOWN  ( Protimer_t *const mobj, Event_t const *const e ) ;
static Event_Status_t protimer_state_handler_STAT       ( Protimer_t *const mobj, Event_t const *const e ) ;
// Prototypes of helper functions
static void display_time                                ( uint32_t time ) ;
static void display_message                             ( String message, uint8_t col, uint8_t row ) ;
static void display_clear                               ( void ) ;
static void do_beep                                     ( void ) ;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void proTimer_init( Protimer_t *mobj ) {
    Event_t entry_event ;
    entry_event.sig = ENTRY ;

    mobj -> active_state    = IDLE ;
    mobj -> productive_time = 0 ;
    proTimer_state_machine( mobj, &entry_event ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Event_Status_t proTimer_state_machine( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( mobj -> active_state ) {
        case IDLE :
            return protimer_state_handler_IDLE( mobj, e ) ;
        case TIME_SET :
            return protimer_state_handler_TIME_SET( mobj, e ) ;   
        case COUNTDOWN :
            return protimer_state_handler_COUNTDOWN( mobj, e ) ;   
        case PAUSE :
            return protimer_state_handler_PAUSE( mobj, e ) ;
        case STAT :
            return protimer_state_handler_STAT( mobj, e ) ;   
    } // switch
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_IDLE( Protimer_t *const mobj, Event_t const *const e ) {
    switch( e -> sig ) {
        case ENTRY : {
            mobj -> current_time = 0 ;
            mobj -> elapsed_time = 0 ;
            display_time( 0 ) ;
            display_message( "Set", 0, 0 ) ;
            display_message( "time", 0, 1 ) ;
            return EVENT_HANDLED ;
        } // ENTRY
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        } // EXIT
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
            mobj -> active_state = TIME_SET ;
            return EVENT_TRANSITION ;
        } // INC_TIME
        case START_PAUSE : {
            mobj -> active_state = STAT ;
            return EVENT_TRANSITION ;
        } // START_PAUSE
        case TIME_TICK : {
            // if ( !( ( (( Protimer_Tick_Event_t * )( e )) -> ss ) % 5 ) ) {
            if ( (( Protimer_Tick_Event_t * )( e )) -> ss == 5 ) {
                do_beep() ;
                return EVENT_HANDLED ;
            }
            return EVENT_IGNORED ;
        } // TIME_TICK
    }   // switch
    return EVENT_IGNORED ;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_TIME_SET( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case ENTRY : {
            display_time( mobj -> current_time ) ;
            return EVENT_HANDLED ;
        } // ENTRY
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        } // EXIT
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
            display_time( mobj -> current_time ) ;
            return EVENT_HANDLED ;
        } // INC_TIME
        case DEC_TIME : {
            if ( mobj -> current_time >= ( 1*SEC2Min_Conversion_Factor ) ) {
                mobj -> current_time -= ( 1*SEC2Min_Conversion_Factor ) ;
                display_time( mobj -> current_time ) ;
                return EVENT_HANDLED ;
            }
            return EVENT_IGNORED ;
        } // DEC_TIME
        case START_PAUSE : {
            if ( mobj -> current_time >= ( 1*SEC2Min_Conversion_Factor ) ) {
                mobj -> active_state = COUNTDOWN ;
                return EVENT_TRANSITION ;   // Transition to COUNTDOWN
            }
            return EVENT_IGNORED ;
        } // START_PAUSE
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;       // Go back to IDLE
        } // ABRT
    } // switch
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_COUNTDOWN( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case EXIT : {
            mobj -> productive_time += mobj -> elapsed_time ;
            mobj -> elapsed_time = 0 ;
            return EVENT_HANDLED ;
        } // EXIT
        case TIME_TICK : {
            // ss is a member of Protimer_Tick_Event_t, not Event_t ; typecast
            if ( !( ( (( Protimer_Tick_Event_t * )( e )) -> ss ) % 10 ) ) {
            // if ( (( Protimer_Tick_Event_t * )( e )) -> ss == 10 ) {
                --mobj -> current_time ;
                ++mobj -> elapsed_time ;
                display_time( mobj -> current_time ) ;
                if ( ! ( mobj -> current_time ) ) {
                    mobj -> active_state = IDLE ;
                    return EVENT_TRANSITION ;
                }
                return EVENT_HANDLED ;
            }
            return EVENT_IGNORED ;
        } // TIME_TICK
        case START_PAUSE : {
            mobj -> active_state = PAUSE ;
            return EVENT_TRANSITION ;
        } // START_PAUSE
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;
        } // ABRT
    } // switch
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_PAUSE( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case ENTRY: {
            display_message( "Paused", 5, 1 ) ;
            return EVENT_HANDLED ;
        } // ENTRY
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        } // EXIT
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
            mobj -> active_state = TIME_SET ;
            return EVENT_TRANSITION ;
        } // INC_TIME
        case DEC_TIME : {
            if ( mobj -> current_time >= 1*SEC2Min_Conversion_Factor ) {
                mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
                mobj -> active_state = TIME_SET ;
                return EVENT_TRANSITION ;
            }
            return EVENT_IGNORED ;
        } // DEC_TIME
        case START_PAUSE : {
            mobj -> active_state = COUNTDOWN ; ;
            return EVENT_TRANSITION ;
        } // START_PAUSE
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;
        } // ABRT
    } // switch
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_STAT( Protimer_t *const mobj, Event_t const *const e ) {
    static uint8_t tick_count ;

    switch( e -> sig ) {
        case ENTRY : {
            display_time( mobj -> productive_time ) ;
            display_message( "Productive Time", 1, 1 ) ;
            return EVENT_HANDLED ;
        } // ENTRY
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        } // EXIT
        case TIME_TICK : {
            if ( ++tick_count == 0.5 * SEC2Min_Conversion_Factor ) {
                tick_count = 0 ;
                mobj -> active_state = IDLE ;
                return EVENT_TRANSITION ;
            }
            return EVENT_IGNORED ;
        } // TIME_TICK
    } // switch
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_time( uint32_t time ) {
    char buf[7] ;
    String time_msg ;

    uint16_t min = time / SEC2Min_Conversion_Factor ;
    uint16_t sec = time % SEC2Min_Conversion_Factor ;
    sprintf( buf, "%03d:%02d", min, sec ) ;

    time_msg = ( String )( buf ) ;
    lcd_set_cursor( 5, 0 ) ;
    lcd_print_string( time_msg ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_message( String message, uint8_t col, uint8_t row ) {
    lcd_set_cursor( col, row ) ;
    lcd_print_string( message ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_clear( void ) {
    lcd_clear() ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void do_beep( void ) {
    tone( PIN_BUZZER, 4000, 25 ) ;
}
