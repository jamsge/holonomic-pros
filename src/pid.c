#include "main.h"


/** @file example.h
 *
 * ========= DEMO CODE =========
 * This file contains example code using BLRS's libfbc
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */
#include "pid.h"
#include "fbc.h"
#include "fbc_pid.h"

fbc_t FRController;
fbc_t FLController;
fbc_t BRController;
fbc_t BLController;

fbc_pid_t FRPID;
fbc_pid_t FLPID;
fbc_pid_t BRPID;
fbc_pid_t BLPID;

// Motor control methods:
static void _FRMove(int power) {
  motorSet(motorFR, power);
}
static int _FRSense() {
  return encoderGet(FR);
}
static void _FRReset() {
  encoderReset(FR);
}

static void _FLMove(int power){
    motorSet(motorFL, power);
}
static int _FLSense() {
    return encoderGet(FL);
}
static void _FLReset() {
    encoderReset(FL);
}

static void _BRMove(int power){
    motorSet(motorBR, power);
}
static int _BRSense() {
    return encoderGet(BR);
}
static void _BRReset() {
    encoderReset(BR);
}

static void _BLMove(int power){
    motorSet(motorBL, power);
}
static int _BLSense() {
    return encoderGet(BL);
}
static void _BLReset() {
    encoderReset(BL);
}


// Initialization, goal and position selection methods:
void FRInit() {
    fbcInit(&FRController, _FRMove, _FRSense, _FRReset,
            fbcStallDetect, -PID_DB, PID_DB, PID_TOLERANCE, PID_CONFIDENCE);
    fbcPIDInitializeData(&FRPID, PID_KP, PID_KI, PID_KD, -PID_INTEG_CAP, PID_INTEG_CAP);
    fbcPIDInit(&FRController, &FRPID);
}
void FLInit(){
    fbcInit(&FLController, _FLMove, _FLSense, _FLReset,
            fbcStallDetect, -PID_DB, PID_DB, PID_TOLERANCE, PID_CONFIDENCE);
    fbcPIDInitializeData(&FLPID, PID_KP, PID_KI, PID_KD, -PID_INTEG_CAP, PID_INTEG_CAP);
    fbcPIDInit(&FLController, &FLPID);
}
void BRInit(){
    fbcInit(&BRController, _BRMove, _BRSense, _BRReset,
            fbcStallDetect, -PID_DB, PID_DB, PID_TOLERANCE, PID_CONFIDENCE);
    fbcPIDInitializeData(&BRPID, PID_KP, PID_KI, PID_KD, -PID_INTEG_CAP, PID_INTEG_CAP);
    fbcPIDInit(&BRController, &BRPID);
}
void BLInit(){
    fbcInit(&BLController, _BLMove, _BLSense, _BLReset,
            fbcStallDetect, -PID_DB, PID_DB, PID_TOLERANCE, PID_CONFIDENCE);
    fbcPIDInitializeData(&BLPID, PID_KP, PID_KI, PID_KD, -PID_INTEG_CAP, PID_INTEG_CAP);
    fbcPIDInit(&BLController, &BLPID);
}
void initControllers(){
    FLInit();
    FRInit();
    BLInit();
    BRInit();
}

void FRSetPos(int position) {
  fbcSetGoal(&FRController, position);
}
void FLSetPos(int position) {
    fbcSetGoal(&FLController, position);
}
void BRSetPos(int position) {
    fbcSetGoal(&BRController, position);
}
void BLSetPos(int position) {
    fbcSetGoal(&BLController, position);
}
void setPosAll(int position){
    FRSetPos(position);
    FLSetPos(position);
    BRSetPos(position);
    BLSetPos(position);
}

void FRParallel() {
    fbcRunParallel(&FRController);
}
void FLParallel() {
    fbcRunParallel(&FLController);
}
void BRParallel() {
    fbcRunParallel(&BRController);
}
void BLParallel() {
    fbcRunParallel(&BLController);
}
void runParallelAll(){
    FRParallel();
    FLParallel();
    BRParallel();
    BLParallel();
}

void endAll(){
    taskDelete(&FRController);
    taskDelete(&FLController);
    taskDelete(&BRController);
    taskDelete(&BLController);
}
