#pragma config(Sensor, S1,     TIR,            sensorEV3_GenericI2C)
#pragma config(Sensor, S2,     HTSMUX,         sensorI2CCustom)
#pragma config(Sensor, S3,     colorSensor,    sensorEV3_Color, modeEV3Color_Color)
#pragma config(Sensor, S4,     Ir_Sensor,      sensorEV3_IRSensor)
#pragma config(Motor,  motorA,          armMotor,      tmotorEV3_Large, PIDControl, encoder)
#pragma config(Motor,  motorB,          leftMotor,     tmotorEV3_Large, PIDControl, driveLeft, encoder)
#pragma config(Motor,  motorC,          rightMotor,    tmotorEV3_Large, PIDControl, driveRight, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// #pragma config(Sensor, S1,     Ir_Sensor,      sensorEV3_IRSensor)
// #pragma config(Sensor, S4,     HTIRS2,         sensorI2CCustom)
// //#pragma config(Sensor, S4,     sonarSensor,    sensorEV3_Ultrasonic) // can't have comments in between pragma config
#include "hitechnic-irseeker-v2.h"
#include "hitechnic-eopd.h"

char robot_no = 'E';
int comma = 44;
#define ARDUINO_ADDRESS 0x14 // or 0x08
#define ARDUINO_PORT S1 //MODIFY SENSOR PORT


ubyte I2Cmessage[7];
char I2Creply[20];

void i2c_message(int message_size, int return_size, int robot_no, long red, int comma, long blue);
//void stopGoInterrupt();


int proximity;
int proximity_1;

//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#pragma config(StandardModel, "EV3_REMBOT")
void halt(){
	//	LIKE sleep (1000); and then execute the code

	setMotorSpeed(leftMotor, 0); //Set the leftMotor (motor1) to  power (0)
	setMotorSpeed(rightMotor, 0); //Set the rightMotor (motor6) to  power (0)
	//sleep(1000); //Wait for 1 seconds before continuing on in the programm
	// break_1(); // calling funtion to break
}

void forwardmotion(){

	setMotorSpeed(leftMotor, 20); //Set the leftMotor (motor1) to half power (20)
	setMotorSpeed(rightMotor, 20); //Set the rightMotor (motor6) to half power (20)
	// sleep(5000); //Wait for 5 seconds before continuing on in the programm
	//return 0;
	//break ; error break is not valid at this point

}

void backwardmotion(){
	setMotorSpeed(leftMotor, -20); //Set the leftMotor (motor1) to half power (-20)
	setMotorSpeed(rightMotor, -20); //Set the rightMotor (motor6) to half power (-20)
	// sleep(5000); //Wait for 5 seconds before continuing on in the programm

}

void circularmotion(){
	setMotorSpeed(leftMotor, -20); //Set the leftMotor (motor1) to half power (-20)
	setMotorSpeed(rightMotor, 20); //Set the rightMotor (motor6) to half power (20)
	// sleep(5000); //Wait for 2 seconds before continuing on in the programm
}

void rotateclockwise(){
	setMotorSpeed(leftMotor, 20); //Set the leftMotor (motor1) to half power (20)
	setMotorSpeed(rightMotor, 0); //Set the rightMotor (motor6) to half power (0)
	// sleep(5000); //Wait for 5 seconds before continuing on in the programm

}
void rotatecounterclockwise(){

	//	clearTimer(T2);
	setMotorSpeed(leftMotor, 0); //Set the leftMotor (motor1) to half power (0)
	setMotorSpeed(rightMotor, 20); //Set the rightMotor (motor6) to half power (20)
	// sleep(5000); //Wait for 1 seconds before continuing on in the programm
}

int select_motion(){

	int motion_selector;
	int a;
	if (proximity < 40 || proximity_1 > 15){
		a = random(4); //generates random number from 0 to 4
		// OR COULD PUT MOTOR SPEED ON RANDOM FUNCTION (CHECK)
		motion_selector = a + 1;
		// adds 1 to the lower and higher limit of random number a.
	}
	//sleep(1000);  // sleep is not the right option
	return(motion_selector);
} //END FUNCTION

