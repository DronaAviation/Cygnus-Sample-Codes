/* Do not remove the include below */
#include "PlutoPilot.h"
#include "Control.h"
#include "Estimate.h"
#include "Motor.h"
#include "Utils.h"
#include "User.h"

int16_t angle;

#define ABS(x) ((x) > 0 ? (x) : -(x))

/* The setup function is called once at Pluto's hardware startup */
void plutoInit() 
{
    /* Add your hardware initialization code here */
    Motor.initReversibleMotors();
}

/* The function is called once before plutoLoop when you activate Developer Mode */
void onLoopStart() 
{
    /* Do your one-time tasks here */
    LED.flightStatus(DEACTIVATE); /* Disable default LED behavior */
    
    /* Reverse the motor direction */
    Motor.setDirection(M1, ANTICLOCK_WISE);
    Motor.setDirection(M2, CLOCK_WISE);
    Motor.setDirection(M3, ANTICLOCK_WISE);
    Motor.setDirection(M4, CLOCK_WISE);
}

/* The loop function is called in an endless loop */
void plutoLoop() 
{
    /* Add your repeated code here */
    angle = Angle.get(AG_ROLL); 
    Monitor.println("Angle: ", angle); /* Read current angle value */
    
    if (!FlightStatus.check(FS_ARMED)) 
    {
        /* Check if drone is armed */
        if (ABS(angle) > 800) /* Checks if the drone is inverted */
        {
            /* Set the motor input to max */
            Motor.set(M1, 2000);
            Motor.set(M2, 2000);
        } 
        else 
        {
            Motor.set(M1, 1000);
            Motor.set(M2, 1000);
            Motor.set(M3, 1000);
            Motor.set(M4, 1000);
        }
    }
}

/* The function is called once after plutoLoop when you deactivate Developer Mode */
void onLoopFinish() 
{
    /* Do your cleanup tasks here */
    LED.flightStatus(ACTIVATE); /* Enable the default LED behavior */
    
    /* Set motor value to default */
    Motor.set(M1, 1000);
    Motor.set(M2, 1000);
    Motor.set(M3, 1000);
    Motor.set(M4, 1000);
    
    /* Set motor directions to default */
    Motor.setDirection(M1, CLOCK_WISE);
    Motor.setDirection(M2, ANTICLOCK_WISE);
    Motor.setDirection(M3, CLOCK_WISE);
    Motor.setDirection(M4, ANTICLOCK_WISE);
}
