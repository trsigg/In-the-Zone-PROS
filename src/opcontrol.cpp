#include "main.h"
#include "autostack.h"


void operatorControl() {
  startStackingTasks();

	while (true) {
		if (!stacking && joystickGetDigital(FRONT_STACK_BTN))
			stackNewCone();

		drive.takeInput();
    goalIntake.takeInput();
	}
}
