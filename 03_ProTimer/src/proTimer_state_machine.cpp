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

void proTimer_init( Protimer_t *const mobj ) {
    mobj -> active_state    = IDLE ;
    mobj -> productive_time = 0 ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Event_Status_t proTimer_state_machine( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( mobj -> active_state ) {
        case IDLE : {
            return protimer_state_handler_IDLE( mobj, e ) ;
        }
        case TIME_SET : {
            return protimer_state_handler_TIME_SET( mobj, e ) ;   
        }
        case PAUSE : {
            return protimer_state_handler_PAUSE( mobj, e ) ;
        }
        case COUNTDOWN : {
            return protimer_state_handler_COUNTDOWN( mobj, e ) ;   
        }
        case STAT : {
            return protimer_state_handler_STAT( mobj, e ) ;   
        }
        default :
            return EVENT_IGNORED ;
    }
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
        }
        case EXIT : {
            display_clear() ;                   // TODO: implement display clear function
            return EVENT_HANDLED ;
        }
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
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
            if ( !( (( Protimer_Tick_Event_t * )( e )) -> ss == 5 ) ) {
                do_beep() ;                 // TODO: Implement do_beep function
                return EVENT_HANDLED ;
            }
            else {
                return EVENT_IGNORED ;
            }
        }
        default : {
            return EVENT_IGNORED ;
        }
    }   /* SWITCH */

    // return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_TIME_SET( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case ENTRY : {
            display_time( mobj -> current_time ) ;
            return EVENT_HANDLED ;
        }
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        }
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
            display_time( mobj -> current_time ) ;
            return EVENT_HANDLED ;
        }
        case DEC_TIME : {
            if ( mobj -> current_time >= ( 1*SEC2Min_Conversion_Factor ) ) {
                mobj -> current_time -= ( 1*SEC2Min_Conversion_Factor ) ;
                display_time( mobj -> current_time ) ;
                return EVENT_HANDLED ;
            }
            else {
                return EVENT_IGNORED ;
            }
        }
        case START_PAUSE : {
            if ( mobj -> current_time >= ( 1*SEC2Min_Conversion_Factor ) ) {
                mobj -> active_state = COUNTDOWN ;
                return EVENT_TRANSITION ;   // Transition to COUNTDOWN
            }
            else {
                return EVENT_IGNORED ;
            }
        }
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;       // Go back to IDLE
        }
        // Why not a default that returns ignored instead?
    }
    return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_PAUSE( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case ENTRY: {
            display_message( "Paused", 5, 1 ) ;
            return EVENT_HANDLED ;
        }
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        }
        case INC_TIME : {
            mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
            return EVENT_TRANSITION ;
        }
        case DEC_TIME : {
            if ( mobj -> current_time >= 1*SEC2Min_Conversion_Factor ) {
                mobj -> current_time += ( 1*SEC2Min_Conversion_Factor ) ;
                return EVENT_TRANSITION ;
            }
            else {
                return EVENT_IGNORED ;
            }
        }
        case START_PAUSE : {
            mobj -> active_state = PAUSE ;
            return EVENT_TRANSITION ;
        }
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;
        }
        default : {
            return EVENT_IGNORED ;
        }
    }
    // return EVENT_IGNORED ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_COUNTDOWN( Protimer_t *const mobj, Event_t const *const e ) {
    switch ( e -> sig ) {
        case EXIT : {
            mobj -> productive_time += mobj -> elapsed_time ;
            mobj -> elapsed_time = 0 ;
            return EVENT_HANDLED ;
        }
        case TIME_TICK : {
            // ss is a member of Protimer_Tick_Event_t, not Event_t ; typecast
            if ( ! ( ((Protimer_Tick_Event_t *)( e )) -> ss % 10 ) ) {
                --mobj -> current_time ;
                ++mobj -> elapsed_time ;
                display_time( mobj -> current_time ) ;
                if ( ! ( mobj -> current_time ) ) {
                    mobj -> active_state = IDLE ;
                    return EVENT_TRANSITION ;
                }
                else {
                    return EVENT_HANDLED ;
                }
            }
        }
        case START_PAUSE : {
            mobj -> active_state = PAUSE ;
            return EVENT_TRANSITION ;
        }
        case ABRT : {
            mobj -> active_state = IDLE ;
            return EVENT_TRANSITION ;
        }
        default: {
            return EVENT_IGNORED ;
        }
    }   /* Switch */
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static Event_Status_t protimer_state_handler_STAT( Protimer_t *const mobj, Event_t const *const e ) {
    switch( e -> sig ) {
        case ENTRY : {
            display_time( mobj -> productive_time ) ;
            display_message( "Productive Time", 1, 1 ) ;
            return EVENT_HANDLED ;
        }
        case EXIT : {
            display_clear() ;
            return EVENT_HANDLED ;
        }
        case TIME_TICK : {
            if ( ! ( ((Protimer_Tick_Event_t *)( e )) -> ss == 10 ) ) {
                mobj -> active_state = IDLE ;
                return EVENT_TRANSITION ;
            }
            return EVENT_IGNORED ;
        }
        default : {
            return EVENT_IGNORED ;
        }
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_time( uint32_t time ) {
    uint16_t min ;
    uint16_t sec ;

    uint8_t minDigits ;
    uint8_t secDigits ;

    min = time / SEC2Min_Conversion_Factor ;
    sec = time % SEC2Min_Conversion_Factor ;

    minDigits = check_minute_digits( min ) ;
    secDigits = check_second_digits( sec ) ;
    
    lcd_set_cursor( 0, 5 ) ;

    switch ( minDigits ) {
        case ONES : {
            lcd_print_number( 0 ) ;
            lcd_print_number( 0 ) ;
            lcd_print_number( min ) ;
        }
        case TENS: {
            lcd_print_number( 0 ) ;
            lcd_print_number( min ) ;
        }
        case HUNDREDS : {
            lcd_print_number( min ) ;
        }
        // Add default??
    }   /* switch( minDigits ) */

    lcd_print_char( ':' ) ;

    switch ( secDigits ) {
        case ONES : {
            lcd_print_number( 0 ) ;
            lcd_print_number( sec ) ;
        }
        case TENS: {
            lcd_print_number( sec ) ;
        }
        // Add default??
    }   /* switch( secDigits ) */

    #ifdef later
    lcd_print_number( min ) ;
    lcd_print_char( ':' ) ;
    lcd_print_number( sec ) ;
    #endif /* later */
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t check_minute_digits( uint16_t min ) {
    typedef enum {
        ONES ,
        TENS ,
        HUNDREDS
    } Place_t ;

    // Check placement of minutes
    if ( min < 10 ) {
        return ONES ;
    }
    else if ( min < 100 ) {
        return TENS ;
    }
    else {
        return HUNDREDS ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint16_t check_second_digits( uint16_t sec ) {
    typedef enum {
        ONES ,
        TENS ,
    } Place_t ;

    if ( sec < 10 ) {
        return ONES ;
    }
    else {
        return TENS ;
    }
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_message( String message, uint8_t col, uint8_t row ) {
    lcd_set_cursor( row, col ) ;
    lcd_print_string( message ) ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void display_clear( void ) {
    lcd_clear() ;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void do_beep( void ) {

}