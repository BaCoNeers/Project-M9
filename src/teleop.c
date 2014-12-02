#include "controller.c"

// :: Variables ::
Controller ControllerA;
Controller ControllerB;
HarvesterStatusEnum HarvesterStatus;
int MotorSpeed = 100;

// :: Methods ::

void Init_Teleop()
{
	// : Reset Motors :
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
* Returns the motor speed defined by specific button states
* Returns the value for either Pressed or Active button states
* bA (Button1), bB (Button2), iA (Motor Value1), iB (Motor Value2), def (Default Motor Value)
*/
signed char Map(byte &bA, byte &bB, signed char iA, signed char iB, signed char def)
{
return bA == ButtonState_Active ? iA : (bB == ButtonState_Active ? iB : 0);
}
int count = 0;
int speed = 5;
/*
* Updates Teleop
*/
void Update_Teleop()
{
	// Update Controllers
	Update_Controller(ControllerA);
	Update_Controller(ControllerB);

	// : Calculate Motor Speed
	/*
	// Clamped squared interpolation for the drive motors
	float rotation = -ControllerA.RightStick.x;
	if(rotation*rotation < 0.0025) rotation = 0;
	else rotation *= (rotation<0) ? -rotation : rotation; // Same sign, Squared Value

	// Each wheel shares a +- Speed Value depending on the LeftStick Y Axis
	// The value is manipulated by the +- Linear Interpolated Value of the Rotation
	float speed = ControllerA.RightStick.x;
	float leftmotorspeed = lerp(speed,-speed, rotation);
	float rightmotorspeed = -lerp(speed,-speed, rotation);
	*/

	// Clamped squared interpolation for the drive motors
	float rotation = -ControllerA.RightStick.x * 0.75;
rotation *= (rotation<0) ? -rotation : rotation;

	// : Calculate Motor Speed
	float leftmotorspeed = -lerp(ControllerA.LeftStick.y, -ControllerA.LeftStick.y, rotation);
	float rightmotorspeed = lerp(ControllerA.LeftStick.y, -ControllerA.LeftStick.y, -rotation);

  // : Harvester code
	if (ControllerA.Buttons.Y == ButtonState_Pressed)
	{
		if (HarvesterStatus == forward)
		{
			HarvesterStatus = stopped;
		}
		else if (HarvesterStatus == stopped)
		{
			HarvesterStatus = reversed;
		}
		else if (HarvesterStatus == reversed)
		{
			HarvesterStatus = forward;
		}
		motor[Motor_Harvester] = HarvesterStatus;
	}

	/*
	 * Left/right arm code. (servo)
	*/
	if (ControllerA.Buttons.LB == ButtonState_Pressed)
	{
		servo[Servo_Arm_Left] = (ServoValue[Servo_Arm_Left] == 30) ? 228: 30;
	}

	if (ControllerA.Buttons.RB == ButtonState_Pressed)
	{
		servo[Servo_Arm_Right] = (ServoValue[Servo_Arm_Right] == 30) ? 243: 30;
	}

	// Goal keeper servo code
	if (ControllerA.Buttons.X == ButtonState_Pressed)
	{
		servo[Servo_GoalKeeper] = (ServoValue[Servo_GoalKeeper] == 60) ? 100: 60;
	}

	/*
	//motor[motorI] = Map(ControllerA.Buttons.A, ControllerA.Buttons.B, speed, -speed, 0);
	int deltame = nMotorEncoder[motorI];
	nMotorEncoder[motorI] = 0;

	count += deltame;

	if(ControllerA.Buttons.A) count = 0;
	else
	if(count > -1440 * 1 * 0.9875)
	{
	motor[motorI] = -100;
	}
	else
	{
	motor[motorI] = 0;
	}*/


	//	playImmediateTone(100+count, 10);

	// :: Drive State Controller Mapping ::
	// ** Calculate the lerp between the positive position of the xaxis lerp(-1,1, (controller#.#Stick.x+1.0)/2.0)
	/*
	float wheeldistance = 0.18; // 18cm
	Angle to center of mass = 45 degrees, sin45 = 0.7
	Back wheel distance from center of mass = sqrt(18cm*18cm + 20cm*20cm) = 0.27cm
	Tangent wheel distance from center of mass = 20cm
	Lerp ratio = 2*0.27 / 0.7 = 0.77

	float rotation = ControllerA.leftStick.x*ControllerA.leftStick.x > 0.1 ?
	lerp(0,1,ControllerA.leftStick.x)
	: 0;
	*/
	// Unused
	//float leftbackmotorspeed = -lerp(ControllerA.leftStick.y, -ControllerA.leftStick.y, rotation*0.77);
	//float rightbackmotorspeed = -lerp(ControllerA.leftStick.y, -ControllerA.leftStick.y, -rotation*0.77);


	// : Mapping of Servos and Motors to Sticks and Buttons :
	motor[Motor_Drive_Left] = MotorSpeed * leftmotorspeed;
	motor[Motor_Drive_Right] = MotorSpeed * rightmotorspeed;

	//motor[Motor_Drive_Left] = Map(ControllerA.Buttons.LB, ControllerA.Buttons.LT, 100, 100, 0);
	//motor[Motor_Drive_Right] = Map(ControllerA.Buttons.RB, ControllerA.Buttons.RT, -100, -100, 0);
}

/*
* Resets the drive code ready to shut down the robot
*/
void End_Teleop()
{
	Init_Teleop(); // Reset Motors
}
