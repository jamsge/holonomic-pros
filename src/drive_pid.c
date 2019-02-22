#include "main.h"
#include "drive_pid.h"
#include "fbc.h"
#include "fbc_pid.h"

fbc_t FRController;
fbc_pid_t FRPID;

static void moveFR(int power){
    motorSet(motorFR, power);
}
static int senseFR(){
    return encoderGet(FR);
}
static void resetFR() {
    encoderReset(FR);
}
void FRInit() {
    fbcInit(&FRController, moveFR, senseFR, resetFR, 
        fbcStallDetect, -DRIVE_DB, DRIVE_DB, DRIVE_TOLERANCE, DRIVE_CONFIDENCE);
    fbcPIDInitializeData(&FRPID, DRIVE_KP, DRIVE_KI, DRIVE_KD, -DRIVE_INTEG_CAP, DRIVE_INTEG_CAP);
    fbcPIDInit(&FRController, &FRPID);
}
void FRSetGoal(int position){
    fbcSetGoal(&FRController, position);
}
void FRParallel(){
    fbcRunParallel(&FRController);
}
void FRStop(){
    taskDelete(&FRController);
}

