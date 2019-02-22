/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */

void getValues(){
	print("\n\n==================\n");
	printf("FR: %d\r\n", encoderGet(FR));
	printf("FL: %d\r\n", encoderGet(FL));
	printf("BR: %d\r\n", encoderGet(BR));
	printf("BL: %d\r\n", encoderGet(BL));
	printf("Joy1: %d\r\n", joystickGetAnalog(1,1));
	printf("Joy2: %d\r\n", joystickGetAnalog(1,2));
	printf("Joy4: %d\r\n", joystickGetAnalog(1,4));
	printf("MotorFL: %d\r\n", motorGet(motorFL));
	printf("MotorFR: %d\r\n", motorGet(motorFR));
	printf("MotorBL: %d\r\n", motorGet(motorBL));
	printf("MotorBR: %d\r\n", motorGet(motorBR));
	delay(1000);
}


void init(){
	TaskHandle output = taskRunLoop(getValues, 500);
	FRInit();
}

void resetEncoders(){
	encoderReset(FR);
	encoderReset(FL);
	encoderReset(BR);
	encoderReset(BL);
}

void parallelAll(){
	FRParallel();
	// FLParallel();
	// BRParallel();
	// BLparallel();
	// Uncomment after you makes these methods in drive_pid.c
}


void operatorControl() {
	int turn;
	int ydir;
	int xdir;
	print("Operator start\n");
	init();
	while (1) {
		delay(20);
		turn = joystickGetAnalog(1, 4);
		ydir = joystickGetAnalog(1, 1);
		xdir = joystickGetAnalog(1, 2);

		if (abs(turn) < 10) {
			turn = 0;
		}
		if (abs(ydir) < 10) {
			ydir = 0;
		}
		if (abs(xdir) < 10) {
			xdir = 0;
		}

		if (abs(ydir) > 110){
			xdir = 0;
			turn = 0;
		}
		if (abs(xdir) > 110){
			ydir = 0;
			turn = 0;
		}

		if (abs(xdir) < 10 && abs(ydir) < 10 && abs(turn) < 10){
			while ( abs(joystickGetAnalog(1,4)) < 10 && 
					abs(joystickGetAnalog(1,1)) < 10 && 
					abs(joystickGetAnalog(1,2)) < 10){
				resetEncoders();
				FRSetGoal(0);
				FRParallel();
			}
			FRStop();
		}

		motorSet(motorFL, ydir + xdir + turn);
		motorSet(motorFR, ydir - xdir + turn);
		motorSet(motorBL, -ydir + xdir + turn);
		motorSet(motorBR, -ydir - xdir + turn);
		
	}
}
