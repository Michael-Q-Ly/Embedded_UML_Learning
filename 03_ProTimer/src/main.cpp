#include "main.h"

/* Main application object */
static Protimer_t proTimer ;

static void proTimer_event_dispatcher( Protimer_t *const mobj, Event_t const *const e) ;

void setup() {
    proTimer_init( &proTimer ) ;
}

void loop() {
    // 1. Read the button pad status
    // 2. Make an event
    // 3. Send it to event dispatcher
    #ifdef later
        proTimer_event_dispatcher() ;
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