// :: Functions ::
#define COUT writeDebugStream
#define COUTL writeDebugStreamLine

// :: Motors ::
#define Motor_Drive_Left mtr_S1_C3_1
#define Motor_Drive_Right mtr_S1_C3_2

#define Encoder_Drive_Left mtr_S1_C3_1
#define Encoder_Drive_Right mtr_S1_C3_2

#define Motor_Arm_Left motorB
#define Motor_Arm_Right motorC

#define Motor_ScrewLift motorH
#define Motor_Harvester motorA
#define Motor_Hook motorB

#define Motor_Arm_A mtr_S1_C2_1
#define Motor_Arm_B mtr_S1_C4_2
#define Motor_Arm_C mtr_S1_C2_2
#define Encoder_Arm_A Motor_Arm_A
#define Encoder_Arm_B mtr_S1_C3_1
#define Encoder_Arm_C Motor_Arm_C

// :: Servos ::
#define Servo_Bucket servo1
#define Servo_GoalAttach servo3
#define Servo_Arm_Left servo5
#define Servo_Arm_Right servo6

#define Servo_GoalAttach_Attach 60
#define Servo_GoalAttach_Dettach 230

// :: Sensors ::
#define Sensor_IR HTIRS2

/*
* Returns the current time value of a timer
*/
float GetTime(TTimers timer)
{
	return (float)time1[timer] / 1000.0;
}
