// :: Definitions ::
/*
#define BUTTON_X 0x01			//Left-most on the front face (Labeled X on newer joysticks)
#define BUTTON_A 0x02			//Bottom-most on the front face (Labeled A on newer joysticks)
#define BUTTON_Y 0x04			//Top-most on the front face (Labeled Y on newer joysticks)
#define BUTTON_B 0x03			//Right-most on the front face (Labeled B on newer joysticks)
#define BUTTON_LB 0x05		//Top-left shoulder button  (Labeled LB on newer joysticks)
#define BUTTON_RB 0x06		//Top-right shoulder button  (Labeled RB on newer joysticks)
#define BUTTON_LT 0x07		//Bottom-left shoulder/trigger button  (Labeled LT on newer joysticks)
#define BUTTON_RT 0x08		//Bottom-right shoulder/trigger button (Labeled RT on newer joysticks)
#define BUTTON_BACK 0x09	//Back button
#define BUTTON_START 0x0A	//Start button
#define BUTTON_LS 0x0B		//Left analog thumbstick (when pressed in)
#define BUTTON_RS 0x0C		//Right analog thumbstick (when pressed in)
*/

// :: Includes ::
#include "JoystickDriver.c"

// :: Type Definitions ::
#define ButtonState_Inactive 0x00
#define ButtonState_Active 0x01
#define ButtonState_Pressed 0x02
#define ButtonState_Released 0x03

struct Controller_Buttons // ControllerButtons Structure
{
	byte
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
	Controller_Buttons Buttons;
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
void Controller_SetButtonState(byte &out, bool state)
{
	/*
	switch (out)
	{
	case Inactive:	{out = state ? Pressed : Inactive; }		break;
	case Active:		{out = state ? Active : Released;  } 		break;
	case Pressed:		{out = state ? Active : Released;  } 		break;
	case Released:	{out = state ? Active : Inactive;  } 		break;
	default:				{out = Inactive;         		   }
	}
	*/

if(out == ButtonState_Inactive) out = state ? ButtonState_Pressed : ButtonState_Inactive;
else if(out == ButtonState_Active) out = state ? ButtonState_Active : ButtonState_Released;
else if(out == ButtonState_Pressed) out = state ? ButtonState_Active : ButtonState_Released;
else if(out == ButtonState_Released) out = state ? ButtonState_Active : ButtonState_Inactive;
	else out = ButtonState_Inactive;
}

/*
* Set the current Up,Down,Left,Right DPad Button States based on an input set array
*/
void Controller_SetDPadState(Controller &controller, char* const input_state_set)
{
	Controller_SetButtonState(controller.Buttons.Dpad_Up, (const bool)input_state_set[0]);
	Controller_SetButtonState(controller.Buttons.Dpad_Down, (const bool)input_state_set[1]);
	Controller_SetButtonState(controller.Buttons.Dpad_Left, (const bool)input_state_set[2]);
	Controller_SetButtonState(controller.Buttons.Dpad_Right, (const bool)input_state_set[3]);
}

// Applies a threshold to a decimal value and returns a scaled ratio
float Controller_ApplyThreshold(float value)
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

/*
* Updates a controller object
*/
void Update_Controller(Controller &controller)
{
	// Update the Joystick(s)
	getJoystickSettings(joystick);


	const char BUTTON_X = 0x01;			//Left-most on the front face (Labeled X on newer joysticks)
	const char BUTTON_A = 0x02;			//Bottom-most on the front face (Labeled A on newer joysticks)
	const char BUTTON_Y = 0x04;			//Top-most on the front face (Labeled Y on newer joysticks)
	const char BUTTON_B = 0x03;			//Right-most on the front face (Labeled B on newer joysticks)
	const char BUTTON_LB = 0x05;		//Top-left shoulder button  (Labeled LB on newer joysticks)
	const char BUTTON_RB = 0x06;		//Top-right shoulder button  (Labeled RB on newer joysticks)
	const char BUTTON_LT = 0x07;		//Bottom-left shoulder/trigger button  (Labeled LT on newer joysticks)
	const char BUTTON_RT = 0x08;		//Bottom-right shoulder/trigger button (Labeled RT on newer joysticks)
	const char BUTTON_BACK = 0x09;	//Back button
	const char BUTTON_START = 0x0A;	//Start button
	const char BUTTON_LS = 0x0B;		//Left analog thumbstick (when pressed in)
	const char BUTTON_RS = 0x0C;		//Right analog thumbstick (when pressed in)

	// Update Controller Buttons
	Controller_SetButtonState(controller.Buttons.A, joy1Btn(BUTTON_A));
	Controller_SetButtonState(controller.Buttons.B, joy1Btn(BUTTON_B));
	Controller_SetButtonState(controller.Buttons.X, joy1Btn(BUTTON_X));
	Controller_SetButtonState(controller.Buttons.Y, joy1Btn(BUTTON_Y));
	Controller_SetButtonState(controller.Buttons.Start, joy1Btn(BUTTON_START));
	Controller_SetButtonState(controller.Buttons.Back, joy1Btn(BUTTON_BACK));
	Controller_SetButtonState(controller.Buttons.LB, joy1Btn(BUTTON_LB));
	Controller_SetButtonState(controller.Buttons.LeftStick, joy1Btn(BUTTON_LS));
	Controller_SetButtonState(controller.Buttons.RB, joy1Btn(BUTTON_RB));
	Controller_SetButtonState(controller.Buttons.RightStick, joy1Btn(BUTTON_RS));
	Controller_SetButtonState(controller.Buttons.LT, joy1Btn(BUTTON_LT));
	Controller_SetButtonState(controller.Buttons.RT, joy1Btn(BUTTON_RT));


	// Update the Joystick
	// Use a square threshold because of the LS-X-A-RS-Y-A racing layout
	if(controller.ControllerID == 0)
	{
		controller.LeftStick.x = Controller_ApplyThreshold((float)joystick.joy1_x1 / 128.0);
		controller.LeftStick.y = Controller_ApplyThreshold((float)joystick.joy1_y1 / 128.0);
		controller.RightStick.x = Controller_ApplyThreshold((float)joystick.joy1_x2 / 128.0);
		controller.RightStick.y = Controller_ApplyThreshold((float)joystick.joy1_y2 / 128.0);
	} else if (controller.ControllerID == 1)
	{
		controller.LeftStick.x = Controller_ApplyThreshold((float)joystick.joy2_x1 / 128.0);
		controller.LeftStick.y = Controller_ApplyThreshold((float)joystick.joy2_y1 / 128.0);
		controller.RightStick.x = Controller_ApplyThreshold((float)joystick.joy2_x2 / 128.0);
		controller.RightStick.y = Controller_ApplyThreshold((float)joystick.joy2_y2 / 128.0);
	}

	// DPad Button States
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

	// Update Controller D-Pad
	controller.Buttons.Dpad_State = joystick.joy1_TopHat;
	//Controller_SetDPadState(controller, dpad_states[controller.Buttons.Dpad_State]);
}
