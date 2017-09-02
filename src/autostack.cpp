#include "motorGroupControl.h"
#include "motorGroup.h"

#define INTAKE_DURATION 300	//amount of time rollers activate when intaking/expelling

int numCones = 0; //current number of stacked cones
bool stacking = false;	//whether the robot is currently in the process of stacking
double liftAngle;	//the target angles of lift sections during a stack mane
double chainAngle;


void stackNewCone() {	//TODO: account for limited range of motion
	double adjustedHeight = CONE_HEIGHT * numCones - LIFT_BASE_HEIGHT;

	if (DR4B) {
		chainAngle = acos(STACK_X_POS / CHAIN_LEN);
		liftAngle = asin((adjustedHeight - CHAIN_LEN * sin(chainAngle)) / LIFT_LEN / 2);
	}
	else {
		double sqrLen1 = pow(LIFT_LEN, 2);
		double sqrLen2 = pow(CHAIN_LEN, 2);
		double a = pow(STACK_X_POS, 2) + pow(adjustedHeight, 2);
		double b = a + sqrLen1 - sqrLen2;
		double r = sqrt(2 * sqrLen1 * (sqrLen2 + b) - pow(LIFT_LEN, 4) - pow(b - sqrLen2, 2));

		liftAngle = atan(adjustedHeight * (adjustedHeight * a + STACK_X_POS * r) /
														(STACK_X_POS * a + adjustedHeight * r));
		chainAngle = acos((STACK_X_POS - LIFT_LEN * cos(liftAngle)) / CHAIN_LEN);
	}

	stacking = true;
}

void liftManeuvers() {	//TODO: TASKIFY
	while (true) {
		chainBar.maintainTargetPos();
		lift.maintainTargetPos();
	}
}

void autoStacking() {	//TODO: taskify
	while (true) {
		while (!stacking);

		//intake cone
		coneIntake.setPower(127);
		delay(INTAKE_DURATION);
		coneIntake.setPower(CONE_STILL_SPEED);

		//move to desired location
		setChainBarState(SAFE);
		lift.setTargetPosition(liftAngle);

		while (liftHeight() < numCones * CONE_HEIGHT + CHAIN_BAR_OFFSET);
		chainBar.setTargetPosition(chainAngle);

		waitForMovementToFinish();

		//expel cone
		coneIntake.setPower(-127);
		delay(INTAKE_DURATION);
		coneIntake.setPower(0);

		//return to ready positions
		setChainBarState(SAFE);
		waitForMovementToFinish(true, false);

		setLiftState(L_DEF);
		waitForMovementToFinish(false);

		setChainBarState(CH_DEF);
	}
}
