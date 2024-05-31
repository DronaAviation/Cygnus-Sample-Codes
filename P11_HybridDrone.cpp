/* Do not remove the include below */
#include "PlutoPilot.h"
#include "Control.h"
#include "User.h"
#include "Motor.h"
#include "Utils.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))

int16_t roll_value;
int16_t throttle_value;
int16_t m1Value;
int16_t m4Value;

int constrain(int amt, int low, int high);
int generatePWM(int amt);

/* The setup function is called once at Pluto's hardware startup */
void plutoInit() 
{
    /* Add your hardware initialization code here */
    Motor.init(M1); /* Initialize motor M1 */
    Motor.init(M4); /* Initialize Motor M4 */
}

/* The function is called once before plutoLoop when you activate Developer Mode */
void onLoopStart() 
{
    /* Do your one-time tasks here */
    roll_value = 0;
    throttle_value = 0;
    LED.flightStatus(DEACTIVATE); /* Disable the default LED behavior */
}

/* The loop function is called in an endless loop */
void plutoLoop() 
{
    /* Add your repeated code here */
    if (FlightStatus.check(FS_ARMED)) /* If drone is armed motors M1, M4 should be OFF */
    {
        LED.flightStatus(ACTIVATE); /* Enable the default LED behavior */
        Motor.set(M1, 1000);
        Motor.set(M4, 1000);
    } 
    else 
    {
        /* Get data from user */
        throttle_value = RcData.get(RC_THROTTLE);
        roll_value = RcData.get(RC_ROLL);
    }

    /* Condition for movement of wheels */
    m1Value = (throttle_value - 1500) + (roll_value - 1500) / 2;
    m1Value = constrain(m1Value, -500, 500);
    m4Value = (throttle_value - 1500) - (roll_value - 1500) / 2;
    m4Value = constrain(m4Value, -500, 500);

    if (m1Value > 0) 
    {
        Motor.setDirection(M1, FORWARD);
    } 
    else 
    {
        Motor.setDirection(M1, BACKWARD);
    }
    Motor.set(M1, generatePWM(m1Value));

    if (m4Value > 0) 
    {
        Motor.setDirection(M4, FORWARD);
    } 
    else 
    {
        Motor.setDirection(M4, BACKWARD);
    }
    Motor.set(M4, generatePWM(m4Value));
}

/* The function is called once after plutoLoop when you deactivate Developer Mode */
void onLoopFinish() 
{
    /* Do your cleanup tasks here */
    LED.flightStatus(ACTIVATE); /* Enable the default LED behavior */
    Motor.set(M1, 1000);
    Motor.set(M4, 1000);
}

int constrain(int amt, int low, int high) 
{
    if (amt > high) 
    {
        return high;
    } 
    else if (amt < low) 
    {
        return low;
    } 
    else 
    {
        return amt;
    }
}

int generatePWM(int amt) 
{
    amt = ABS(amt);
    amt = 1000 + (amt * 2);
    return amt;
}
