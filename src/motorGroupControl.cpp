#include "config.h"
#include "timer.h"


void waitForMovementToFinish(bool waitForChain=true, bool waitForLift=true, unsigned short timeout=75, double chainMargin=7, double liftMargin=10) {
	Timer movementTimer;

	while (movementTimer.time() < timeout) {
		if ((!chainBar.errorLessThan(chainMargin) && waitForChain) ||
				(!lift.errorLessThan(liftMargin) && waitForLift))
			movementTimer.reset();
		delay(5);
	}
}

//#region lift
double liftHeight() {
	return LIFT_BASE_HEIGHT + LIFT_LEN * sin(lift.getPosition()) * (DR4B ? 2 : 1);
}
//#endregion
