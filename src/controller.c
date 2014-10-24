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

// :: Includes ::
#include "drivers\JoystickDriver.h"

// :: Type Definitions ::
enum ButtonState // ButtonState Structure
{
	// True/False States
	Inactive=false,
	Active=true,
	// Aux States
	Pressed,
	Released
};

struct ControllerButtons // ControllerButtons Structure
{
	ButtonState
	A, B, X, Y, LB, RB, LT, RT,
	Start, Back,
	LeftStick, RightStick,
	Dpad_Up, Dpad_Down, Dpad_Left, Dpad_Right;
	int Dpad_State;
};

struct Controller // Controller Structure
{
	// Vector Variables
	vec2f LeftStick, RightStick;
	ControllerButtons Buttons;
	int ControllerID;
};

// :: Variables ::
// Controller Threshold
const float JoystickThreshold = 0.15;
const float JoystickRatio = 1.0 - JoystickThreshold;

// :: Methods ::
/*
* Set the state of a button given a button location and a boolean value.
*/
void SetButtonState(ButtonState &out, bool state)
{
	switch (out)
	{
	case Inactive:	{out = state ? Pressed : Inactive; }		break;
	case Active:		{out = state ? Active : Released;  } 		break;
	case Pressed:		{out = state ? Active : Released;  } 		break;
	case Released:	{out = state ? Active : Inactive;  } 		break;
	default:				{out = Inactive;         		   }
	}
}

/*
* Set the current Up,Down,Left,Right DPad Button States based on an input set array
*/
void SetDPadState(Controller &controller, char* const input_state_set)
{
	SetButtonState(controller.Buttons.Dpad_Up, (const bool)input_state_set[0]);
	SetButtonState(controller.Buttons.Dpad_Down, (const bool)input_state_set[1]);
	SetButtonState(controller.Buttons.Dpad_Left, (const bool)input_state_set[2]);
	SetButtonState(controller.Buttons.Dpad_Right, (const bool)input_state_set[3]);
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

const char dpad_states[9][4] =
{
	{ 1, 0, 0, 0 },
	{ 0, 0, 0, 1 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 1, 0, 0, 1 },
	{ 0, 1, 0, 1 },
	{ 0, 1, 1, 0 },
	{ 1, 0, 1, 0 },
	{ 0, 0, 0, 0 },
};

/*
* Updates a controller object
*/
void UpdateController(Controller &controller)
{
	// Update the Joystick(s)
	getJoystickSettings(joystick);

	if(controller.ControllerID == 0)
	{
		controller.LeftStick.x = ApplyThreshold((float)joystick.joy1_x1 / 128.0);
		controller.LeftStick.y = ApplyThreshold((float)joystick.joy1_y1 / 128.0);
		controller.RightStick.x = ApplyThreshold((float)joystick.joy1_x2 / 128.0);
		controller.RightStick.y = ApplyThreshold((float)joystick.joy1_y2 / 128.0);
	} else if (controller.ControllerID == 1)
	{
		controller.LeftStick.x = ApplyThreshold((float)joystick.joy2_x1 / 128.0);
		controller.LeftStick.y = ApplyThreshold((float)joystick.joy2_y1 / 128.0);
		controller.RightStick.x = ApplyThreshold((float)joystick.joy2_x2 / 128.0);
		controller.RightStick.y = ApplyThreshold((float)joystick.joy2_y2 / 128.0);
	}

	// Update the Joystick
	// Use a square threshold because of the LS-X-A-RS-Y-A racing layout
	controller.LeftStick.x = ApplyThreshold((float)joystick.joy1_x1 / 128.0);
	controller.LeftStick.y = ApplyThreshold((float)joystick.joy1_y1 / 128.0);
	controller.RightStick.x = ApplyThreshold((float)joystick.joy1_x2 / 128.0);
	controller.RightStick.y = ApplyThreshold((float)joystick.joy1_y2 / 128.0);

	// Update Controller Buttons
	SetButtonState(controller.Buttons.A, joy1Btn(BUTTON_A));
	SetButtonState(controller.Buttons.B, joy1Btn(BUTTON_B));
	SetButtonState(controller.Buttons.X, joy1Btn(BUTTON_X));
	SetButtonState(controller.Buttons.Y, joy1Btn(BUTTON_Y));
	SetButtonState(controller.Buttons.Start, joy1Btn(BUTTON_START));
	SetButtonState(controller.Buttons.Back, joy1Btn(BUTTON_BACK));
	SetButtonState(controller.Buttons.LB, joy1Btn(BUTTON_LB));
	SetButtonState(controller.Buttons.LeftStick, joy1Btn(BUTTON_LS));
	SetButtonState(controller.Buttons.RB, joy1Btn(BUTTON_RB));
	SetButtonState(controller.Buttons.RightStick, joy1Btn(BUTTON_RS));
	SetButtonState(controller.Buttons.LT, joy1Btn(BUTTON_LT));
	SetButtonState(controller.Buttons.RT, joy1Btn(BUTTON_RT));

	controller.Buttons.Dpad_State = joystick.joy1_TopHat;
	SetDPadState(controller, &dpad_states[controller.Buttons.Dpad_State][0]);
}
