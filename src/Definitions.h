// :: Functions ::
#define COUT writeDebugStream
#define COUTL writeDebugStreamLine

// :: Motors ::
#define Motor_Drive_Left motorD
#define Motor_Drive_Right motorE

#define Motor_Harvester motorA

// :: Servos ::




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
