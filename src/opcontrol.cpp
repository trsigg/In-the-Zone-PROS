#include "main.h"
#include "autostack.h"


void operatorControl() {
  startStackingTasks();

	while (true) {
		if (!stacking && joystickGetDigital(FRONT_STACK_BTN)) {
			stackNewCone();
      printf("wtf");
    }

    if (joystickGetDigital(1, 5, JOY_UP)) {
      chainBar.resetEncoder();
      printf("Enc reset");
    }

		drive.takeInput();
    goalIntake.takeInput();
	}
}
