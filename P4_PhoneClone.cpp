#include "PlutoPilot.h"
#include "Control.h" 
#include "User.h"
#include "Utils.h" 
#include "Estimate.h" 

int16_t Error=0, PlutoXHeading=0; 

void plutoInit() {
    // No spare hardware to be initialized
}

void onLoopStart() {
    LED.flightStatus(DEACTIVATE); /*disable the default LED behavior*/ 
    BiasError = App.getAppHeading() - Angle.get(AG_YAW); 
    Monitor.println("Error is: ", Error); 
}

void plutoLoop() {
    // Phone clone logic
    Monitor.println("PhoneHeading: ", App.getAppHeading()); 
    PlutoXHeading = App.getAppHeading() - BiasError; 
    if (PlutoXHeading < 0) { 
        PlutoXHeading+=360; 
    } 
    
    Monitor.println("Pluto X should turn to ", PlutoXHeading);   
    DesiredAngle.set(AG_YAW, PlutoXHeading); 
    Monitor.println("Pluto X is at: ", Angle.get(AG_YAW));  
    LED.set(RED, ON); 
    LED.set(GREEN, ON);   
}

void onLoopFinish() {
    // Cleanup tasks
    LED.flightStatus(ACTIVATE); /*Enable the default LED behavior*/ 
}
