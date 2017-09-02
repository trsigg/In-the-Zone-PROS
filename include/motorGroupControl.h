#ifndef GROUP_CONTROL_INCLUDED
#define GROUP_CONTROL_INCLUDED

#include "config.h"

void waitForMovementToFinish(bool waitForChain=true, bool waitForLift=true, int timeout=75, double chainMargin=7, double liftMargin=10);

//#region lift
void setLiftState(liftState state);
double liftHeight();
//#endregion

//#region chain bar
void setChainBarState(chainState state);
//#endregion

#endif
