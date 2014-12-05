// :: Functions ::
#define COUT writeDebugStream
#define COUTL writeDebugStreamLine

// :: Motors ::
#define Motor_Drive_Left mtr_S1_C3_1
#define Motor_Drive_Right mtr_S1_C3_2

#define Encoder_Drive_Left mtr_S1_C3_1
#define Encoder_Drive_Right mtr_S1_C4_2

#define Motor_Harvester motorA

#define Motor_Exhaust motorH

#define Motor_Arm_A mtr_S1_C2_1
#define Motor_Arm_B mtr_S1_C4_2
#define Motor_Arm_C mtr_S1_C2_2
#define Encoder_Arm_A Motor_Arm_A
#define Encoder_Arm_B mtr_S1_C3_1
#define Encoder_Arm_C Motor_Arm_C

// :: Servos ::
#define Servo_Stack servo1
#define Servo_GoalKeeper servo3
#define Servo_Arm_Right servo5
#define Servo_Arm_Left servo6

#define Servo_Arm_Left_Down 230
#define Servo_Arm_Left_Up 0
#define Servo_Arm_Right_Down 0
#define Servo_Arm_Right_Up 230


/*
* Returns the current time value of a timer
*/
float GetTime(TTimers timer)
{
	return (float)time1[timer] / 1000.0;
}

/*
 * Enum for harvester status
*/
enum HarvesterStatusEnum {
    HarvesterStatus_Forward = 100,
    HarvesterStatus_Stopped = 0,
    HarvesterStatus_Reversed = -100,
};

/*
 * Enum for exhaust status
*/
enum ExhaustStatusEnum {
    ExhaustStatus_Forward = 25,
    ExhaustStatus_Stopped = 0,
    ExhaustStatus_Reversed = -25,
};
