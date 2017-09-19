#include "JINX.h"
#include "config.h"
#include "buttonTracker.h"


//#region controls
#define QUIT_BTN 1, 8, JOY_UP
  //#subregion testSensors
#define AUTO_UPDATE_BTN 1, 5, JOY_UP
#define MANUAL_UPDATE_BTN 1, 6, JOY_UP
  //#endsubregion
  //#subregion testDerpBot
#define STORE_POS_BTN 7, JOY_UP
#define MAINTAIN_POS_BTN 1, 7, JOY_DOWN
  //#endsubregion
//#endregion

void printSensorVals() {
  printf("Gyro: %.f\n"
         "EncL: %f\n"
         "EncR: %f\n"
         "Lift: %d\n"
         "Chain: %d\n", drive.gyroVal(), drive.encoderVal(LEFT),
                        drive.encoderVal(RIGHT), lift.getPosition(),
                        chainBar.getPosition());
}

void testSensors() {
  while (!joystickGetDigital(QUIT_BTN)) {
    while (!(joystickGetDigital(MANUAL_UPDATE_BTN) || joystickGetDigital(QUIT_BTN))) {
      printSensorVals();
      delay(2000);
    }
    while (!(joystickGetDigital(AUTO_UPDATE_BTN) || joystickGetDigital(QUIT_BTN))) {
      if (joystickGetDigital(MANUAL_UPDATE_BTN)) {
        printSensorVals();
        while (joystickGetDigital(MANUAL_UPDATE_BTN)) delay(5);
      }
    }
  }
}

void testDerpBot(int testLevel) {
  unsigned char lMotors[] = { 10 };
  unsigned char rMotors[] = { 1 };
  unsigned char flMotors[] = { 2 };
  ParallelDrive derpDrive(1, 1, lMotors, rMotors);
  MotorGroup flapper(1, flMotors, 1);

  if (testLevel == 2) { //test flapper PID positioning
    int prevPos = 2000;

  	while (!joystickGetDigital(QUIT_BTN)) {
  		//flapper.takeInput();
  		flapper.maintainTargetPos();
  		derpDrive.takeInput();

  		if (ButtonTracker::newlyPressed(STORE_POS_BTN)) {
        drive.setDrivePower(127, 127);
        delay(100);
        drive.setDrivePower(0, 0);

  			prevPos = flapper.getPosition();
  			printf("%d", prevPos);
  		}

  		if (joystickGetDigital(MAINTAIN_POS_BTN))
  			flapper.setTargetPosition(prevPos);
  	}
  }
  else {  //test sensors
    char potVal[10];  //shouldn't be bigger than 4, but whatever

    while (!joystickGetDigital(QUIT_BTN)) {
      sprintf(potVal, "%d", flapper.getPosition());
      writeJINXData("potVal", potVal);
      delay(1000);
    }
  }
}

void handleTesting() {
  if (TESTING == 1)
      testSensors();
  if (TESTING <= 3)  //TESTING is 2 or 3
      testDerpBot(TESTING);
}