task main() {

	// Create struct to hold sensor data
	tHTEOPD eopdSensor;
	// Initialise and configure struct and port
	// Default mode for the EOPD is short range
	initSensor(&eopdSensor, S2);
	int RGB; // reading color sensor and storing
	long redValue; // color Red
	long greenValue; //color Green
	long blueValue; //Color Blue

	int proximity;
	int proximity_1;
	int motion;
	while (1)
	{
		if (time1[T1] > 1000)
		{
			if (eopdSensor.shortRange == false)
			{
				eopdSensor.shortRange = true;
				configSensor(&eopdSensor);
				displayClearTextLine(1);
			}
			else
			{
				eopdSensor.shortRange = false;
				configSensor(&eopdSensor);
				displayClearTextLine(1);
			}
			playSound(soundBeepBeep);
			while(bSoundActive)
				time1[T1] = 0;
		}
		configSensor(&eopdSensor);
		readSensor(&eopdSensor);

		//TRY NORMALIZING BOTH PROXIMITY

		// getting sensor distance
		proximity = getIRDistance(Ir_Sensor);  // decreases if they are close // CODE GOES HERE EVERYTIME TO GET VALUE

		// getting distance from eopd sensor
		proximity_1 = eopdSensor.processed ;	// increases if they are close &added
		RGB = getColorRGB(colorSensor, redValue, greenValue, blueValue);

		i2c_message(4, 4, robot_no, redValue, comma, blueValue); // 1 is changed to 4 is changed

		displayCenteredTextLine(0, " proximity is %d ", proximity);
		displayCenteredTextLine(1, " proximity_1 is %d ", proximity_1);
		displayCenteredTextLine(2, " motion is %d ", motion);
		displayCenteredTextLine(4, " Red is %d",redValue);
		displayCenteredTextLine(5, " Green is %d",greenValue);
		displayCenteredTextLine(6, " Blue is %d",blueValue);


		if (proximity < 40 || proximity_1 > 15){ // UPDATING MOTION WITH CHANGE IN PROXIMITY
			motion = select_motion();
			halt(); // PRIORITY STOP
			clearTimer(T3); // PROPER PLACE FOR CLEARING TIMER
		}
		else {
			switch (motion) { // can put function here directly as well

			case 1: //forward motion
				if (proximity < 40 || proximity_1 > 15 ){
					halt();
				}
				else {
					//	while (time1[T3] < 5000) // executes till 5 second regardless check
					forwardmotion();
				}
				displayCenteredTextLine(3, " Timer_3 is %d ", time1[T3]);
				//	clearTimer(T3);
				break; // HOW DOES THIS BREAK WORKS ?

			case 2: //turn clockwise
				if (proximity < 40 || proximity_1 > 15)  // manually break after 1s
				{
					halt();
				}
				else {
					while (time1[T3] != 5000) { // added
						displayCenteredTextLine(3, " Timer_3 is %d ", time1[T3]);
						rotateclockwise();
						// motion = select_motion(); // added
						break;//added
					} // added
				}
				break;

			case 3:  //turn counter clockwise
				if (proximity < 40 || proximity_1 > 15 ){
					halt();
				}
				else  {
					displayCenteredTextLine(3, " Timer_3 is %d ", time1[T3]);
					rotatecounterclockwise();
				}
				break;

			case 4:
				if (proximity < 40 || proximity_1 > 15 ){
					halt();
				}
				else {
					circularmotion();
					displayCenteredTextLine(3, " Timer_3 is %d ", time1[T3]);
				}
				break;

			case 5: //backward motion
				backwardmotion();
				displayCenteredTextLine(3, " Timer_3 is %d ", time1[T3]);
				if (proximity < 40 || proximity_1 > 15){
					halt();
				}
				break;

			} // END SWITCH STATEMENT

		} //END IF
		//displayCenteredTextLine(7, " Timer_2 is %d ", time1[T3]);

	} //END WHILE LOOP


} //END MAIN
//NOTE TIMER T3 ONLY COUNTS WHEN THE PROXIMITY IS CLOSER TOGETHER.

void i2c_message(int message_size, int return_size, int robot_no, long red, int comma, long blue)
{
	memset(I2Creply, 0, sizeof(I2Creply));
	message_size = message_size+1;

	I2Cmessage[0] = message_size; // Messsage Size
	I2Cmessage[1] = ARDUINO_ADDRESS;
	I2Cmessage[2] = robot_no;
	I2Cmessage[3] = red; // Register
	I2Cmessage[4] = comma;
	I2Cmessage[5] = blue;
	sendI2CMsg(S1, &I2Cmessage[0], return_size);
	writeDebugStream("Wrote %d, %d, %d, %d \n",I2Cmessage[2],I2Cmessage[3], I2Cmessage[4], I2Cmessage[5]);
	wait1Msec(400);			//changed from 500Msec
	readI2CReply(ARDUINO_PORT, &I2Creply[0], return_size); //have return size of 2 for two coordinates
	writeDebugStream("Received %d, %d, %d, %d \n", I2Creply[0], I2Creply[1], I2Creply[2], I2Creply[3]);
}