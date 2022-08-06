#include "arduino.h"

#define PUMP_ONE 5
#define PUMP_TWO 6
#define SWITCH_ONE 7
#define SWITCH_TWO 8
#define LED 13

#define PUMP_ON_TIME 30
#define PUMP_OFF_TIME 5 // off time divided by on time

int switch_1_reading;
int switch_2_reading;
int previous_state;
int current_state;

unsigned long timer_on;
unsigned long timer_off;
unsigned long pump_on_time;
unsigned long pump_off_time;
unsigned long current_time;

void setup()
{ // THIS RUNS ONCE AT STARTUP
    Serial.begin(115200);
    Serial.println("Serial connection established.");

    // set pin mode of devices connected to MCU
    pinMode(PUMP_ONE, OUTPUT);
    pinMode(PUMP_TWO, OUTPUT);
    pinMode(SWITCH_ONE, INPUT);
    pinMode(SWITCH_TWO, INPUT);
    // set pump pins to 0 for clean start using PWM
    analogWrite(PUMP_ONE, 0);
    analogWrite(PUMP_TWO, 0);
    // set switch pins to 0 for clean start
    digitalWrite(SWITCH_ONE, LOW);
    digitalWrite(SWITCH_TWO, LOW);

    pump_on_time = millisToMinutes(PUMP_ON_TIME);
    pump_off_time = millisToMinutes(PUMP_ON_TIME * PUMP_OFF_TIME);
    Serial.print("pump on time: ");
    Serial.println(pump_on_time);
    Serial.print("pump off time: ");
    Serial.println(pump_off_time);

} // END OF SETUP

void loop()
{ // THIS LOOPS INFINITELY AFTER STARTUP

    // CHECK SWITCH

    // IF SWITCH IS IN STATE 0 (OFF)
    // STOP PUMP, AND WAIT FOR STATE TO CHANGE
    if (switch_1_reading == 0 && switch_2_reading == 0) // if both switches are off
    {
        digitalWrite(LED,HIGH);
        analogWrite(PUMP_ONE, 0); // stop pump one
        analogWrite(PUMP_TWO, 0); // stop pump two
        // update state
        previous_state = current_state; // set previous state to current state before changing current state to new state.
        current_state = 0;              // set current state to off (0)
        printCurrentState();            // prints current state to serial console
    }

    // IF SWITCH IS IN STATE 1 (ON)
    // RUN PUMP AT MAXIMUM POWER UNTIL STATE CHANGES
    if (switch_1_reading == 1) // if switch 1 is on
    {
        digitalWrite(LED,LOW);
        analogWrite(PUMP_ONE, 255); // run pump one
        analogWrite(PUMP_TWO, 255); // run pump two
        // update state
        previous_state = current_state; // set previous state to current state before changing current state to new state.
        current_state = 1;              // set current state to off (0)
        printCurrentState();            // prints current state to serial console
    }

    // IF SWITCH IS IN STATE 2 ()
    // RUN THE PUMP FOR X/minutes EVERY X/hours
    if (switch_2_reading == 2) // if switch 2 is on
    {
        if (millis() >= timer_on + pump_on_time)
        {
            timer_on += pump_on_time;
            digitalWrite(LED,LOW);
            // pump on
            // pump on
            print_millis(timer_on);

            if (millis() >= timer_off + pump_off_time)
            {
                
                timer_off += pump_off_time;
                digitalWrite(LED,HIGH);
                // pump off
                // pump off
                print_millis(timer_off);
            }
        }

        // update state
        previous_state = current_state; // set previous state to current state before changing current state to new state.
        current_state = 2;              // set current state to off (0)
        printCurrentState();            // prints current state to serial console
    }

} // END OF LOOP

//print millis to serial console
void print_millis(unsigned long time_millis){
    Serial.print("Time: ");
    Serial.print(time_millis/1000);
    Serial.print("s - ");
}

// prints current state to serial console
void printCurrentState()
{
    Serial.print("Current State: ");
    Serial.println(current_state);
}

// convert milliseconds to minutes (60000 millis in a minute)
float millisToMinutes(unsigned long x)
{
    return x * 60000;
}