#include "config.h"
#include "timer.h"

double liftPos[] = LIFT_POS;
double chainPos[] = CHAIN_POS;

void waitForMovementToFinish(bool waitForChain=true, bool waitForLift=true, unsigned short timeout=75, double chainMargin=7, double liftMargin=10) {
	Timer movementTimer;

	while (movementTimer.time() < timeout) {
		if ((!chainBar.errorLessThan(chainMargin) && waitForChain) ||
				(!lift.errorLessThan(liftMargin) && waitForLift))
			movementTimer.reset();
	}
}

//#region lift
void setLiftState(liftState state) {
	lift.setTargetPosition(liftPos[state]);
}

double liftHeight() {
	return LIFT_BASE_HEIGHT + LIFT_LEN * sin(lift.getPosition()) * (DR4B ? 2 : 1);
}
//#endregion

//#region chain bar
void setChainBarState(chainState state) {
	chainBar.setTargetPosition(chainPos[state]);
}
//#endregion
