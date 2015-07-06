#include "controller.c"

// :: Variables ::
Controller ControllerA;
Controller ControllerB;

// :: Methods ::

void Init_Teleop()
{
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

	if (ControllerA.Buttons.Back == ButtonState_Pressed || ControllerB.Buttons.Back == ButtonState_Pressed)
		TeleopActive = !TeleopActive;

	if (TeleopActive)
	{
		// Harvester control
		if (ControllerA.Buttons.A == ButtonState_Pressed)
		{
			motor[Motor_Harvester] = (motor[Motor_Harvester] == -100) ? 0 : -100;
		}

		// Screwlift control
		if (ControllerA.Buttons.Y == ButtonState_Pressed)
		{
			if (motor[Motor_ScrewLift] == 100)
			{
				motor[Motor_ScrewLift] = 0;
			}
			else if (motor[Motor_ScrewLift] == 0)
			{
				motor[Motor_ScrewLift] = -100;
			}
			else if (motor[Motor_ScrewLift] == -100)
			{
				motor[Motor_ScrewLift] = 100;
			}
		}

		// Left arm control
		if (ControllerA.Buttons.LB == ButtonState_Pressed)
		{
			servo[Servo_Arm_Left] = (servo[Servo_Arm_Left] == 95) ? 225 : 95;
		}

		// Right arm control
		if (ControllerA.Buttons.RB == ButtonState_Pressed)
		{
			servo[Servo_Arm_Right] = (servo[Servo_Arm_Right] == 155) ? 15 : 155;
		}

		// Goal attach servo code
		if (ControllerA.Buttons.X == ButtonState_Pressed)
		  servo[Servo_GoalAttach] = (servo[Servo_GoalAttach] == Servo_GoalAttach_Dettach) ? Servo_GoalAttach_Attach : Servo_GoalAttach_Dettach;

		// Bucket servo control
		if (ControllerA.Buttons.B == ButtonState_Pressed)
		{
		  servo[Servo_Bucket] = (servo[Servo_Bucket] == 10) ? 120 : 10;
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

		motor[Motor_Arm_A] = -arm_speed;
		motor[Motor_Arm_B] = -arm_speed;
		motor[Motor_Arm_C] = arm_speed;

		// Clamped squared interpolation for the drive motors
		float rotation = -ControllerA.RightStick.x * 0.9;
		rotation *= (rotation<0) ? -rotation : rotation;

		float leftmotorspeed = -lerp(ControllerA.LeftStick.y * 0.8, -ControllerA.LeftStick.y * 0.8, rotation);
		float rightmotorspeed = lerp(ControllerA.LeftStick.y * 0.8, -ControllerA.LeftStick.y * 0.8, -rotation);

		if (ControllerA.Buttons.LT == ButtonState_Active)
		{
			motor[Motor_Hook] = 50;
		}
		else
		{
			motor[Motor_Hook] = -50;
		}

		if (ControllerA.Buttons.RT == ButtonState_Active)
		{
			motor[Motor_Drive_Left] = 20 * leftmotorspeed;
			motor[Motor_Drive_Right] = 20 * rightmotorspeed;
		} else {
			motor[Motor_Drive_Left] = 100 * leftmotorspeed;
			motor[Motor_Drive_Right] = 100 * rightmotorspeed;
		}
	}
	else
	{
		playImmediateTone(300, 10);
	}
}

/*
* Resets the drive code ready to shut down the robot
*/
void End_Teleop()
{
	Init_Teleop(); // Reset Motors
}
