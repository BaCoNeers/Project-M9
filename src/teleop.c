#include "Controller.c"

// :: Variables ::
Controller ControllerA;
Controller ControllerB;
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
signed char Map(ButtonState &bA, ButtonState &bB, signed char iA, signed char iB, signed char def)
{
	return bA == Active ? iA : (bB == Active ? iB : 0);
}

/*
* Updates Teleop
*/
void Update_Teleop()
{
	// Update Controllers
	UpdateController(ControllerA);
	UpdateController(ControllerB);

	// : Calculate Motor Speed

	// Clamped squared interpolation for the drive motors
	float rotation = -ControllerA.RightStick.x;
	if(rotation*rotation < 0.0025) rotation = 0;
	else rotation *= (rotation<0) ? -rotation : rotation; // Same sign, Squared Value

	// Each wheel shares a +- Speed Value depending on the LeftStick Y Axis
	// The value is manipulated by the +- Linear Interpolated Value of the Rotation
	float speed = ControllerA.LeftStick.y;
	float leftmotorspeed = lerp(speed,-speed, rotation);
	float rightmotorspeed = -lerp(speed,-speed, rotation);

	// :: Drive State Controller Mapping ::
	// ** Calculate the lerp between the positive position of the xaxis lerp(-1,1, (controller#.#Stick.x+1.0)/2.0)
	/*
	float wheeldistance = 0.18; // 18cm
	Angle to center of mass = 45 degrees, sin45 = 0.7
	Back wheel distance from center of mass = sqrt(18cm*18cm + 20cm*20cm) = 0.27cm
	Tangent wheel distance from center of mass = 20cm
	Lerp ratio = 2*0.27 / 0.7 = 0.77

	float rotation = controllerA.leftStick.x*controllerA.leftStick.x > 0.1 ?
	lerp(0,1,controllerA.leftStick.x)
	: 0;
	*/
	// Unused
	//float leftbackmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, rotation*0.77);
	//float rightbackmotorspeed = -lerp(controllerA.leftStick.y, -controllerA.leftStick.y, -rotation*0.77);


	// : Mapping of Servos and Motors to Sticks and Buttons :
	motor[Motor_Drive_Left] = MotorSpeed * leftmotorspeed;
	motor[Motor_Drive_Right] = -MotorSpeed * rightmotorspeed;

	motor[Motor_Drive_Left] = Map(ControllerA.Buttons.LB, ControllerA.Buttons.LT, 100, 100, 0);
	motor[Motor_Drive_Right] = Map(ControllerA.Buttons.RB, ControllerA.Buttons.RT, -100, -100, 0);

	if (ControllerA.Buttons.LeftStick == Active) playImmediateTone(500,10); // Input Test aka Horn
}

/*
* Resets the drive code ready to shut down the robot
*/
void End_Teleop()
{
	Init_Teleop(); // Reset Motors
}
