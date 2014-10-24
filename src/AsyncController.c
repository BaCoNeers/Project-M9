#include "Math.c"
#include "JoystickDriver.c"

// :: Definitions ::
#define BUTTON_X 1		//Left-most on the front face (Labeled X on newer joysticks)
#define BUTTON_A 2		//Bottom-most on the front face (Labeled A on newer joysticks)
#define BUTTON_Y 4		//Top-most on the front face (Labeled Y on newer joysticks)
#define BUTTON_B 3		//Right-most on the front face (Labeled B on newer joysticks)
#define BUTTON_LB 5		//Top-left shoulder button  (Labeled LB on newer joysticks)
#define BUTTON_RB 6		//Top-right shoulder button  (Labeled RB on newer joysticks)
#define BUTTON_LT 7		//Bottom-left shoulder/trigger button  (Labeled LT on newer joysticks)
#define BUTTON_RT 8		//Bottom-right shoulder/trigger button (Labeled RT on newer joysticks)
#define BUTTON_BACK 9	//Back button
#define BUTTON_START 10	//Start button
#define BUTTON_LS 11	//Left analog thumbstick (when pressed in)
#define BUTTON_RS 12	//Right analog thumbstick (when pressed in)

// :: Type Definitions ::
typedef enum
{
	Active,
	Inactive,
	Pressed,
	Released
} ButtonState; // ButtonState Structure

typedef struct
{
	ButtonState
	A, B, X, Y, LB, RB, LT, RT,
	Start, Back,
	LeftStick, RightStick,
	Dpad_Up, Dpad_Down, Dpad_Left, Dpad_Right;
	int Dpad_State;
} ControllerButtons; // ControllerButtons Structure

typedef struct
{
	// Vector Variables
	Vector2 leftStick, rightStick;
	ControllerButtons m_Buttons;
	int m_ID;
} Controller; // Controller Structure

// :: Variables ::
// Controller Threshold
const float JoystickThreshold = 0.15;
const float JoystickRatio = 1.0 - JoystickThreshold;

Controller controllerA;
Controller controllerB;

// :: Methods ::

// Set the state of a button given a button location and a boolean value.
void SetButtonState(ButtonState &out, bool state)
{
	switch (out)
	{
	case Inactive:	{out = state ? Pressed : Inactive; }
		break;
	case Active:	{out = state ? Active : Released;  }
		break;
	case Pressed:	{out = state ? Active : Released;  }
		break;
	case Released:	{out = state ? Active : Inactive;  }
		break;
	default:		{out = Inactive;         		   }
	}
}

// Create a function for setting the state of the DPad from a boolean array.
void SetDPadState(Controller &controller, char* state)
{
	SetButtonState(controller.m_Buttons.Dpad_Up, (bool)state[0]);
	SetButtonState(controller.m_Buttons.Dpad_Down, (bool)state[1]);
	SetButtonState(controller.m_Buttons.Dpad_Left, (bool)state[2]);
	SetButtonState(controller.m_Buttons.Dpad_Right, (bool)state[3]);
}

// Applies a threshold to a decimal value and returns a scaled ratio
float ApplyThreshold(float value)
{
	if (value > JoystickThreshold)
	{
		value -= JoystickThreshold;
		value /= JoystickRatio;
	}
	else if (value < -JoystickThreshold)
	{
		value += JoystickThreshold;
		value /= JoystickRatio;
	}
	else
	{
		value = 0.0;
	}
	return value;
}

char dpad_none[4] = { 0, 0, 0, 0 };
char dpad_0[4] = { 1, 0, 0, 0 };
char dpad_1[4] = { 0, 0, 0, 1 };
char dpad_2[4] = { 0, 1, 0, 0 };
char dpad_3[4] = { 0, 0, 1, 0 };
char dpad_4[4] = { 1, 0, 0, 1 };
char dpad_5[4] = { 0, 1, 0, 1 };
char dpad_6[4] = { 0, 1, 1, 0 };
char dpad_7[4] = { 1, 0, 1, 0 };

