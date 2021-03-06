#pragma config(Hubs,  S1, HTServo,  HTMotor,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     beacon,         sensorHiTechnicIRSeeker1200)
#pragma config(Sensor, S3,     sonar,          sensorSONAR)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     right,         tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     blocks,        tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C3_2,     left,          tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     flag,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C1_1,    left_finger,          tServoStandard)
#pragma config(Servo,  srvo_S1_C1_2,    right_finger,         tServoStandard)
#pragma config(Servo,  srvo_S1_C1_3,    autoBlockServo,       tServoStandard)
#pragma config(Servo,  srvo_S1_C1_4,    servo4,               tServoContinuousRotation)
#pragma config(Servo,  srvo_S1_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"//needed for waitForStart()

// ------------------------------------------------------------
// REMMEMBER! This is for R I G H T !!!!!!!!!!!!!!!!!!

#define Wall_Follow_Distance 35
#define See_Wall_Min 0
#define See_Wall_Max 55
#define WallFollow_Correction_Factor 2

#define Beacon_Stop_Correction 0
#define Drive_To_First_Turn 200
#define First_Turn 75
#define Drive_To_Second_Turn 1100
#define Second_Turn 200
#define Drive_To_Third_Turn -1000
#define Third_turn 120
#define Drive_Up_Ramp 5000

#define AUTO_ARM_START 169
#define AUTO_ARM_TARGET 40
#define AUTO_ARM_END 80
#define AUTO_ARM_SPEED 255

// ===========================================================
// Procedure/Function/Task interfaces definitions
// ===========================================================
void BlockPlacement();
void DriveUntilBeacon();
void EncoderDrive(long _distTrav);
void EncoderTurn(int speed, int degrees, bool turnRight);
void DriveUntilEndOfWall(int _Direction);

// ===========================================================
// Globals
// ===========================================================
bool g_BeaconFound = false;
int g_Speed = -40;
int g_TurnSpeed = 30;
int g_MotorOffset = -7;
//former value -13


// clicks / inch = 154.83
// inches / click =
const float ClicksPerInch = 41.6667;
// inch / degree = .1196
//float InchPerDegree = .142;
float InchPerDegree = .35;//more turns more
const float InchPerDegreeLeftTurn = .35;//more turns more

//============================================================================
//== task main
//============================================================================
task main(){
	servoChangeRate[autoBlockServo] = 0;
	for(int i=0;i<50;i++)
	{
		servo[autoBlockServo] = AUTO_ARM_START;
		wait1Msec(5);
	}
	servoChangeRate[autoBlockServo] = AUTO_ARM_SPEED;

	waitForStart();

	// Zero out encoders at start
	nMotorEncoder[right] = 0;
	nMotorEncoder[left] = 0;

	DriveUntilBeacon();

	BlockPlacement();

	// Drive back until end of wall
	DriveUntilEndOfWall(-1);

	//while(true);

	PlaySound(soundBeepBeep);

	// drive a bit more
	EncoderDrive(Drive_To_First_Turn);

	wait1Msec(200);

	PlaySound(soundBeepBeep);

	// first turn tward ramp
 	EncoderTurn(g_TurnSpeed, First_Turn, true);

 	wait1Msec(200);

	PlaySound(soundBeepBeep);

	// drive a bit
 	EncoderDrive(Drive_To_Second_Turn);

 	wait1Msec(200);

	PlaySound(soundBeepBeep);

	// second turn tward ramp
 	EncoderTurn(g_TurnSpeed, Second_Turn, true);

 	// drive a bit
 	EncoderDrive(Drive_To_Third_Turn);

	PlaySound(soundBeepBeep);

	// turn to face ramp
 	EncoderTurn(g_TurnSpeed, Third_turn, true);

 	wait1Msec(200);

	PlaySound(soundBeepBeep);
//while(true);
	// Get on ramp
 	EncoderDrive(Drive_Up_Ramp);

	PlaySound(soundBeepBeep);
}

//============================================================================
//=== DriveUntilEndOfWall
//============================================================================
// Drive until the beacon is found
void DriveUntilEndOfWall(int _Direction) {
	bool endOfWall = false;

	while(!endOfWall) {
		int WallError = 0;

		int sonarValue = SensorValue[sonar];
		nxtDisplayTextLine(2, "Sonar = %1.0f", sonarValue);

		// Calc Wall Following Error
		if(sonarValue > See_Wall_Min && sonarValue < See_Wall_Max){
			// We can see the wall, calc wall error
			WallError = Wall_Follow_Distance - sonarValue;
		}else{
			// We can't see the wall, just drive straight
			WallError = 0;
			endOfWall = true;
		}

		motor[right] = -g_Speed*_Direction + (.3 * WallError);
		motor[left] = -g_Speed*_Direction + g_MotorOffset - (.3 * WallError);
	}
	motor[right] = 0;
	motor[left] = 0;
}

