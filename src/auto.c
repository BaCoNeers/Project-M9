#include "hitechnic-irseeker-v2.h"

typedef enum
{
	A1,A2,A3,A4,A5,A6,A7,A8,A9,A10,
	B1,B2,B3,B4,B5,T,Mode_Num_Modes
} Auto_Mode;

char* mode_name[] = {"A1","A2","A3","A4","A5","A6","A7","A8","A9","A10","B1","B2","B3","B4","B5","T"};


void _drive(float time, int speed)
{
	motor[Motor_Drive_Left] = -speed;
	motor[Motor_Drive_Right] = speed;

	sleep(time);

	motor[Motor_Drive_Left] = 0;
	motor[Motor_Drive_Right] = 0;
}

void _turn(float time, int leftSpeed, int rightSpeed)
{
	motor[Motor_Drive_Left] = -leftSpeed;
	motor[Motor_Drive_Right] = rightSpeed;

	sleep(time);

	motor[Motor_Drive_Left] = 0;
	motor[Motor_Drive_Right] = 0;
}

void _liftControl(float time, int speed)
{
	motor[Motor_Arm_A] = speed;
	motor[Motor_Arm_B] = speed;
	motor[Motor_Arm_C] = -speed;
	sleep(time);
	motor[Motor_Arm_A] = 0;
	motor[Motor_Arm_B] = 0;
	motor[Motor_Arm_C] = 0;
	sleep(100);
}

void _goalAttachControl(bool attach)
{
servo[Servo_GoalAttach] = (attach) ? Servo_GoalAttach_Attach : Servo_GoalAttach_Dettach;
}

void _bucketControl(bool downIsFalse_UpIsTrue)
{
servo[Servo_Bucket] = (downIsFalse_UpIsTrue) ? 120 : 10;
}

int _IRTrack(tSensors link)
{
	tHTIRS2DSPMode mode = DSP_1200;

	if (HTIRS2setDSPMode(link, mode) == 0)
	{
		return -1;
	}

	return HTIRS2readACDir(link); // return direction (sector) of strongest detected IR signal
}


/*
*  Autonomous Ramp Start
*
*   Off the ramp
*/
void auto_a1()
{
	_drive(2800, -30); // Drive in reverse off the ramp
}

/*
*  Autonomous Ramp Start
*
*   Off the ramp
*   Dispense 2 balls in 60 cm goal (starting balls)
*/
void auto_a2()
{
	auto_a1(); // Off the ramp

	// Drive to 60cm goal

	_goalAttachControl(true); // Attach the 60 cm goal
	// Dispense starting balls
}

/*
*  Autonomous Ramp Start
*
*   Off the ramp
*   Dispense 2 balls in 60 cm goal (starting balls)
*   Move to parking zone
*/
void auto_a3()
{
	auto_a2(); // Off ramp - dispense starting balls in 60cm goal

	/*_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to right
	_drive(2500, -30); //drive forward 60cm again not measured yet
	_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to left
	_drive(2500, -30); //drive forward 210cm again not measured yet
	_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to right
	_drive(2500, -30); //drive forward 100cm again not measured yet*/
}

/*
*  Autonomous Ramp Start
*
*   Off the ramp
*   Release kickstand
*/
void auto_a4()
{
	auto_a1(); // Off ramp

	_turn(525, -60, 60)
	// Turns left 90* facing front to centre of field
	sleep(250);
	_drive(500, 100);
	_drive(1500, -100);
}

/*
*  Autonomous Ramp Start
*
*   Off the ramp
*   Score 2 balls in 60 cm goal (starting balls)
*   Release kickstand
*/
void auto_a5() // off ramp - score 2 balls in 60cm goal - release kickstand
{
	auto_a1(); // Off the ramp

	// drive to 60cm rolling goal

	_goalAttachControl(true); // Attach to goal
	// stack up to 60cm
	// dispense
	//release stick IR
}

/*
*  Autonomous Ramp Start
*
*   Off the ramp
*   Score 2 balls in 60 cm goal (starting balls)
*   Release kickstand
*		Move goal to parking zone
*/
void auto_a6()
{
	auto_a5();

	// drive to parking zone
}

/*
*  Autonomous Ramp Start
*
*/
void auto_a7()
{
	_drive(2500, -30); // this is to drive off backwards the ramp and to the goal but it has not been measured yet
	_goalAttachControl(true);
	// stack up to 60cm
	// dispense
	//release stick IR
	//collect balls in stack
}

void auto_b5()
{
	_turn(1250, -48, -56);
	sleep(500);
	if (_IRTrack(Sensor_IR) == 5)
	{
		playImmediateTone(400,400);
		_goalAttachControl(false);
		_turn(850, -48, -56);
		sleep(500);
		_turn(180, 48, 56);
		_liftControl(3150, 100);
		sleep(250);
		_bucketControl(true);
		sleep(2000);
		_bucketControl(false);
		sleep(750);
		_bucketControl(true);
		sleep(2000);
		_bucketControl(false);
		_liftControl(3000, -100);
		return;
	}

	_turn(627, 50, -50);
	// _turn(678, -50, 50); perfect 90 to the left

	/*_turn(700, -47, 54);
	sleep(500);

	_turn(678, -50, 50);
	sleep(500);
	_turn(1500, -47, 54);
	sleep(500);
	_turn(1017, 50, -50);*/
}

int previous_state = 0;

Auto_Mode auto_select()
{
	Auto_Mode r;
	while (true)
	{
		eraseDisplay();
		nxtDisplayCenteredTextLine(1, "(L/R arrows to select):");
		if(r<0)r=Mode_Num_Modes-1;
		r = r % Mode_Num_Modes;

		nxtDisplayCenteredBigTextLine(3, mode_name[r]);

		if(nNxtButtonPressed == 1 && nNxtButtonPressed != previous_state) r++;
		if(nNxtButtonPressed == 2 && nNxtButtonPressed != previous_state) r--;
		if(nNxtButtonPressed == 3 && nNxtButtonPressed != previous_state) return r;

		playImmediateTone(50,20);

		// Store State
		previous_state = nNxtButtonPressed;

		sleep(50);
	}
}

void auto(Auto_Mode mode)
{
	auto_b5();
	return;

	if(mode == A1) auto_a1();
	else if(mode == A2) auto_a2();
	else if(mode == A3) auto_a3();
	else if(mode == A4) auto_a4();
	else if(mode == A5) auto_a5();
	else if(mode == A6) auto_a6();
	else if(mode == A7) auto_a7();
	else if(mode == A8) auto_a8();
	else if(mode == A9) auto_a9();
	else if(mode == B1) auto_b1();
	else if(mode == B2) auto_b2();
	else if(mode == B3) auto_b3();
	else if(mode == B4) auto_b4();
	else if(mode == B5) auto_b5();
	else if(mode == T) auto_t();
}
