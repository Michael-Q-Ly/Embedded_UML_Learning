#include "main.h"

/* Main application object */
static Protimer_t proTimer ;

static void proTimer_event_dispatcher( Protimer_t *const mobj, Event_t const *const e) ;
static uint8_t process_button_pad_value( uint8_t btn_pad_value ) ;

void setup() {
    proTimer_init( &proTimer ) ;
}

void loop() {
    uint8_t b1 ;
    uint8_t b2 ;
    uint8_t b3 ;
    uint8_t btn_pad_value ;
    Protimer_User_Event_t user_event ;

    // 1. Read the button pad status
    b1 = digitalRead( PIN_BUTTON1 ) ;
    b2 = digitalRead( PIN_BUTTON2 ) ;
    b3 = digitalRead( PIN_BUTTON3 ) ;
    // Bit-extension to get pad values for encoding
    btn_pad_value = ( b1<<2 ) | ( b2<<1 ) | ( b3<<0 ) ;

    // Software button debouncing
    btn_pad_value = process_button_pad_value( btn_pad_value ) ;
    // 2. Make an event
    if ( btn_pad_value ) {
        if ( ! ( btn_pad_value % BTN_PAD_VALUE_INC_TIME ) ) {
            user_event.super.sig = INC_TIME ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_DEC_TIME ) ) {
            user_event.super.sig = DEC_TIME ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_SP ) ) {
            user_event.super.sig = START_PAUSE ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_ABRT ) ) {
            user_event.super.sig = ABRT ;
        }
    }

    // 3. Send it to event dispatcher
    proTimer_event_dispatcher( &proTimer, &user_event.super ) ;

    #ifdef later
    #endif /* later */
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

static void proTimer_event_dispatcher( Protimer_t *const mobj, Event_t const *const e) {
    Event_Status_t      status ;
    Protimer_State_t    source ;
    Protimer_State_t    target ;

    source = mobj -> active_state ;
    status = proTimer_state_machine( mobj, e ) ;

    if ( status != EVENT_TRANSITION ) {
        ;
    }
    else {
        Event_t ee ;
        target = mobj -> active_state ;

        // 1. Run exit action for the source state
        ee.sig = EXIT ;
        mobj -> active_state = source ;
        proTimer_state_machine( mobj, &ee ) ;
        // 2. Run the entry action for the target state
        ee.sig = ENTRY ;
        mobj -> active_state = target ;
        proTimer_state_machine( mobj, &ee ) ;
    }
}

static uint8_t process_button_pad_value( uint8_t btn_pad_value ) {
    return btn_pad_value ;
}