#include "motorGroup.h"		//also includes API
#include "coreIncludes.h"	//also includes cmath
#include "PID.h"
#include "Timer.h"

void MotorGroup::setPower(char power, bool overrideAbsolutes) {
	/*if (!overrideAbsolutes) {
		if (hasAbsMin && getPosition() <= absMin && power < -maxPowerAtAbs)
			power = -defPowerAtAbs;

		if (hasAbsMax && getPosition() >= absMax && power > maxPowerAtAbs)
			power = defPowerAtAbs;
	}*/

	for (unsigned char motor=0; motor<numMotors; motor++)
	  motorSet(motors[motor], power);
}

char MotorGroup::getPower() {
	return motorGet(motors[0]);	//return power of first motor in group
}

//#region constructors
MotorGroup::MotorGroup(unsigned char numMotors, unsigned char motors[]) : numMotors(numMotors), motors(motors) {
	maneuverTimer = new Timer;
}

MotorGroup::MotorGroup(unsigned char numMotors, unsigned char motors[], unsigned char encPort1, unsigned char encPort2, double coeff)
												: numMotors(numMotors), motors(motors), encCoeff(fabs(coeff)) {
	maneuverTimer = new Timer;
	encoder = encoderInit(encPort1, encPort2, coeff<0);
}

MotorGroup::MotorGroup(unsigned char numMotors, unsigned char motors[], unsigned char potPort, bool potReversed)
												: numMotors(numMotors), motors(motors), potPort(potPort), potReversed(potReversed) {
	maneuverTimer = new Timer;
}
//#endregion

//#region sensors
void MotorGroup::addSensor(unsigned char encPort1, unsigned char encPort2, double coeff, bool setAsDefault) {
	encoder = encoderInit(encPort1, encPort2, coeff<0);
	encCoeff = fabs(coeff);
	encoderReset(encoder);
	if (setAsDefault) potIsDefault = false;
}

void MotorGroup::addSensor(unsigned char potPort, bool reversed, bool setAsDefault) {
	this->potPort = potPort;
	potReversed = reversed;
	if (setAsDefault) potIsDefault = true;
}

int MotorGroup::encoderVal(bool rawValue) {
	if (hasEncoder()) {
		return encoderGet(encoder) * (rawValue ? 1 : encCoeff);
	}

	return 0;	//possible debug location
}

void MotorGroup::resetEncoder() {
	while (fabs(encoderVal()) > 0)	//I'm terrible
		encoderReset(encoder);
}	//possible debug location

int MotorGroup::potVal() {
	if (hasPotentiometer()) {
		return potReversed ? 4095-analogRead(potPort) : analogRead(potPort);
	} else {
		return 0;	//possible debug location
	}
}

int MotorGroup::getPosition() {
	if (hasPotentiometer() && hasEncoder()) {
		return potIsDefault ? potVal() : encoderVal();
	} else {
		return (hasEncoder() ? encoderVal() : potVal());
	}
}
//#endregion

//#region position movement
void MotorGroup::moveTowardPosition(int pos, char power) {
	return setPower(copysign(power, pos-getPosition()));
}

void MotorGroup::stopManeuver() {
	maneuverExecuting = false;
	setPower(0);
}

void MotorGroup::executeManeuver() {
	if (maneuverExecuting) {
		if (forward == (getPosition() < maneuverTarget)) {
			maneuverTimer->reset();
			setPower(maneuverPower);
		} else if (maneuverTimer->time() > maneuverTimeout) {
			maneuverExecuting = false;
			setPower(endPower);
		}
	}
}

void MotorGroup::goToPosition(int pos, bool runAsManeuver, char endPower, char maneuverPower, unsigned short timeout) {
	maneuverTarget = pos;
	endPower = endPower;
	forward = maneuverTarget > getPosition();
	maneuverPower = copysign(maneuverPower, (forward ? 1 : -1));
	maneuverExecuting = true;
	maneuverTimeout = timeout;
	maneuverTimer->reset();

	if (!runAsManeuver) {
		while (maneuverExecuting) executeManeuver();
	}
}

	//#subregion position targeting
void MotorGroup::posPIDinit(double kP, double kI, double kD, unsigned short minSampleTime, double integralMax, bool useTimeAdjustment) {
	posPID = new PID(0, kP, kI, kD, minSampleTime, integralMax, useTimeAdjustment);
}

void MotorGroup::setTargetPosition(int position) {
	posPID->changeTarget(position);
	targetingActive = true;
}

void MotorGroup::maintainTargetPos() {
	if (targetingActive && posPID) {
		setPower(posPID->evaluate(getPosition()));
	}
}

bool MotorGroup::errorLessThan(int margin) {
	return abs(posPID->getTarget() - getPosition()) < margin;
}
	//#endsubregion
//#endregion

//#region accessors and mutators
	//#subregion sensors
bool MotorGroup::isPotReversed() { return potReversed; }
void MotorGroup::setPotReversed(bool reversed) { potReversed = reversed; }
bool MotorGroup::isPotDefault() { return potIsDefault; }
void MotorGroup::setDefaultSensor(bool potIsDefault) { this->potIsDefault = potIsDefault; }
unsigned char MotorGroup::getPotPort() { return potPort; }
bool MotorGroup::hasEncoder() { return encoder; }  //TODO: verify that this works
bool MotorGroup::hasPotentiometer() { return potPort; }
	//#endsubregion
	//#subregion automovement
bool MotorGroup::isManeuverExecuting() { return maneuverExecuting; }
void MotorGroup::activatePositionTargeting() { targetingActive = true; }
void MotorGroup::deactivatePositionTargeting() { targetingActive = false; }
	//#endsubregion
	//#subregion position limits
void MotorGroup::setAbsMin(int min, char defPowerAtAbs, char maxPowerAtAbs) {
	absMin = min;
	hasAbsMin = true;
	this->maxPowerAtAbs = maxPowerAtAbs;
	this->defPowerAtAbs = defPowerAtAbs;
}
void MotorGroup::setAbsMax(int max, char defPowerAtAbs, char maxPowerAtAbs) {
	absMax = max;
	hasAbsMax = true;
	this->maxPowerAtAbs = maxPowerAtAbs;
	this->defPowerAtAbs = defPowerAtAbs;
}
void MotorGroup::setAbsolutes(int min, int max, char defPowerAtAbs, char maxPowerAtAbs) {
	absMin = min;
	absMax = max;
	hasAbsMin = true;
	hasAbsMax = true;
	this->maxPowerAtAbs = maxPowerAtAbs;
	this->defPowerAtAbs = defPowerAtAbs;
}
int MotorGroup::getAbsMin() { return absMin; }
int MotorGroup::getAbsMax() { return absMax; }
char MotorGroup::getDefPowerAtAbs() { return defPowerAtAbs; }
void MotorGroup::setDefPowerAtAbs(char power) { defPowerAtAbs = power; }
char MotorGroup::getMaxPowerAtAbs() { return maxPowerAtAbs; }
void MotorGroup::setMaxPowerAtAbs(char power) { maxPowerAtAbs = power; }
	//#endsubregion
//#endregion
