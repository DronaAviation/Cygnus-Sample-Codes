/* Do not remove the include below */
#include "PlutoPilot.h"
#include "Control.h"
#include "Sensor.h"
#include "User.h"
#include "Utils.h"
#include "Math.h"

int16_t initPressure;
int16_t currPressure;

/* The setup function is called once at Pluto's hardware startup */
void plutoInit() 
{
    /* Add your hardware initialization code here */
}

/* The function is called once before plutoLoop when you activate Developer Mode */
void onLoopStart() 
{
    /* Do your one time tasks here */
    initPressure = Barometer.get(PRESSURE); /* Get Initial Pressure when you turn the developer mode */
    LED.flightStatus(DEACTIVATE); /* Disable the default LED behavior */
}

/* The loop function is called in an endless loop */
void plutoLoop() 
{
    /* Add your repeated code here */
    currPressure = Barometer.get(PRESSURE); /* Get current pressure continuously */
    
    /* Calculate pressure difference if the value difference between the current pressure 
       and initial pressure is greater than 8 */
    if (fabs(currPressure - initPressure) > 8) 
    {
        LED.set(RED, ON); /* Turn on LED to indicate if condition is true */
        Command.arm(); /* Arm the drone */
        DesiredPosition.set(Z, 100); /* Set the drone altitude to 100 cms */
    }
    
    initPressure = ((initPressure * 0.9) + (currPressure * 0.1));
    
    Monitor.println("oldPressure", initPressure);
    Monitor.println("newPressure", currPressure);
    Graph.red(currPressure);
}

/* The function is called once after plutoLoop when you deactivate Developer Mode */
void onLoopFinish() 
{
    /* Do your cleanup tasks here */
    LED.flightStatus(ACTIVATE); /* Enable the default LED behavior */
}
