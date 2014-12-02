// :: Functions ::
#define COUT writeDebugStream
#define COUTL writeDebugStreamLine

// :: Motors ::
#define Motor_Drive_Left motorF
#define Motor_Drive_Right motorI

#define Motor_Harvester motorA

#define Motor_Exhaust motorH

// :: Servos ::
#define Servo_Stack servo1

#define Servo_Arm_Left servo5
#define Servo_Arm_Right servo6

#define Servo_GoalKeeper servo3

/*
* Returns the current time value of a timer
*/
float GetTime(TTimers timer)
{
	return (float)time1[timer] / 1000.0;
}

/*
 * Enum for harvester status
*/
enum HarvesterStatusEnum {
    forward = 100,
    stopped = 0,
    reversed = -100,
};
