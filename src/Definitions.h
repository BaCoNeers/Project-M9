// :: Functions ::
#define COUT writeDebugStream
#define COUTL writeDebugStreamLine

// :: Motors ::
#define Motor_Drive_Left motorD
#define Motor_Drive_Right motorE

// :: Servos ::


/*
* Returns the current time value of a timer
*/
float GetTime(TTimers timer)
{
	return (float)time1[timer] / 1000.0;
}