//============================================================================
//=== DriveUntilBeacon
//============================================================================
// Drive until the beacon is found
void DriveUntilBeacon() {
	while(!g_BeaconFound) {
		int WallError = 0;
		bool sonarASeesWall = false;

		int sonarValueA = SensorValue[sonar];

		//nxtDisplayTextLine(2, "Sonar = %1.0f", sonarValue);

		if(sonarValueA > See_Wall_Min && sonarValueA < See_Wall_Max){
			//WallError = Wall_Follow_Distance - sonarValueA;
		}else{
			WallError = 0;
		}

		motor[right] = -g_Speed - (WallFollow_Correction_Factor * WallError);
		motor[left] = -g_Speed - g_MotorOffset + (WallFollow_Correction_Factor * WallError);

		int beaconValue = SensorValue[beacon];
	  nxtDisplayTextLine(1, "Beacon = %1.0f", beaconValue);

		if(beaconValue == 5) g_BeaconFound = true;
	}
	motor[right] = 0;
	motor[left] = 0;
}

//============================================================================
//=== BlockPlacement
//============================================================================
void BlockPlacement(){
	//g_BlockPlaced = true;
	servo[autoBlockServo] = AUTO_ARM_TARGET;
	wait1Msec(500);
	servo[autoBlockServo] = AUTO_ARM_END;
	wait1Msec(500);
	servo[autoBlockServo] = AUTO_ARM_START;
	wait1Msec(500);
	//PlaySound(soundBeepBeep);

	//g_DistanceTraveled = (nMotorEncoder[right]);
}

//============================================================================
//=== EncoderDrive
//============================================================================
void EncoderDrive(long _distTrav){
	// clicks must be divided by the clicks/inch
	//float distrightInches = dist/ClicksPerInch;
	//float distleftInches = dist/ClicksPerInch;

	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;
	if (_distTrav > 0) {
		while(abs(nMotorEncoder[left]) <= _distTrav && abs(nMotorEncoder[right]) <= _distTrav) {
	  	motor[right] = g_Speed;
			motor[left] = g_Speed + g_MotorOffset;
			//nxtDisplayTextLine(2, "%Left = 1.2f", nMotorEncoder[left]);
			//nxtDisplayTextLine(3, "%Righ = 1.2f", nMotorEncoder[right]);
		}
	} else {
		while(abs(nMotorEncoder[left]) <= abs(_distTrav) && abs(nMotorEncoder[right]) <= abs(_distTrav)) {
	  	motor[right] = -g_Speed;
			motor[left] = -(g_Speed + g_MotorOffset);
			//nxtDisplayTextLine(2, "%Left = 1.2f", nMotorEncoder[left]);
			//nxtDisplayTextLine(3, "%Righ = 1.2f", nMotorEncoder[right]);
		}
	}
	nxtDisplayTextLine(2, "Success!");
	motor[right] = 0;
	motor[left] = 0;
}

//============================================================================
//=== EncoderTurn
//============================================================================
// turns the robot base on degrees given
void EncoderTurn(int speed, int degrees, bool turnRight){
	int turnFactor;
	if(turnRight){
		turnFactor = -1;
	}else{
		turnFactor = 1;
		InchPerDegree = InchPerDegreeLeftTurn;
	}

	float distInches = abs(degrees) * InchPerDegree;
	float distClicks = distInches * ClicksPerInch;

	nMotorEncoder[left] = 0;
	nMotorEncoder[right] = 0;

	eraseDisplay();

	//while(abs(nMotorEncoder[left]) <= distClicks && abs(nMotorEncoder[right]) <= distClicks) {
  while(abs(nMotorEncoder[left]) <= distClicks) {
  	motor[right] = speed*turnFactor;
		motor[left] = -speed*turnFactor;
		nxtDisplayTextLine(2, "LeftEn= %1.0f", abs(nMotorEncoder[left]));
		nxtDisplayTextLine(3, "RightEn= %1.0f", abs(nMotorEncoder[right]));
		nxtDisplayTextLine(4, "distClicks= %1.2f", distClicks);
	}
	motor[right] = 0;
	motor[left] = 0;
	//nxtDisplayTextLine(2, "LeftEn= %1.0f", nMotorEncoder[left]);
	//nxtDisplayTextLine(3, "RightEn= %1.0f", nMotorEncoder[right]);
	//nxtDisplayTextLine(4, "distClicks= %1.2f", distClicks);
}
