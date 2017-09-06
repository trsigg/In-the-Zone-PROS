#include "config.h"

//#region drive
unsigned char leftMotors[] = LD_MPS;
unsigned char rightMotors[] = RD_MPS;
ParallelDrive drive(sizeof(leftMotors), sizeof(rightMotors), leftMotors,
                    rightMotors, LEFT_ENC, RIGHT_ENC, WHEEL_DIAMETER);  //unsigned chars are 1 byte, so sizeof() == number of elements
//#endregion

//#region lift
unsigned char liftMotors[] = LIFT_MPS;
MotorGroup lift(sizeof(liftMotors), liftMotors, LIFT_POT);
//#endregion

//#region chain bar
unsigned char chainMotors[] = CHAIN_MPS;
MotorGroup chainBar(sizeof(chainMotors), chainMotors, CHAIN_ENC);
//#endregion

//#region cone intake
unsigned char coneMotors[] = CONE_INTAKE_MPS;
MotorGroup coneIntake(sizeof(coneMotors), coneMotors);
//#endregion

//#region mobile goal intake
unsigned char goalMotors[] = GOAL_INTAKE_MPS;
ButtonGroup goalIntake(GOAL_BTN_GROUP, sizeof(goalMotors), goalMotors,
                       GOAL_STILL_SPEED, GOAL_BTN_CONFIG);
//#endregion
