/*
 *  Name: Michael Ly
 *  Date: December 5, 2021
 *  Description: Change LED brightness with PWM using a Moore Machine.
 *  Target: Arduino Uno R3
 *  Materials: Breadboard, Jumper wires, 220 Ohm Resistor, USB B cable
 */

/*
 * Implementation of State Machine Handler
 *      Nested Switch           <==== Use this for now
 *      State Table Approach
 *      State Handler Approach
 */

#include <Arduino.h>

// byte const LED = 9 ;
#define PIN_LED             9
#define BAUD_RATE           115200

#define LIGHT_BRIGHT_DIM    25
#define LIGHT_BRIGHT_MED    85
#define LIGHT_BRIGHT_FULL   255
#define LIGHT_BRIGHT_OFF    0

typedef enum {
    ON ,
    OFF
} Event_t ;

typedef enum {
    LIGHT_OFF ,
    LIGHT_DIM ,
    LIGHT_MEDIUM ,
    LIGHT_FULL
} Light_State_t ;

Light_State_t current_state ;

void light_init( void ) ;
void light_state_machine( uint8_t event )  ;
void light_change_intensity( uint8_t pin, uint8_t intensity ) ;
void run_entry_action( Light_State_t state ) ;


void setup() {
    // Set the baud rate to 115200 bits per second
    Serial.begin( BAUD_RATE ) ;
    // Initialize the current state:
    current_state = LIGHT_OFF ;
    // Menu
    Serial.println( "Light control application" ) ;
    Serial.println( "-------------------------" ) ;
    Serial.println( "Send 'x' or 'o'" ) ;
}

void loop() {
    uint8_t event ;

    if ( Serial.available() > 0 ) {

        event = Serial.read() ;

        if ( event == 'o' ) {
            light_state_machine( ON ) ;
        }
        else if ( event == 'x' ) {
            light_state_machine( OFF ) ;
        }

  }
}
////////////////////////////////////////////////////////////////////////////////////
////////////////////////////    Function Definitions    ////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

/* Initializes the led state to OFF */
void light_init( void ) {
    current_state = LIGHT_OFF ;
    run_entry_action( LIGHT_OFF ) ;
}
////////////////////////////////////////////////////////////////////////////////////

/* Determines what events will make one state change to another */
void light_state_machine( uint8_t event ) {

    Light_State_t previous_state ;

    previous_state = current_state ;

    switch ( current_state ) {
        
        case LIGHT_OFF : {

            switch ( event ) {
                case ON : {
                    current_state = LIGHT_DIM ;
                    break ;
                }
            }

            break ;
        }   /* LIGHT_OFF */


        case LIGHT_DIM : {

            switch ( event ) {
                case ON : {
                    current_state = LIGHT_MEDIUM ;
                    break ;
                }
                case OFF: {
                    current_state = LIGHT_OFF ;
                    break ;
                }
            }

            break ;
        }   /* LIGHT_DIM */


        case LIGHT_MEDIUM : {
            switch ( event ) {

                case ON : {
                    current_state = LIGHT_FULL ;
                    break ;
                }
                case OFF: {
                    current_state = LIGHT_OFF ;
                    break ;
                }
            }

            break ;
        }   /* LIGHT_MEDIUM */
        

        case LIGHT_FULL : {

            switch ( event ) {
                case ON : {
                    current_state = LIGHT_DIM ;
                    break ;
                }
                case OFF: {
                    current_state = LIGHT_OFF ;
                    break ;
                }
            }

            break ;
        }   /* LIGHT_FULL */

    }

    if ( previous_state != current_state ) {
        run_entry_action( current_state ) ;
    }

}
////////////////////////////////////////////////////////////////////////////////////

/* Changes the PWM of the LED */
void light_change_intensity( uint8_t pin, uint8_t intensity ) {
    analogWrite( pin, intensity ) ;
}
////////////////////////////////////////////////////////////////////////////////////

/* Changes the state of the LED */
void run_entry_action( Light_State_t state ) {
    switch ( state ) {
        case LIGHT_OFF : {
            light_change_intensity( PIN_LED, LIGHT_BRIGHT_OFF ) ;
            break ;
        }
        case LIGHT_DIM : {
            light_change_intensity( PIN_LED, LIGHT_BRIGHT_DIM ) ;
            break ;
        }
        case LIGHT_MEDIUM : {
            light_change_intensity( PIN_LED, LIGHT_BRIGHT_MED ) ;
            break ;
        }
        case LIGHT_FULL : {
            light_change_intensity( PIN_LED, LIGHT_BRIGHT_FULL ) ;
            break ;
        }
    }
}