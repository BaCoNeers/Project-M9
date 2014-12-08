#include "controller.c"

// :: Variables ::
Controller ControllerA;
Controller ControllerB;
HarvesterStatusEnum HarvesterStatus;
ExhaustStatusEnum ExhaustStatus;
int MotorSpeed = 100;

// :: Methods ::

void Init_Teleop()
{
	HarvesterStatus = HarvesterStatus_Stopped;
	ExhaustStatus = ExhaustStatus_Stopped;
	servo[Servo_Stack] = 255;

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
return bA == ButtonState_Active ? iA : (bB == ButtonState_Active ? iB : def);
}
int count = 0;
int speed = 5;
bool TeleopActive = true;
/*
* Updates Teleop
*/
void Update_Teleop()
{
	// Update Controllers
	Update_Controller(ControllerA);
	ControllerB.ControllerID = 1;
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

	if (ControllerA.Buttons.Back == ButtonState_Active && ControllerA.Buttons.Start == ButtonState_Pressed ||
		ControllerB.Buttons.Back == ButtonState_Active && ControllerB.Buttons.Start == ButtonState_Pressed) TeleopActive = !TeleopActive;

	if(TeleopActive)
	{
		/*// : Harvester code
		if (ControllerA.Buttons.A == ButtonState_Pressed || ControllerB.Buttons.A == ButtonState_Pressed)
		{
			if (HarvesterStatus == HarvesterStatus_Forward)HarvesterStatus = HarvesterStatus_Stopped;
			else if (HarvesterStatus == HarvesterStatus_Stopped)HarvesterStatus = HarvesterStatus_Reversed;
			else if (HarvesterStatus == HarvesterStatus_Reversed)HarvesterStatus = HarvesterStatus_Forward;
			motor[Motor_Harvester] = HarvesterStatus;
		}*/ // TEMP for fix on harvester
		// : Harvester code
		if (ControllerA.Buttons.A == ButtonState_Pressed || ControllerB.Buttons.A == ButtonState_Pressed)
		{
			if (HarvesterStatus == 100) HarvesterStatus = 0;
			else if (HarvesterStatus == 0) HarvesterStatus = -100;
			else if (HarvesterStatus == -100) HarvesterStatus = 100;
			motor[Motor_Harvester] = HarvesterStatus;
		}

		// : Exhaust Button Map :
		if (ControllerA.Buttons.Y == ButtonState_Pressed || ControllerB.Buttons.Y == ButtonState_Pressed)
		{
			if (ExhaustStatus == ExhaustStatus_Forward)ExhaustStatus = ExhaustStatus_Stopped;
			else if (ExhaustStatus == ExhaustStatus_Stopped)ExhaustStatus = ExhaustStatus_Reversed;
			else if (ExhaustStatus == ExhaustStatus_Reversed)ExhaustStatus = ExhaustStatus_Forward;
			motor[Motor_Exhaust] = ExhaustStatus;
		}

		/*
		* Left/right arm code. (servo)
		*/
		/*
		if (ControllerA.Buttons.LB == ButtonState_Pressed)
		{
		servo[Servo_Arm_Left] = (servo[Servo_Arm_Left] == Servo_Arm_Left_Down) ? Servo_Arm_Left_Up: Servo_Arm_Left_Down;
		}

		if (ControllerA.Buttons.RB == ButtonState_Pressed)
		{
		servo[Servo_Arm_Right] = (servo[Servo_Arm_Right] == Servo_Arm_Right_Down) ? Servo_Arm_Right_Up: Servo_Arm_Right_Down;
		}
		*/
		motor[Motor_Arm_Left] = Map(ControllerA.Buttons.LB, ControllerA.Buttons.LT, 100,-100,0);
		motor[Motor_Arm_Right] = Map(ControllerA.Buttons.RB, ControllerA.Buttons.RT, 100,-100,0);

		// Goal keeper servo code
		if (ControllerA.Buttons.X == ButtonState_Pressed || ControllerB.Buttons.X == ButtonState_Pressed)
		{
		servo[Servo_GoalKeeper] = (servo[Servo_GoalKeeper] == 230) ? 60: 230;
		}

		if (ControllerB.Buttons.B == ButtonState_Pressed)
		{
		servo[Servo_Stack] = (servo[Servo_Stack] == 255) ? 0: 255;
		}

		// Lift (vertical arm)
		int arm_speed = 0;
		arm_speed = -17 * ControllerB.LeftStick.y;
		if(ControllerB.LeftStick.y > 0) arm_speed *= 1.25;
		if (arm_speed * arm_speed < 9) arm_speed = 0;

		arm_speed = Map(
		ControllerB.Buttons.LT,
		ControllerB.Buttons.RT,
		40, // Down
		-40, // Up
		arm_speed
		);

		motor[Motor_Arm_A] = motor[Motor_Arm_C] = arm_speed;
		motor[Motor_Arm_B] = -arm_speed;


		// Clamped squared interpolation for the drive motors
		float rotation = -ControllerA.RightStick.x * 0.75;
	rotation *= (rotation<0) ? -rotation : rotation;

		// : Calculate Motor Speed
		float leftmotorspeed = -lerp(ControllerA.LeftStick.y, -ControllerA.LeftStick.y, rotation);
		float rightmotorspeed = lerp(ControllerA.LeftStick.y, -ControllerA.LeftStick.y, -rotation);

		// : Mapping of Servos and Motors to Sticks and Buttons :
		motor[Motor_Drive_Left] = MotorSpeed * leftmotorspeed;
		motor[Motor_Drive_Right] = MotorSpeed * rightmotorspeed;
	}
	else
	{
		playImmediateTone(300,10);
		motor[Motor_Drive_Left] = 0;
		motor[Motor_Drive_Right] = 0;
		motor[Motor_Arm_A] = motor[Motor_Arm_C] = motor[Motor_Arm_B] = 0;
		motor[Motor_Exhaust] =motor[Motor_Harvester] = 0;
	}
}

/*
* Resets the drive code ready to shut down the robot
*/
void End_Teleop()
{
	Init_Teleop(); // Reset Motors
}
