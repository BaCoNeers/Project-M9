// :: Definitions ::
#define MAX_AUTO_EVENTS 100
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

// :: Functions ::


// Autonomous update function
void Auto_Update()
{
	AutoEvent* current_event = &Events[Current_Event];

	if (current_event->Type == AutoEventType_Drive)
	{
		int speed = 100 * current_event->data_f[EL_DRIVE_SPEED];
		int left_encoder_ticks = nMotorEncoder[Encoder_Drive_Left];
		int right_encoder_ticks = nMotorEncoder[Encoder_Drive_Right];
		float distance_left = 10.0 * (float)Total_Left_Encoder_Ticks;
		float distance_right = 10.0 * (float)Total_Right_Encoder_Ticks;
		float delta_distance = (Total_Left_Encoder_Ticks + Total_Right_Encoder_Ticks) / 2.0;
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
	Auto_Add_Event(&event);
}
