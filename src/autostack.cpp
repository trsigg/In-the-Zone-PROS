#include <API.h>
#include "motorGroupControl.h"
#include "motorGroup.h"

int numCones = 0; //current number of stacked cones
bool stacking = false;	//whether the robot is currently in the process of stacking
double liftAngle1;	//the target angles of lift sections during a stack maneuver
double liftAngle2;
double chainAngle;


double liftStackPositions[MAX_NUM_CONES] = L_STACK_POS;
double liftStackOffsets[MAX_NUM_CONES] = L_STACK_OFF;	//offsets from stack positions
double chainPositions[MAX_NUM_CONES] = CH_STACK_POS;


void stackNewCone() {
	if (numCones < MAX_NUM_CONES) {
		#ifdef MATH	//TODO: account for limited range of motion
		double y = CONE_HEIGHT * numCones - LIFT_BASE_HEIGHT;
		double x = STACK_X_POS;

		if (DR4B) {
			chainAngle = acos(x / CHAIN_LEN);
			liftAngle1 = asin((y - CHAIN_LEN * sin(chainAngle)) / LIFT_LEN / 2);
		}
		else {
			double sqrLen1 = pow(LIFT_LEN, 2);
			double sqrLen2 = pow(CHAIN_LEN, 2);
			double a = pow(x, 2) + pow(y, 2);
			double b = sqrLen1 - sqrLen2;
			double c = sqrLen1 * sqrt(2 * (a + sqrLen2) - pow(a - sqrLen2, 2) - sqrLen1);

			chainAngle = atan((y * (a - b) - x * c) / (x * (a - b) + y * c));
			liftAngle2 = acos((CHAIN_LEN * cos(chainAngle) - x) / LIFT_LEN);
		}

		liftAngle1 = liftAngle2 + L_DEF_OFF;
		#else
		liftAngle2 = liftStackPositions[numCones];
		liftAngle1 = liftAngle2 + liftStackOffsets[numCones];
		chainAngle = chainPositions[numCones];
		#endif

		stacking = true;
	}
}

void liftManeuversTask(void* ignore) {
	while (true) {
		chainBar.maintainTargetPos();
		lift.maintainTargetPos();
		delay(LIFT_SAMPLE_TIME);
	}
}

void autoStackingTask(void* ignore) {
	while (true) {
		while (!stacking) delay(5);

		//intake cone
		setChainBarState(CH_INTAKE)
		coneIntake.setPower(127);
		delay(INTAKE_DURATION);
		coneIntake.setPower(CONE_STILL_SPEED);

		//move to desired location
		chainBar.setTargetPosition(chainAngle);
		lift.setTargetPosition(liftAngle1);
		waitForMovementToFinish();

		lift.setTargetPosition(liftAngle2);
		waitForMovementToFinish();

		//expel cone
		coneIntake.setPower(-127);
		delay(INTAKE_DURATION);
		coneIntake.setPower(0);

		//return to ready positions
		setChainBarState(CH_DEF);
		setLiftState(L_BOTTOM);
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
