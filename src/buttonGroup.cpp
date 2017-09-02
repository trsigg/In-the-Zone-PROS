#include "buttonGroup.h"
#include "buttonTracker.h"
#include <cmath>
#include <API.h>

char ButtonGroup::takeInput() {
  char power = 0;

  if (active) {
    if (joystickGetDigital(upJoystick, upGroup, upButton))
      power = upPower;
    else if (joystickGetDigital(downJoystick, downGroup, downButton))
      power = downPower;
    else
      power = stillSpeed;
  }

  setPower(power);
  return power;
}

void ButtonGroup::configureInput(unsigned char buttonGroup, char stillSpeed, unsigned char buttonConfig, char power, char downPower, unsigned char joystick) {
  active = true;

  upGroup = buttonGroup;
  downGroup = buttonGroup;
  upJoystick = joystick;
  downJoystick = joystick;

  if (buttonConfig<=5 && buttonGroup>6) {
    unsigned char buttonArray[6][2] = {{JOY_UP,    JOY_DOWN}, //0
                                      {JOY_LEFT,  JOY_RIGHT}, //1
                                      {JOY_UP,    JOY_LEFT},  //2
                                      {JOY_UP,    JOY_RIGHT}, //3
                                      {JOY_LEFT,  JOY_DOWN},  //4
                                      {JOY_RIGHT, JOY_DOWN}}; //5

    upButton = buttonArray[buttonConfig][0];
    downButton = buttonArray[buttonConfig][0];
  } else {  //possible debug location
    upButton = JOY_UP;
    downButton = JOY_DOWN;
  }

  if (!(ButtonTracker::isValidButton(upGroup, upButton, upJoystick)
        && ButtonTracker::isValidButton(downGroup, downButton, downJoystick)))
        active = false;

  this->stillSpeed = stillSpeed;
  setMovementPower(power, downPower);
}

void ButtonGroup::configureMultiGroupInput(unsigned char upGroup, unsigned char upButton, unsigned char downGroup, unsigned char downButton, char stillSpeed, char power, char downPower, unsigned char upJoystick, unsigned char downJoystick) {
  active = true;

  this->upJoystick = upJoystick;
  this->upGroup = upGroup;
  this->upButton = upButton;

  this->downJoystick = downJoystick;
  this->downGroup = downGroup;
  this->downButton = downButton;

  if (!(ButtonTracker::isValidButton(upGroup, upButton, upJoystick)
        && ButtonTracker::isValidButton(downGroup, downButton, downJoystick)))
        active = false;

  this->stillSpeed = stillSpeed;
  setMovementPower(power, downPower);
}

//#region constructors
/*ButtonGroup::ButtonGroup(unsigned char numMotors, unsigned char motors[]) : MotorGroup(numMotors, motors) {
  active = false;
}*/

ButtonGroup::ButtonGroup(unsigned char buttonGroup, unsigned char numMotors, unsigned char motors[], char stillSpeed, unsigned char buttonConfig, char power, char downPower, unsigned char joystick) : MotorGroup(numMotors, motors) {
  configureInput(buttonGroup, stillSpeed, buttonConfig, power, downPower, joystick);
}

ButtonGroup::ButtonGroup(unsigned char numMotors, unsigned char motors[], unsigned char encPort1, unsigned char encPort2, double coeff) : MotorGroup(numMotors, motors, encPort1, encPort2, coeff) {
  active = false;
}

ButtonGroup::ButtonGroup(unsigned char numMotors, unsigned char motors[], unsigned char potPort, bool potReversed) : MotorGroup(numMotors, motors, potPort, potReversed) {
  active = false;
}
//#endregion

//#region accessors and mutators
  //#subregion state
bool ButtonGroup::isActive() { return active; }
void ButtonGroup::activate() { active = true; }
void ButtonGroup::deactivate() { active = false; }
  //#endsubregion
  //#subregion motor powers
char ButtonGroup::getStillSpeed() { return stillSpeed; }
char ButtonGroup::getUpPower() { return upPower; }
char ButtonGroup::getDownPower() { return downPower; }
void ButtonGroup::setStillSpeed(char power) { stillSpeed = power; }
void ButtonGroup::setMovementPower(char power, char downPower) {
  upPower = power;

  if (downPower == 0)
    this->downPower = -upPower;
  else
    this->downPower = -copysign(downPower, upPower);
}
  //#endsubregion
//#endregion