// Updates the global controller
void UpdateController(Controller &controller)
{
	// Update the Joystick
	// Use a square threshold because of the LS-X-A-RS-Y-A racing layout
	controller.leftStick.x = ApplyThreshold((float)joystick.joy1_x1 / 128.0);
	controller.leftStick.y = ApplyThreshold((float)joystick.joy1_y1 / 128.0);
	controller.rightStick.x = ApplyThreshold((float)joystick.joy1_x2 / 128.0);
	controller.rightStick.y = ApplyThreshold((float)joystick.joy1_y2 / 128.0);


	SetButtonState(controller.m_Buttons.A, joy1Btn(BUTTON_A));
	SetButtonState(controller.m_Buttons.B, joy1Btn(BUTTON_B));
	SetButtonState(controller.m_Buttons.X, joy1Btn(BUTTON_X));
	SetButtonState(controller.m_Buttons.Y, joy1Btn(BUTTON_Y));
	SetButtonState(controller.m_Buttons.Start, joy1Btn(BUTTON_START));
	SetButtonState(controller.m_Buttons.Back, joy1Btn(BUTTON_BACK));
	SetButtonState(controller.m_Buttons.LB, joy1Btn(BUTTON_LB));
	SetButtonState(controller.m_Buttons.LeftStick, joy1Btn(BUTTON_LS));
	SetButtonState(controller.m_Buttons.RB, joy1Btn(BUTTON_RB));
	SetButtonState(controller.m_Buttons.RightStick, joy1Btn(BUTTON_RS));
	SetButtonState(controller.m_Buttons.LT, joy1Btn(BUTTON_LT));
	SetButtonState(controller.m_Buttons.RT, joy1Btn(BUTTON_RT));

	controller.m_Buttons.Dpad_State = joystick.joy1_TopHat;

	// Reset the DPad
	switch (controller.m_Buttons.Dpad_State)
	{
	case 0:	{SetDPadState(controller, &dpad_0[0]);	} //SetButtonState(controller.m_Buttons.Dpad_Up,    TRUE);
		break;
	case 2:	{SetDPadState(controller, &dpad_1[0]);	} //SetButtonState(controller.m_Buttons.Dpad_Right, TRUE);
		break;
	case 4:	{SetDPadState(controller, &dpad_2[0]);  } //SetButtonState(controller.m_Buttons.Dpad_Down,  TRUE);
		break;
	case 6:	{SetDPadState(controller, &dpad_3[0]);	} //SetButtonState(controller.m_Buttons.Dpad_Left,  TRUE);
		break;
		/*	case 1:	{SetDPadState(&dpad_4[0]); } //SetButtonState(controller.m_Buttons.Dpad_Up,     TRUE); SetButtonState(controller.m_Buttons.Dpad_Right, TRUE);
		break;
		case 3:	{SetDPadState(&dpad_5[0]); } //SetButtonState(controller.m_Buttons.Dpad_Down,   TRUE); SetButtonState(controller.m_Buttons.Dpad_Right, TRUE);
		break;
		case 5:	{SetDPadState(&dpad_6[0]); } //SetButtonState(controller.m_Buttons.Dpad_Down,   TRUE); SetButtonState(controller.m_Buttons.Dpad_Left,  TRUE);
		break;
		case 7:	{SetDPadState(&dpad_7[0]); } //SetButtonState(controller.m_Buttons.Dpad_Up,     TRUE); SetButtonState(controller.m_Buttons.Dpad_Left,  TRUE);
		break;*/
	default: {SetDPadState(controller, &dpad_none[0]); }
		break;
	}
}

void Controller_Init()
{
	controllerA.m_ID = 1; // Assign controller A to the first controller
	controllerB.m_ID = 2; // Assign controller B to the second controller
}

void Controller_Update(float delta)
{
	// Update the Joystick(s)
	getJoystickSettings(joystick);
	// Update the Controller
	UpdateController(controllerA); // Update controller A
	UpdateController(controllerB); // Update controller B
}
