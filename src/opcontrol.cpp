#include "main.h"
#include "config.h"
#include "liftControl.h"
#include "testing.h"


void operatorControl() {
  handleTesting();

  startStackingTasks();

	while (true) {
		if (!stacking && joystickGetDigital(STACK_BTN)) {
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
