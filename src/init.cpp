#include "main.h"
#include "config.h"

extern "C" {
  void __libc_init_array();
}

void initializeIO() {
  __libc_init_array();
}

void initialize() {
  //#region sensor config
  drive.addSensor(HYRO);
  goalIntake.addSensor(GOAL_INTAKE_ENC);
  //#endregion
  //#region PID config
  lift.posPIDinit(0.3, 0.01, 0.05);
  chainBar.posPIDinit(0.2, 0.001, 0.05);
  //#endregion
}
