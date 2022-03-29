#include "main.h"
#include "lcd.h"

/*Main application object */
static Protimer_t protimer;

static void protimer_event_dispatcher   ( Protimer_t *const mobj, Event_t const *const e ) ;
static uint8_t process_button_pad_value ( uint8_t btn_pad_value ) ;
static void display_init                ( void ) ;

void setup() {
    // put your setup code here, to run once:
    Serial.begin( BAUD_RATE ) ;
    Serial.println("Productive timer application");
    Serial.println("===========================");
    pinMode( PIN_BUTTON1, INPUT ) ;
    pinMode( PIN_BUTTON2, INPUT ) ;
    pinMode( PIN_BUTTON3, INPUT ) ;

    display_init();
    proTimer_init(&protimer);
}

void loop() {
    uint8_t b1 ;
    uint8_t b2 ;
    uint8_t b3 ;
    uint8_t btn_pad_value ;
    Protimer_User_Event_t user_event ;
    static uint32_t current_time = millis() ;
    static Protimer_Tick_Event_t tick_event;

    // 1. Read the button pad status
    b1 = digitalRead( PIN_BUTTON1 ) ; 
    b2 = digitalRead( PIN_BUTTON2 ) ;
    b3 = digitalRead( PIN_BUTTON3 ) ;

    btn_pad_value = ( b1<<2 ) | ( b2<<1 )| ( b3 << 0 ) ;

    // Software button de-bouncing 
    btn_pad_value = process_button_pad_value( btn_pad_value );

    //2. Make an event 
    if ( btn_pad_value ) {
        if ( ! ( btn_pad_value % BTN_PAD_VALUE_ABRT ) ) {
            user_event.super.sig = ABRT ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_INC_TIME ) ) {
            user_event.super.sig = INC_TIME ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_DEC_TIME ) ) {
            user_event.super.sig = DEC_TIME ;
        }
        else if ( ! ( btn_pad_value % BTN_PAD_VALUE_SP ) ) {
            user_event.super.sig = START_PAUSE ;
        }
        //3. Send it to event dispatcher
        protimer_event_dispatcher( &protimer, &user_event.super ) ;
    }

    //4. Dispatch the time tick event for every 100ms
    if ( millis() - current_time >= 100 ) {
        // 100ms have passed
        current_time = millis() ;
        tick_event.super.sig = TIME_TICK ;
        if ( ++tick_event.ss > 10 ) {
            tick_event.ss = 1 ;
        }
        protimer_event_dispatcher( &protimer,&tick_event.super ) ;
    }
} // end loop


static void protimer_event_dispatcher( Protimer_t *const mobj,Event_t const *const e ) {
    Event_Status_t status;
    Protimer_State_t source;
    Protimer_State_t target;
    
    source = mobj -> active_state ;
    status = proTimer_state_machine( mobj, e ) ;

    if ( status == EVENT_TRANSITION ) {
        Event_t exit_event ;
        target = mobj -> active_state ;
        // 1. Run the exit action for the source state
        exit_event.sig = EXIT ;
        mobj -> active_state = source ;
        proTimer_state_machine( mobj, &exit_event ) ;

        // 2. Run the entry action for the target state
        exit_event.sig = ENTRY ;
        mobj -> active_state = target ;
        proTimer_state_machine( mobj, &exit_event ) ;
    }
} 


static uint8_t process_button_pad_value( uint8_t btn_pad_value ) {
    static Button_State_t btn_sm_state = NOT_PRESSED ;
    static uint32_t current_time = millis() ;

    switch( btn_sm_state ) {
        case NOT_PRESSED: {
            if ( btn_pad_value ) {
                btn_sm_state = BOUNCE ;
                current_time = millis() ;
            }
            break ;
        } // NOT_PRESSED
        case BOUNCE: {
            if ( millis() - current_time >= 50 ) {
                // 50ms has passed 
                if ( btn_pad_value ) {
                    btn_sm_state = PRESSED ;
                    return btn_pad_value ;
                }
                else
                    btn_sm_state = NOT_PRESSED ;
            }
            break ;
        } // BOUNCE
        case PRESSED: {
            if ( !btn_pad_value ) {
                btn_sm_state = BOUNCE ;
                current_time = millis() ;
            }
            break ;
        } // PRESSED
    } // switch

    return 0;
}

static void display_init(void) {
    lcd_begin(16,2);
    lcd_clear();
    lcd_move_cursor_L_to_R();
    lcd_set_cursor(0,0);
    lcd_no_auto_scroll();
    lcd_cursor_off();
}
