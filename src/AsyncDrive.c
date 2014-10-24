#include "AsyncController.c"
#include "Math.c"

int motorspeed = 100;
void Reset_Motors()
{
	/*
	intrinsic void memset(void *pToBuffer, const char nValue, const short nNumbOfBytes)
	asm(opcdSystemFunctions, byte(sysFuncMemSet),
	variableRefVoidPtr(pToBuffer),
	variable(nValue),
	variable(nNumbOfBytes));
	*/
	// Set the memory speed of all the motors to 0
	// TEST CODE
	memset(&motor[motorA], 0x00, sizeof(motor[motorA]) * 11); // Set the first 11 motors to 0 speed
}

/*
* Initialize the robot drive functionality
*/
void Drive_Init()
{
	Reset_Motors();
}

/*
* Resets the drive code ready to shut down the robot
*/
void Drive_End()
{
	Reset_Motors();
}

/*
* Returns the motor speed defined by specific button states
* Returns the value for either Pressed or Active button states
* bA (Button1), bB (Button2), iA (Motor Value1), iB (Motor Value2), def (Default Motor Value)
*/
signed char MapMotor(ButtonState &bA, ButtonState &bB, signed char iA, signed char iB, signed char def)
{
	return bA == Active ? iA : (bB == Active ? iB : 0);
}
float lastfeedchangetime = 0;
void Drive_Update(float delta)
{
	Controller_Update(delta); // Update the controller

	// :: Drive State Controller Mapping ::
	// ** Calculate the lerp between the positive position of the xaxis lerp(-1,1, (controllerA.leftStick.x+1.0)/2.0)

	//float wheeldistance = 0.18; // 18cm

	// Angle to center of mass = 45 degrees, sin45 = 0.7
	// Back wheel distance from center of mass = sqrt(18cm*18cm + 20cm*20cm) = 0.27cm
	// Tangent wheel distance from center of mass = 20cm
	// Lerp ratio = 2*0.27 / 0.7 = 0.77
	/*
	float rotation = controllerA.leftStick.x*controllerA.leftStick.x > 0.1 ?
	lerp(0,1,controllerA.leftStick.x)
	: 0;
	*/
	// Clamped squared interpolation for the drive motors
	float rotation = -controllerA.rightStick.x * 0.75;
	rotation *= (rotation<0) ? -rotation : rotation;

	// : Calculate Motor Speed
	float leftmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, rotation);
	float rightmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, -rotation);
	float leftbackmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, rotation*0.77);
	float rightbackmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, -rotation*0.77);

	motor[MotorLeft] = 100 * leftmotorspeed;
	motor[MotorRight] = -100 * rightmotorspeed;
	//motor[MotorLeft_Back] = 100 * leftbackmotorspeed;
	//motor[MotorRight_Back] = -100 * rightbackmotorspeed;

	// : Assign motor speed :
	//motor[motorA] = motorspeed * leftmotorspeed;
	motor[motorA] = MapMotor(controllerA.m_Buttons.LB, controllerA.m_Buttons.LT, 100, -100, 0);
	motor[motorB] = MapMotor(controllerA.m_Buttons.RB, controllerA.m_Buttons.RT, 100, -100, 0);

	//motor[MotorAux_RightArm] = MapMotor(controllerA.m_Buttons.Y, controllerA.m_Buttons.A, 30, -30, 0);
	//motor[MotorAux_LeftArm] = MapMotor(controllerA.m_Buttons.Back, controllerA.m_Buttons.Start, -30, 30, 0);
//	servo[Servo_Left] = MapMotor(controllerA.m_Buttons.A,controllerA.m_Buttons.Y, 40, 0, 0);
	///servo[Servo_Right] = MapMotor(controllerA.m_Buttons.X, controllerA.m_Buttons.B, 180-40, 180, 0);

	if (controllerA.m_Buttons.LeftStick == Active)
	{
		PlayImmediateTone(500,10);
	}
}
