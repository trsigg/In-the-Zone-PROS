#include "main.h"
#include "JINX.h"
#include "config.h"

extern "C" {
  void __libc_init_array();
}

void initializeIO() {
  __libc_init_array();
}

//#region global objects
  //#subregion drive
  unsigned char leftMotors[] = LD_MPS;
  unsigned char rightMotors[] = RD_MPS;
  ParallelDrive drive(sizeof(leftMotors), sizeof(rightMotors), leftMotors,
                      rightMotors, LEFT_ENC, RIGHT_ENC, WHEEL_DIAMETER);  //unsigned chars are 1 byte, so sizeof() == number of elements
  //gyro added in init
  //#endsubregion

  //#subregion lift
  unsigned char liftMotors[] = LIFT_MPS;
  MotorGroup lift(sizeof(liftMotors), liftMotors, L_POS_SENSOR);
  //PID initialized in init
  //#endsubregion

  //#subregion chain bar
  unsigned char chainMotors[] = CHAIN_MPS;
  MotorGroup chainBar(sizeof(chainMotors), chainMotors, CH_POS_SENSOR);
  //PID initialized in init
  //#endsubregion

  //#subregion cone intake
  unsigned char coneMotors[] = CONE_INTAKE_MPS;
  MotorGroup coneIntake(sizeof(coneMotors), coneMotors);
  //#endsubregion

  //#subregion mobile goal intake
  unsigned char goalMotors[] = GOAL_INTAKE_MPS;
  ButtonGroup goalIntake(GOAL_BTN_GROUP, sizeof(goalMotors), goalMotors,
                         GOAL_STILL_SPEED, GOAL_BTN_CONFIG);
  //sensor added in init
  //#endsubregion
//#endregion

void initialize() {
  //#region sensor config
  drive.addSensor(HYRO);
  goalIntake.addSensor(GOAL_POS_SENSOR);
  //#endregion
  //#region PID config
  lift.posPIDinit(0.3, 0.01, 0.05, 0);
  chainBar.posPIDinit(0.2, 0.001, 0.05, 0);
  //#endregion
  //#region JINX config
  if (TESTING > 0 || DEBUG_LEVEL > 0) {
    initJINX(uart1/*stdout*/);
    delay(100);
    taskCreate(JINXRun, TASK_DEFAULT_STACK_SIZE, NULL, (TASK_PRIORITY_DEFAULT));
  }
  //#endregion
}
