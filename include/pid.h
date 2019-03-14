#ifndef _PID_H_
#define _PID_H_

#include <API.h>

void FRInit();
void FLInit();
void BRInit();
void BLInit();

void BLSetPos();
void BRSetPos();
void FLSetPos();
void FRSetPos();

void initControllers();
void setPosAll(int position);
void runParallelAll();
void endAll();

#define PID_DB 15 // the motors don't do much below this speed
#define PID_TOLERANCE 20 // Depends heavily on the system
#define PID_CONFIDENCE 5 // Means that the controller needs to be in the tolerance band for ~200ms, usually enough

#define PID_KP 1.0f
#define PID_KI 0.001f
#define PID_KD 100.0f
#define PID_INTEG_CAP 100000


#endif