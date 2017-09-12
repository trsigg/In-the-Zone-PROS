#ifndef GROUP_CONTROL_INCLUDED
#define GROUP_CONTROL_INCLUDED

#include "config.h"

void waitForMovementToFinish(bool waitForChain=true, bool waitForLift=true, unsigned short timeout=75, double chainMargin=7, double liftMargin=10);

//#region lift
double liftHeight();
//#endregion

#endif
