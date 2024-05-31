/* Do not remove the include below */
#include "PlutoPilot.h"
#include "Control.h"
#include "Estimate.h"
#include "Motor.h"
#include "User.h"
#include "Utils.h"

#define ABS(x) ((x) > 0 ? (x) : -(x))

bool isAutoStabilized = false; /* To check if auto-stabilization is ON or OFF */
int16_t setHeading = 0; /* To store initial heading value */
int16_t heading_error = 0; /* To store error value */
int16_t k = 10; /* Constant of proportional */
int16_t M1_Value; /* Calculated input value for M1 */
int16_t M4_Value; /* Calculated input value for M4 */
int16_t M1_Valuef; /* Generated PWM for M1 */
int16_t M4_Valuef; /* Generated PWM for M4 */
int16_t Roll_value;
int16_t Throttle_value;

int constrain(int amt, int low, int high);
int generatePWM(int amt);

/* The setup function is called once at Pluto's hardware startup */
void plutoInit() 
{
    /* Add your hardware initialization code here */
    Motor.init(M1); /* Initialize motor M1 */
    Motor.init(M4); /* Initialize motor M4 */
}

/* The function is called once before plutoLoop when you activate Developer Mode */
void onLoopStart() 
{
    /* Do your one-time tasks here */
    LED.flightStatus(DEACTIVATE); /* Disable the default LED behavior */
    Roll_value = 0;
    Throttle_value = 0;
}

/* The loop function is called in an endless loop */
void plutoLoop() 
{
    /* Add your repeated code here */
    if (!FlightStatus.check(FS_ARMED)) /* If drone is not armed */
    {
        if (RcData.get(RC_PITCH) >= 1930) /* Turn on Autostabilization */
        {
            isAutoStabilized = true;
            setHeading = Angle.get(AG_YAW); /* Take initial heading of the drone */
            LED.set(RED, OFF);
            LED.set(BLUE, ON);
            LED.set(GREEN, ON);
        } 
        else if (RcData.get(RC_PITCH) < 1070) /* Turn Autostabilization OFF */
        {
            isAutoStabilized = false;
            LED.set(RED, ON);
            LED.set(BLUE, OFF);
            LED.set(GREEN, OFF);
        }

        if (isAutoStabilized) /* If Autostabilized */
        {
            if (ABS(RcData.get(RC_ROLL) - 1500) < 30) /* If deviation is due to system error */
            {
                heading_error = setHeading - Angle.get(AG_YAW);
                /* Calculate error (original - current heading) */
                if (heading_error > 180)
                    heading_error = heading_error - 360;
                else if (heading_error < -180)
                    heading_error = heading_error + 360;
                Roll_value = 1500 + k * heading_error;
            } 
            else /* If deviation is due to user input */
            {
                Roll_value = RcData.get(RC_ROLL);
                setHeading = Angle.get(AG_YAW);
            }
        } 
        else /* If Auto-stabilization is OFF */
        {
            Roll_value = RcData.get(RC_ROLL);
        }

        Throttle_value = RcData.get(RC_THROTTLE);

        /* Condition for movement of wheels */
        M1_Value = (Throttle_value - 1500) + (Roll_value - 1500) / 2;
        M1_Value = constrain(M1_Value, -500, 500);

        M4_Value = (Throttle_value - 1500) - (Roll_value - 1500) / 2;
        M4_Value = constrain(M4_Value, -500, 500);

        if (M1_Value > 0) 
        {
            Motor.setDirection(M1, FORWARD);
        } 
        else 
        {
            Motor.setDirection(M1, BACKWARD);
        }

        if (M4_Value > 0) 
        {
            Motor.setDirection(M4, FORWARD);
        } 
        else 
        {
            Motor.setDirection(M4, BACKWARD);
        }

        M1_Valuef = generatePWM(M1_Value);
        M4_Valuef = generatePWM(M4_Value);
        Motor.set(M1, M1_Valuef);
        Motor.set(M4, M4_Valuef);

        Monitor.print("M1:", M1_Valuef);
        Monitor.println("M4:", M4_Valuef);
    } 
    else /* If drone is armed */
    {
        Motor.set(M1, 1000);
        Motor.set(M4, 1000);
    }
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
    if (amt < low)
        return low;
    else if (amt > high)
        return high;
    else
        return amt;
}

int generatePWM(int amt) 
{
    amt = ABS(amt);
    amt = 1000 + (amt * 2);
    return amt;
}
