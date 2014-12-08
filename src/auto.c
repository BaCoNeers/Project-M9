// :: Definitions ::
#define MAX_AUTO_EVENTS 100
#define MAX_EVENT_TIME 5.0
// Event location definitions
// float
#define EL_DRIVE_CURRENT_DISTANCE 2
#define EL_DRIVE_DISTANCE 0
#define EL_DRIVE_SPEED 1
#define EL_ROTATION_CURRENT_ANGLE_DIST 2
#define EL_ROTATION_ANGLE_DIST 0
#define EL_ROTATION_SPEED 1
#define EL_DISPENSE_HEIGHT 0
#define EL_ARM_ENUM 0
#define EL_CATCH_MODE 0
#define EL_EVENT_TIME 5

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
	AutoEventType_Arms,
	AutoEventType_Catch
};

enum AutoCatchMode
{
	AutoCatchMode_Down,
	AutoCatchMode_Up
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
AutoEvent Events[MAX_AUTO_EVENTS];

int Num_Events = 0;
int Current_Event = 0;

int Total_Left_Encoder_Ticks = 0;
int Total_Right_Encoder_Ticks = 0;

float Left_Motor_Speed_Adjust = 1.0;
float Right_Motor_Speed_Adjust = 1.0;

// :: Functions ::

float Auto_AngleToDistance(float angle)
{
	const float c = (fpi * 0.4) / 180.0;
	return angle * c;
}

void Auto_End_Event()
{
#ifdef DEBUG
	float time = Events[Current_Event].data_f[EL_EVENT_TIME];
	if(Current_Event >= Num_Events)	writeDebugStreamLine("All Events Completed Total Time:%d", );
	else
	{
		if(Events[Current_Event].Type == AutoEventType_Drive) writeDebugStream("AutoEventType_Drive");
		else if(Events[Current_Event].Type == AutoEventType_Turn) writeDebugStream("AutoEventType_Turn");
		else if(Events[Current_Event].Type == AutoEventType_Dispense) writeDebugStream("AutoEventType_Dispense");
		else if(Events[Current_Event].Type == AutoEventType_SeekIR) writeDebugStream("AutoEventType_SeekIR");
		else if(Events[Current_Event].Type == AutoEventType_PlotIR) writeDebugStream("AutoEventType_PlotIR");
		else if(Events[Current_Event].Type == AutoEventType_Arms) writeDebugStream("AutoEventType_Arms");
		else if(Events[Current_Event].Type == AutoEventType_Catch) writeDebugStream("AutoEventType_Catch");
		else writeDebugStream("AutoEventType_Unknown");
		writeDebugStreamLine(" Event Complete! Total Time: %d", time);
	}
#endif

	Current_Event++;
	Total_Left_Encoder_Ticks = 0;
	Total_Right_Encoder_Ticks = 0;
				motor[Motor_Drive_Left] = 0;
			motor[Motor_Drive_Right] = 0;

}

void Auto_Init()
{
	Total_Left_Encoder_Ticks = 0;
	Total_Right_Encoder_Ticks = 0;
					motor[Motor_Drive_Left] = 0;
			motor[Motor_Drive_Right] = 0;
}

void Auto_End()
{

}

// Autonomous update function
void Auto_Update(float delta)
{
	if(Current_Event < Num_Events)
	{
		AutoEvent* pcurrent_event = &Events[Current_Event];
		pcurrent_event->data_f[EL_EVENT_TIME] += delta; // Accumulate Time

		if (pcurrent_event->Type == AutoEventType_Drive) // Drive
		{
			int speed = 100 * pcurrent_event->data_f[EL_DRIVE_SPEED];
			int left_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Left];
			nMotorEncoder[Encoder_Drive_Left] = 0;
		left_delta_encoder_ticks = left_delta_encoder_ticks<0 ? -left_delta_encoder_ticks : left_delta_encoder_ticks;
			int right_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Right];
			nMotorEncoder[Encoder_Drive_Right] = 0;
		right_delta_encoder_ticks = right_delta_encoder_ticks<0 ? -right_delta_encoder_ticks : right_delta_encoder_ticks;
			float delta_ticks = (float)(left_delta_encoder_ticks + right_delta_encoder_ticks) / 2.0;

			Total_Left_Encoder_Ticks += left_delta_encoder_ticks;
			Total_Right_Encoder_Ticks += right_delta_encoder_ticks;

			// : Speed Control Logic :
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
			//Set Motor
			motor[Motor_Drive_Left] = speed * fClamp(Left_Motor_Speed_Adjust,0.9,1.0);
			motor[Motor_Drive_Right] = -speed * fClamp(Right_Motor_Speed_Adjust,0.9,1.0);

			//Calculate Distance
			float delta_distance = ApproximateDistance_Drive(delta_ticks, speed);
			pcurrent_event->data_f[EL_DRIVE_CURRENT_DISTANCE]+=delta_distance;
			if(pcurrent_event->data_f[EL_DRIVE_CURRENT_DISTANCE]>
				pcurrent_event->data_f[EL_DRIVE_DISTANCE])
			{
				Auto_End_Event(); // Event Finished
			}
		}
		else if (pcurrent_event->Type == AutoEventType_Turn) // Turn
		{
						int speed = 100 * pcurrent_event->data_f[EL_ROTATION_SPEED];
			int left_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Left];
			nMotorEncoder[Encoder_Drive_Left] = 0;
		left_delta_encoder_ticks = left_delta_encoder_ticks<0 ? -left_delta_encoder_ticks : left_delta_encoder_ticks;
			int right_delta_encoder_ticks = nMotorEncoder[Encoder_Drive_Right];
			nMotorEncoder[Encoder_Drive_Right] = 0;
		right_delta_encoder_ticks = right_delta_encoder_ticks<0 ? -right_delta_encoder_ticks : right_delta_encoder_ticks;
			float delta_ticks = (float)(left_delta_encoder_ticks + right_delta_encoder_ticks) / 2.0;

