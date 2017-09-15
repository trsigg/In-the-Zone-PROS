#include <API.h>
#include "config.h"
#include "motorGroup.h"
#include "timer.h"


int liftPos[] = L_POS;
int chainPos[] = CH_POS;


double liftHeight() {
	return LIFT_BASE_HEIGHT + 2 * LIFT_LEN * sin(lift.getPosition() / RAD_TO_POT);
}

void setLiftState(liftState state) {
	lift.setTargetPosition(liftPos[state]);
}

void setChainState(chainState state) {
	chainBar.setTargetPosition(chainPos[state]);
}

void waitForMovementToFinish(bool waitForChain=true, bool waitForLift=true, unsigned short timeout=75, double chainMargin=7, double liftMargin=10) {
	Timer movementTimer;

	while (movementTimer.time() < timeout) {
		if ((!chainBar.errorLessThan(chainMargin) && waitForChain) ||
				(!lift.errorLessThan(liftMargin) && waitForLift))
			movementTimer.reset();
		delay(5);
	}
}


//#region stacking
int numCones = 0; //current number of stacked cones
bool stacking = false;	//whether the robot is currently in the process of stacking
double liftTarget;


void stackNewCone() {
	if (numCones < MAX_NUM_CONES) {
		liftTarget = RAD_TO_POT * asin((CONE_HEIGHT * numCones + LIFT_BASE_HEIGHT) / 2 / LIFT_LEN) + liftPos[L_ZERO];
		stacking = true;
	}
}

void liftManeuversTask(void* ignore) {
	while (true) {
		chainBar.maintainTargetPos();
		lift.maintainTargetPos();
		/* printf("CB: %d", chainBar.getPosition());
		printf("LI: %d", lift.getPosition()); */
		delay(LIFT_SAMPLE_TIME);
	}
}

void autoStackingTask(void* ignore) {
	while (true) {
		while (!stacking) delay(5);

		//intake cone
		setChainState(INTAKE);
		coneIntake.setPower(127);
		//while (!chainBar.errorLessThan(5)) delay(5);
		delay(INTAKE_DURATION);
		coneIntake.setPower(CONE_STILL_SPEED);

		//move lift to desired location
		chainBar.setTargetPosition(SAFE);
		lift.setTargetPosition(liftTarget);
		waitForMovementToFinish(false);

		//move chain bar to desired location
		setChainState(STACK);
		waitForMovementToFinish();

		//expel cone
		coneIntake.setPower(-127);
		chainBar.setTargetPosition(CH_DEF);
		delay(INTAKE_DURATION);
		coneIntake.setPower(0);
		numCones++;

		while (chainBar.getPosition() > chainPos[SAFE]) delay(5);	//wait for chain bar to clear stack

		//lift down
		setLiftState(L_DEF);
	}
}

TaskHandle liftManeuvers;
TaskHandle autoStacking;

void startStackingTasks() {
	liftManeuvers = taskCreate(liftManeuversTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
	autoStacking = taskCreate(autoStackingTask, TASK_DEFAULT_STACK_SIZE, NULL, TASK_PRIORITY_DEFAULT);
}

void stopStackingTasks() {
	taskDelete(liftManeuvers);
	taskDelete(autoStacking);
}
//#endregion
