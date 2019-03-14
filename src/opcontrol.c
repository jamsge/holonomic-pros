/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "fbc.h"
#include "fbc_pid.h"
#include "pid.h"

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

// Reset all encoders
void resetEncoders(){
	encoderReset(FR);
	encoderReset(FL);
	encoderReset(BR);
	encoderReset(BL);
}

// Print all encoder positions
void printVals(){
	print("\n\n==================\n");
	printf("FR: %d\r\n", encoderGet(FR));
	printf("FL: %d\r\n", encoderGet(FL));
	printf("BR: %d\r\n", encoderGet(BR));
	printf("BL: %d\r\n", encoderGet(BL));
}

void operatorControl() {
	int turn;
	int ydir;
	int xdir;
	bool isMoving = false;
	// Run printVals task every 1000ms
	TaskHandle taskhandle = taskRunLoop(printVals, 1000);

	// Initialize all encoders
	initControllers();

	// Reset all encoders
	resetEncoders();
	
	print("Operator start\n");
	while (true) {
		// Assign joysticks to holonomic movement
		turn = joystickGetAnalog(1, 4);
		ydir = joystickGetAnalog(1, 1);
		xdir = joystickGetAnalog(1, 2);

		// joystick deadzones
		if (abs(turn) < 15) {
			turn = 0;
		}
		if (abs(ydir) < 15) {
			ydir = 0;
		}
		if (abs(xdir) < 15) {
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
		if (abs(turn) > 110){
			ydir = 0;
			xdir = 0;
		}

		// when moving, always reset the encoders and set position goal for all to zero
		// this is because if the drive stops at any moment, it can use 0 as a home position for PID braking
		setPosAll(0);
		resetEncoders();
		while (abs(joystickGetAnalog(1,1)) < 20 
		&& abs(joystickGetAnalog(1,2)) < 20 
		&& abs(joystickGetAnalog(1,4)) < 20){
			// drive should no longer be moving
			isMoving = false;

			// run all PID controllers in parallel
			runParallelAll();

			// turn on LED at port 1
			digitalWrite(1, HIGH);
		}
		digitalWrite(1, LOW);

		// this use of isMoving ensures that endAll() is only executed once when the drive starts moving again
		// endAll() only gets used when exiting the last loop
		if (!isMoving){
			isMoving = true;
			endAll();
		}
		
		// set motor speeds according to joystick locations
		motorSet(motorFL, ydir + xdir + turn);
		motorSet(motorFR, ydir - xdir + turn);
		motorSet(motorBL, -ydir + xdir + turn);
		motorSet(motorBR, -ydir - xdir + turn);
	}
}
