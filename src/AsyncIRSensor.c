// :: Type Definitions ::
typedef enum
{
	IRS_No_Signal,
	IRS_Left,
	IRS_Right,
	IRS_Signal_Lock
} IRS; // IRState


typedef enum
{
	IRSD_Left,
	IRSD_Right
} IRSD;
typedef int byte;

// :: Variables ::
float IRAngle = 0; // Start at the middle of the rotation
byte IRDataState = -1; // The current value of the IRSensor
float IRUpdateAccumulator = 0;
const float IRUpdateTime = 10.0 / 1000.0;
const float RotationPerSecond = 3;

IRSD IRScanDirection = IRSD_Left; // Start the sensor assuming a left seek direction
IRS IRState = IRS_No_Signal; // Start the sensor assuming no signal state

/*
* Converts a euclidean angle of a tangent to a normal rotation as a byte format.
* Designed to be passed to a servo.
*/
byte AngleToServoRotation(float angle)
{
	float scale = 255.0 / 180.0; // Scale 180 degrees into a byte
	float offset = 90.0; // Add 90 degreed to create a normal to the robot
	byte rotation = (byte)((-angle + offset) * scale); // Offset the angle and convert it to a servo rotation
	return rotation;
}

/*
* Calculated the euclidean angle of a tangent based on a normal rotation as a byte format.
* Designed to be the stored normal format of the angle.
*/




/*
* Scan the IRSensor servo left to right
* This function is designed to be used when there is no known direction
* Takes approximately 1.8 seconds to achieve signal lock using single degrees.
*/
void IRScan()
{
	if (IRState == IRS_No_Signal)
	{
		if (IRScanDirection == IRSD_Left)
		{
			IRAngle += RotationPerSecond;
		}
		if (IRScanDirection == IRSD_Right || IRState == IRS_Left)
		{
			IRAngle -= RotationPerSecond;
		}

		if (IRScanDirection == IRSD_Right) { if (IRAngle < -90){ IRScanDirection = IRSD_Left; } };
		if (IRScanDirection == IRSD_Left) { if (IRAngle > 90){ IRScanDirection = IRSD_Right; } };
	}
	if (IRState == IRS_Left) // Sensor Detects a signal to the left
	{
		IRAngle += RotationPerSecond;
		IRScanDirection = IRSD_Left;
	}
	if (IRState == IRS_Right) // Sensor Detects a signal to the right
	{
		IRAngle -= RotationPerSecond;
		IRScanDirection = IRSD_Right;
	}

	// Clamp the values in order to not overflow the normal into a nevative value
	if (IRAngle > 90) IRAngle = 90;
	if (IRAngle < -90) IRAngle = -90;
}

/*
* Runs a sequence of functions and calculations in order to update the mechanical IRSensor
*/
void IR_Update(float delta, tSensors sensor, TServoIndex servoindex)
{
	IRUpdateAccumulator += delta; // Accumulate time
	if (IRUpdateAccumulator > IRUpdateTime)
	{

		IRUpdateAccumulator = 0; // Reset the IRUpdate Accumulator

		IRDataState = SensorValue[sensor];

		IRScan(); 																		// Run the IRSensor Scanning Code
		servo[servoindex] = AngleToServoRotation(IRAngle); // Rotate the servo to the current angle

		if (IRDataState == 0)
		{
			IRState = IRS_No_Signal; // The sensor does not detect a signal, run default scan code
		}
		else
		{
			if (IRDataState > 5) IRState = IRS_Left; // The sensor detects a signal to the left
			else
			if (IRDataState < 5) IRState = IRS_Right; // The sensor detects a signal to the right
			else
			if (IRDataState == 5) {
				IRState = IRS_Signal_Lock;
			}// The sensor is currently facing the IR Beacon
		}

		// At this point, the current angle to the normal will determine the sensor location provided there is a signal lock.
	}
}
