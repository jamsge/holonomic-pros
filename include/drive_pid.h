#include <API.h>

#define DRIVE_DB 15 // the motors don't do much below this speed
#define DRIVE_TOLERANCE 20 // Depends heavily on the system
#define DRIVE_CONFIDENCE 5 // Means that the controller needs to be in the tolerance band for ~200ms, usually enough

#define DRIVE_KP 1.0f
#define DRIVE_KI 0.001f
#define DRIVE_KD 100.0f
#define DRIVE_INTEG_CAP 100000

void driveInit();
void driveSetPos(int position);
void driveMoveToPos();