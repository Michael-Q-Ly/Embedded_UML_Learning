#include "main.h"

void proTimer_init( Protimer_t *mobj ) {
    mobj -> active_state    = IDLE ;
    mobj -> productive_time = 0 ;
}