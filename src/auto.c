// :: Definitions ::
#define MAX_AUTO_EVENTS 100
#define MAX_EVENT_TIME 5.0
// Event location definitions
// float
#define EL_DRIVE_CURRENT_DISTANCE 2
#define EL_DRIVE_DISTANCE 0
#define EL_DRIVE_SPEED 1
#define EL_ROTATION_CURRENT_ANGLE 2
#define EL_ROTATION_ANGLE 0
#define EL_ROTATION_SPEED 1
#define EL_DISPENSE_HEIGHT 0
#define EL_ARM_ENUM 0

// int
#define EL_DRIVE_ENCODER_LEFT 1
#define EL_DRIVE_ENCODER_RIGHT 0


// :: Type Definitions ::
enum AutoModeEnum
{
	AutoModeEnum_RG1G2,
	AutoModeEnum_RGCG
};

enum AutoArmEvent
{
	AutoArmEvent_Left,
	AutoArmEvent_Right,
	AutoArmEvent_Both
};

enum AutoEventType
{
	AutoEventType_Drive,
	AutoEventType_Turn,
	AutoEventType_Dispense,
	AutoEventType_SeekIR,
	AutoEventType_PlotIR,
	AutoEventType_Arms
};

enum AutoDispenseLevel
{
	AutoDispenseLevel_Small = 30,
	AutoDispenseLevel_Medium = 60,
	AutoDispenseLevel_Large = 90,
	AutoDispenseLevel_ExtraLarge = 120
};

struct AutoEvent
{
	AutoEventType Type;
	int data_i[6];
	float data_f[6];
};

// :: Variables ::
AutoModeEnum Auto;
AutoEvent Events[MAX_AUTO_EVENTS];

int Num_Events = 0;
int Current_Event = 0;

int Total_Left_Encoder_Ticks = 0;
int Total_Right_Encoder_Ticks = 0;

float Left_Motor_Speed_Adjust = 1.0;
float Right_Motor_Speed_Adjust = 1.0;
float Current_Event_Time = 0.0;


// :: Functions ::

void Auto_Init()
{

}

void Auto_End()
{

}

void Auto_End_Event()
{
	Current_Event = 0;
	Total_Left_Encoder_Ticks = 0;
	Total_Right_Encoder_Ticks = 0;
	Current_Event_Time = 0.0;
}

// Autonomous update function
void Auto_Update(float delta)
{
	if(Current_Event < Num_Events)
	{
		AutoEvent* current_event = &Events[Current_Event];

		if (current_event->Type == AutoEventType_Drive)
		{
			int speed = 100 * current_event->data_f[EL_DRIVE_SPEED];
			int left_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Left];
			int right_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Right];
			float delta_ticks = (Total_Left_Encoder_Ticks + Total_Right_Encoder_Ticks) / 2.0;

			// : Speed Controll Logic :
			int difference = Total_Left_Encoder_Ticks-Total_Right_Encoder_Ticks;
			if(difference < 0) difference = -difference;
			const int max_difference_ticks = (const int)(ApproximateDeltaTicks_Drive(speed) * 0.05); // 5cm of ticks at current speed
			if(difference > max_difference_ticks)
			{
				if(Total_Left_Encoder_Ticks > Total_Right_Encoder_Ticks) // Left is going too fast
				{
					if(Right_Motor_Speed_Adjust < 1.0) Right_Motor_Speed_Adjust += 0.01; 	// +1%
						if(Left_Motor_Speed_Adjust > 0.9) Left_Motor_Speed_Adjust -= 0.01; 		// -1%
#ifdef DEBUG
					writeDebugStreamLine("Left Drive Motor Faster Than Right. Adjusting: [%d/%d]", Total_Left_Encoder_Ticks, Total_Right_Encoder_Ticks);
#endif
					}
				else // Right is gonig too fast
				{
					if(Left_Motor_Speed_Adjust < 1.0) Left_Motor_Speed_Adjust += 0.01; 		// +1%
						if(Right_Motor_Speed_Adjust > 0.9) Right_Motor_Speed_Adjust -= 0.01; 	// -1%
							#ifdef DEBUG
					writeDebugStreamLine("Right Drive Motor Faster Than Right. Adjusting: [%d/%d]", Total_Right_Encoder_Ticks, Total_Left_Encoder_Ticks);
				#endif
					}
			}

		}
		else if (current_event->Type == AutoEventType_Turn)
		{

		}
		else if (current_event->Type == AutoEventType_Dispense)
		{

		}
		else if (current_event->Type == AutoEventType_SeekIR)
		{

		}
		else if (current_event->Type == AutoEventType_PlotIR)
		{

		}
		else if (current_event->Type == AutoEventType_Arms)
		{

		}

	}
	else Auto_End();
};

void Auto_Add_Event(AutoEvent *event)
{
	Events[Num_Events] = *event;
	Num_Events++;
};

void RG1G2()
{
	AutoEvent event;

	event.Type = AutoEventType_Drive;
	event.data_f[EL_DRIVE_DISTANCE] = 1.0;
	event.data_f[EL_DRIVE_SPEED] = 1.0;
	Auto_Add_Event(&event);

	event.Type = AutoEventType_Turn;
	event.data_f[EL_ROTATION_ANGLE] = 90.0;
	event.data_f[EL_ROTATION_SPEED] = 1.0;
	Auto_Add_Event(&event)
}
