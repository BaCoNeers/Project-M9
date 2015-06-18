// !!! DON'T CHANGE BELOW THIS LINE !!!

#include "hitechnic-irseeker-v2.h"

void _drive(float time, int speed)
{
	motor[Motor_Drive_Left] = -speed;
	motor[Motor_Drive_Right] = speed - 2;

	sleep(time);

	motor[Motor_Drive_Left] = 0;
	motor[Motor_Drive_Right] = 0;
}

void _turn(float time, int leftSpeed, int rightSpeed)
{
	motor[Motor_Drive_Left] = -leftSpeed;
	motor[Motor_Drive_Right] = rightSpeed - 2; // Fixes incorrect motor powers

	sleep(time);

	motor[Motor_Drive_Left] = 0;
	motor[Motor_Drive_Right] = 0;
}

void _goalAttachControl(bool attach)
{
	servo[Servo_GoalAttach] = (attach) ? Servo_GoalAttach_Attach : Servo_GoalAttach_Dettach;
}

void _bucketControl(bool doIWantItToGoUp)
{
	if (doIWantItToGoUp == true)
	{
		servo[Servo_Bucket] = Servo_Bucket_Up; //up
	}
	else
	{
		servo[Servo_Bucket] = Servo_Bucket_Down; //down
	}
}

int _IRTrack(tSensors link)
{
	tHTIRS2DSPMode mode = DSP_1200;

	if (HTIRS2setDSPMode(link, mode) == 0)
	{
		return -1;
	}

	return HTIRS2readACDir(link);
}

void _turnToIRBeacon(int dir)
{
	switch (dir)
	{
	  case 1: // -67.5 to -90
	    break;
	  case 2: // -45 to -67.5
	  	break;
	  case 3: // -22.5 to -45
	  	break;
	  case 4: // 0 to -22.5
	  	break;
	  case 5: // 0
	  	break;
	  case 6: // 0 to 22.5
	  	break;
	  case 7: // 22.5 to 45
	  	break;
	  case 8: // 45 to 67.5
	  	break;
	  case 9: // 90
	  	break;
	  default: // NO SIGNAL

	}
}


// i will comment my code
// haydn

// DISREGARD THE FIRST COMMENT OF THIS FILE HERE
// !!! DON'T CHANGE ABOVE THIS LINE !!!


void auto_a1() // label
{
	_drive(2500, -30); //drives off ramp backwards
}

void auto_a2() // label
{
	_drive(2500, -30); // this is to drive off backwards the ramp and to the goal but it has not been measured yet
	// goal attach down
	// stack up to 60cm
	_bucketControl(true); //lifts up bucket with balls already in it
}

void auto_a3()
{
	/*_drive(2500, -30); //supposed to drive off ramp backwards to the goal not measured yet
	// goal attach down
	//stack up to 60cm
	_bucketControl(true);
	//stack down
	_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to right
	_drive(2500, -30); //drive forward 60cm again not measured yet
	_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to left
	_drive(2500, -30); //drive forward 210cm again not measured yet
	_turn(time, leftSpeed, rightSpeed) //turn 90 degrees to right
	_drive(2500, -30); //drive forward 100cm again not measured yet*/
}

void auto_a4()
{
	auto_a1();

	_turn(525, -60, 60)
	// Turns left 90* facing front to centre of field					//IR required because it is stick mark watson
	sleep(250);
	_drive(500, 100);
	_drive(1500, -100);
}

void auto_a5()
{
	_drive(2500, -30); // this is to drive off backwards the ramp and to the goal but it has not been measured yet
	// goal attach down
	// stack up to 60cm
	_bucketControl(true); //lifts up bucket with balls already in it
	//release stick IR
}

void auto_a6()
{
	_drive(2500, -30); // this is to drive off backwards the ramp and to the goal but it has not been measured yet
	// goal attach down
	// stack up to 60cm
	_bucketControl(true); //lifts up bucket with balls already in it
	//release stick IR
}

void auto_a7()
{
	_drive(2500, -30); // this is to drive off backwards the ramp and to the goal but it has not been measured yet
	// goal attach down
	// stack up to 60cm
	_bucketControl(true); //lifts up bucket with balls already in it
	//release stick IR
	//collect balls in stack
}

void auto_t1()
{
	// start facing reverse inwards to field

	_goalAttachControl(true);

	_drive(1200, -30);
	sleep(350);

	int dir = _IRTrack(Sensor_IR);

	if (dir == 5) {
		playImmediateTone(400, 50);
	}

	// check seeker
	// if dir == 1 { in position 1 }

	motor[Motor_Arm_A] = motor[Motor_Arm_C] = -100;
	motor[Motor_Arm_B] = 100;

	return;
	_turn(690, -30, 30); // 90 right
	sleep(350); // give time to stop momentum
	_drive(800, -30);
	sleep(350);
	_turn(690, 30, -30); // 90 left
	// if dir == 1 { in position 2 }

	sleep(350);
	_turn(710, -30, 30); // 90 right
	sleep(350);
	_drive(900, -30);
	sleep(350);
	_turn(690, 30, -30); // 90 left
	sleep(350);
	_drive(1470, -30);
	sleep(350);
	_turn(690, 30, -30); // 90 left
}

void auto()
{
		auto_t1();
}
