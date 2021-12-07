#include "main.h"

/* Main application object */
static Protimer_t proTimer ;

typedef enum {
    NOT_PRESSED ,
    BOUNCE , 
    PRESSED
} Button_State_t ;

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
    static uint32_t current_time ;
    static Protimer_Tick_Event_t tick_event ;

    current_time = millis() ;

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

    // 4. Dispatch the time tick event every 100ms
    if ( millis() - current_time > 100 ) {      // Instead of using millis, you may also use a hardware timer interrupt
        // 100mS have passed
        current_time = millis() ;
        tick_event.super.sig = TIME_TICK ;
        if ( ++tick_event.ss > 10 ) {
            tick_event.ss = 1 ;
        }
        proTimer_event_dispatcher( &proTimer, &tick_event.super) ;
    }
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
    static Button_State_t btn_sm_state ;
    static uint32_t current_time ;

    btn_sm_state = NOT_PRESSED ;
    current_time = millis() ;

    switch ( btn_sm_state ) {
        case NOT_PRESSED : {
            if ( btn_pad_value ) {
                btn_sm_state = BOUNCE ;
                current_time = millis() ;
            }
            break ;
        }
        case BOUNCE : {
            if ( millis() - current_time >= 50 ) {
                // 50ms have passed
                if ( btn_pad_value ) {
                    btn_sm_state = PRESSED ;
                    return btn_pad_value ;
                }
                else {
                    btn_sm_state = NOT_PRESSED ;
                }
            }
            break ;
        }
        case PRESSED : {
            if ( ! ( btn_pad_value ) ) {
                btn_sm_state = BOUNCE ;
                current_time = millis() ;
            }
            break ;
        }
        default: {

        }
    }

    return 0 ;
}