			Total_Left_Encoder_Ticks += left_delta_encoder_ticks;
			Total_Right_Encoder_Ticks += right_delta_encoder_ticks;

			// : Speed Control Logic :
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
			//Set Motor
			motor[Motor_Drive_Left] = (speed * fClamp(Left_Motor_Speed_Adjust,0.9,1.0));
			motor[Motor_Drive_Right] = (speed * fClamp(Right_Motor_Speed_Adjust,0.9,1.0));

			//Calculate Distance
			float delta_distance = ApproximateDistance_Drive(delta_ticks, speed);
			pcurrent_event->data_f[EL_ROTATION_CURRENT_ANGLE_DIST]+=delta_distance;

			if(pcurrent_event->data_f[EL_ROTATION_CURRENT_ANGLE_DIST]>
				pcurrent_event->data_f[EL_ROTATION_ANGLE_DIST])
			{
				Auto_End_Event(); // Event Finished
			}
		}
		else if (pcurrent_event->Type == AutoEventType_Dispense)
		{
			const float time = pcurrent_event->data_f[EL_EVENT_TIME];
			if(time > 6)
			{
				motor[Motor_Arm_A] = 0;

				Auto_End_Event();
			}
			else
				if(time > 4.5)
			{
				motor[Motor_Arm_A] = 30;
			}
			else if(time > 4.0)
			{
				motor[Motor_Arm_A] = 0;
				servo[Servo_Stack] = 0;
				motor[Motor_Arm_B] = 0;
			}
			else if(time > 3.0)
			{
				motor[Motor_Arm_A] = 0;
				servo[Servo_Stack] = 150;
				motor[Motor_Arm_B] = 20;
			}
			else if(time > 1.0)
			{
				motor[Motor_Arm_A] = 0;
				servo[Servo_Stack] = (int)(fClamp((time-1.0),0.0,1.0) * 64.0);
			}
			else
			{
				motor[Motor_Arm_A] = -35; // Up
				servo[Servo_Stack] = 0;
			}
		}
		else if (pcurrent_event->Type == AutoEventType_SeekIR)
		{

		}
		else if (pcurrent_event->Type == AutoEventType_PlotIR)
		{

		}
		else if (pcurrent_event->Type == AutoEventType_Arms)
		{

		}
		else if (pcurrent_event->Type == AutoEventType_Catch)
		{
			// Toggle the Catcher
			servo[Servo_GoalKeeper] = (bool)pcurrent_event->data_i[EL_CATCH_MODE] ? 100: 60;
			sleep(150);
			Auto_End_Event(); // Event Finished
		}
	}
	else Auto_End();
};

void Auto_Add_Event(AutoEvent *event)
{
	memcpy(&Events[Num_Events], event, sizeof(AutoEvent)); // Fix RobotC Bug
	//Events[Num_Events] = &event;
	Num_Events++;
};

void OffRamp()
{
	AutoEvent event;

	event.Type = AutoEventType_Drive;
	event.data_f[EL_DRIVE_DISTANCE] = 1.75;
	event.data_f[EL_DRIVE_SPEED] = 0.5;
	Auto_Add_Event(&event);

	event.Type = AutoEventType_Turn;
	event.data_f[EL_ROTATION_ANGLE_DIST] = Auto_AngleToDistance(90.0);
	event.data_f[EL_ROTATION_SPEED] = 1.0;
	Auto_Add_Event(&event);

	event.Type = AutoEventType_Drive;
	event.data_f[EL_DRIVE_DISTANCE] = 0.85;
	event.data_f[EL_DRIVE_SPEED] = 1.0;
	Auto_Add_Event(&event);

/*
	event.Type = AutoEventType_Catch;
	event.data_i[EL_CATCH_MODE] = AutoCatchMode_Down;
	Auto_Add_Event(&event);

	event.Type = AutoEventType_Dispense;
	event.data_i[EL_DISPENSE_HEIGHT] = AutoDispenseLevel_Medium;
	Auto_Add_Event(&event);

	event.Type = AutoEventType_Catch;
	event.data_i[EL_CATCH_MODE] = AutoCatchMode_Up;
	Auto_Add_Event(&event);
	*/
}
