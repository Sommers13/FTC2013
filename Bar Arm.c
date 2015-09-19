#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     beacon,         sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     right,         tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     blocks,        tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     left,          tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     flag,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    left_finger,          tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    right_finger,         tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    autoBlockServo,       tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define autoBlockServo_ARM (80)
#define autoBlockServo_ARM_SPEED (30)
#define FLAG_ARM_SPEED (255)

#define AUTO_ARM_START 169
#define AUTO_ARM_TARGET 40
#define AUTO_ARM_END 80
#define AUTO_ARM_SPEED 255

#define RIGHT_FINGER_CLOSED (194)
#define LEFT_FINGER_CLOSED (15)

task main(){
	servoChangeRate[autoBlockServo] = 0;
	for(int i=0;i<50;i++)
	{
		servo[autoBlockServo] = autoBlockServo_ARM;
		servo[right_finger] = RIGHT_FINGER_CLOSED;
		servo[left_finger] = LEFT_FINGER_CLOSED;
		wait1Msec(5);
	}
	servoChangeRate[autoBlockServo] = AUTO_ARM_SPEED;

	while(true){
		if(nNxtButtonPressed==1){
			//Arm Up
			motor[lift] =  -100;
		wait1Msec(5);
		}

		if(nNxtButtonPressed==2){
			//Arm Up
			motor[lift] = 100;
		wait1Msec(5);
		}

		motor[lift] =  0;
	}
}
