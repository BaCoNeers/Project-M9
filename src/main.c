// :: Definitions ::
#define DEBUG

// :: Includes ::
#include "definitions.h"
#include "math.c"

#ifdef TELE
#include "teleop.c"
#endif
#ifdef AUTO
#include "auto.c"
#endif

// :: Global Type Definitions ::
enum Mode
{
	Autonomous,
	Teleop,
	Finished
};

#pragma debuggerWindows("joystickGame");

// : Local Variables :
float TotalTime = 0.0;
#ifdef AUTO
Mode RobotMode = Autonomous;
#endif
#ifdef TELE
Mode RobotMode = Teleop;
#endif
bool Running = true;
int VolumeStore;

void default_ext_devices()
{
	// Defaults servos
	servo[Servo_Arm_Left] = Servo_Arm_Left_Down;
	servo[Servo_Arm_Right] = Servo_Arm_Right_Down;
	servo[Servo_GoalKeeper] = 210;

	if(RobotMode == Autonomous)
	{
		// Set motor exhaust to 1/4 speed (TEMP)
		motor[Motor_Exhaust] = -25;
	}
	else if(RobotMode == Teleop)
	{

		// Set motor exhaust to 1/4 speed (TEMP)
		motor[Motor_Exhaust] = 0;
	}
}

/*
* Initialize the Robot and ensure that the software is ready to operate.
*/
void Init()
{
	VolumeStore = nVolume; // Store the Initial Volume Setting
	nVolume = 4; // Maximum Volume
#ifdef AUTO
	if(RobotMode == Autonomous)
	{
		OffRamp(); // Add off ramp auto events

		servo[Servo_Arm_Left] = Servo_Arm_Left_Down;
		servo[Servo_Arm_Right] = Servo_Arm_Right_Down;
		servo[Servo_GoalKeeper] = 210;
		motor[Motor_Exhaust] = 0;

		// Run Startup Code
		waitForStart();
		default_ext_devices();

#ifdef DEBUG
		long init_time = time1(T1);
		writeDebugStreamLine("M9 Software Initialised. Wait time: %dms", init_time); // Debug the initialize time
#endif

		// Reset Time
#ifdef DEBUG
		clearTimer(T1);
#endif
		time1[T1] = 0;
		TotalTime = 0;
	}
#endif
#ifdef TELE
	if(RobotMode == Teleop)
	{
		ControllerA.ControllerID = 0;
		ControllerB.ControllerID = 1;
		default_ext_devices();
		Init_Teleop();
	}
#endif
}

/*
* Ends the current Robot Mode and ensures that the software is ready to operate
* in a new state.
*/
void EndState()
{
#ifdef AUTO
	if (RobotMode == Autonomous)
	{
		Auto_End(); // Update Autonomous Mode
		RobotMode = Teleop;
#ifdef DEBUG
		writeDebugStreamLine("Autonomous Mode End!"); // Debug
#endif
		Init();
	}
#endif
#ifdef TELE
	if (RobotMode == Teleop)
	{
		End_Teleop(); // Update Drive Mode
#ifdef DEBUG
		writeDebugStreamLine("Teleop Mode End!"); // Debug
#endif
		Running = false;
		RobotMode = Finished;
	}
#endif
	if (RobotMode == Autonomous)
	{
		nVolume = VolumeStore; // Restore the Initial Volume Setting
	}
}

/*
* Call the main update functions of all the inuse functionality
* Update the current mode that the robot is using. Autonomous/Drive
*/
void Update()
{
	alive(); // Reset sleep timer
	// Calculate Elapsed Time
	float delta = GetTime(T1);
	TotalTime += delta; // Accumulate time
	time1[T1] = 0;

#ifdef AUTO
	if (RobotMode == Autonomous)
	{
		Auto_Update(delta);
		if (TotalTime >= 29.5)  // End autonomous after 30 seconds
		{
			playImmediateTone(300, 20);
			EndState();
#ifdef DEBUG
			writeDebugStreamLine("Autonomous Mode End!"); // Debug
#endif
		}
	}
#endif
#ifdef TELE
	if (RobotMode == Teleop)
	{
		Update_Teleop(); // Update Drive Mode
#ifdef DEBUG
		if(ControllerA.Buttons.Start == ButtonState_Pressed) EndState();
		if(ControllerB.Buttons.Start == ButtonState_Pressed) EndState();
#endif
	}
	if (ControllerA.Buttons.LeftStick == ButtonState_Active) playImmediateTone(400, 1); // Input Test aka Horn
	else if (ControllerB.Buttons.LeftStick == ButtonState_Active) playImmediateTone(6000, 1); // Input Test aka Horn

#endif
}

/*
* Application Entry Point
*/
task main
{
	Init();
	while (Running)
	{
		// Update
		Update();
		sleep(2);
	}
	EndState();
}
