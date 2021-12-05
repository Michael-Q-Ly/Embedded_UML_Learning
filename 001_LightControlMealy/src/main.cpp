/*
 *  Name: Michael Ly
 *  Date: December 5, 2021
 *  Description: Change LED brightness with PWM using a Mealy Machine.
 *  Target: Arduino Uno R3
 */

/*
 * enum in C/C++ is a user-defined data type
 * Use it to define your own data type
 * Provides clarity to your program
 * To use enum, enumerator-list must be known in advance
 * (a finite list set of named integer constant values a
 * data type can take on)
 */

#include <Arduino.h>

byte const LED = 9 ;

enum Day_of_week {
    SUNDAY ,
    MONDAY ,
    TUESDAY
} Day_of_week_t ;

 enum {
    RAINING = 30 ,
    CLOUDY ,
    SUNNY
} ;

typedef enum {  /* No Tag */
    MEETING ,
    CODING ,
    TRAVELLING ,
    READING ,
    EATING
} Activity_t ;  /* typedef alias name */

void setup() {
    enum Day_of_week today ;
    int climate ;
    Activity_t activity ;

    today = MONDAY ;
    climate = SUNNY ;
    activity = MEETING ;

    (void)( climate ) ;
    (void)( today ) ;
    (void)( activity ) ;
}

void loop() {
  // put your main code here, to run repeatedly:
}