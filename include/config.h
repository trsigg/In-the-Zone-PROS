/* Global configuration variables */

#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#define E_TEAM

//#region defaults
#define NUM_JOYSTICKS 1
#define DEBUG_LEVEL 0 //0 is not debugging, higher levels yield increasingly detailed output (currently non-functional)
//#endregion


//#region default config
  //#subregion includes
  #include "parallelDrive.h"
  #include "buttonGroup.h"
  //#endsubregion
  //#subregion constants
  #define MAX_NUM_CONES 11
  //#endsubregion
  //#subregion config
  #define DR4B false	//true if using double reverse as primary lift
  #define TESTING 2 //0 for normal behavior, 1 for sensor testing, 2 for test bot
  //#define MATH  //defined if using mathematical model for autostacking (as opposed to hand-tuned ovement)
  //#endsubregion
  //#subregion drive
  //motors (mps = motor ports)
  #define LD_MPS  { 9, 10 }   //left
  #define RD_MPS  { 1, 2 }  //right
  //sensors
  #define HYRO      1
  #define RIGHT_ENC 4, 3, false
  #define LEFT_ENC  2, 1, false
  //dimensions
  #define WHEEL_DIAMETER 4.0
  //#endsubregion
  //#subregion lift
  //motors
  #define LIFT_MPS  { 5 }
  //sensors
  #define LIFT_POT  2, true
  //dimensions
  #define LIFT_MIN_ANGLE  -42.0 //degrees
  #define LIFT_MAX_ANGLE  61.3
  #define LIFT_LEN        15.0  //inches
  //positions
  #define L_BOTTOM 3450
  #define L_PRELOAD 2450
  //autostacking offsets
  #define L_DEF_OFF 200
  #define L_STACK_POS { 2960, 3090, 3145, 2735, 2645, 2340, 2085, 1990, 2130, 1830, 1900 }
  #define L_STACK_OFF { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  //#endsubregion
  //#subregion chain bar
  //motors
  #define CHAIN_MPS { 6 }
  //sensors
  #define CHAIN_ENC 6, 5, false
  //dimensions
  #define CHAIN_MIN_ANGLE -53.8 //degrees
  #define CHAIN_MAX_ANGLE 71.3
  #define CHAIN_LEN       16.5  //inches
  //positions
  #define CH_DEF 150
  #define CH_INTAKE 71
  #define CH_VERT 340
  //autostacking offsets
  #define CH_STACK_POS { 694, 619, 563, 629, 624, 672, 709, 653, 600, 503, 537 }
  //#endsubregion
  //#subregion cone intake
  //motors
  #define CONE_INTAKE_MPS { 7 }
  //motor powers
  #define CONE_STILL_SPEED  10  //TODO: still speed
  //#endsubregion
  //#subregion mobile goal intake
  //motors
  #define GOAL_INTAKE_MPS { 3, 8 }
  //sensors
  #define GOAL_INTAKE_ENC 3
  //input
  #define GOAL_BTN_GROUP    7
  #define GOAL_BTN_CONFIG   5
  #define GOAL_STILL_SPEED  0
  //#endsubregion
  //#subregion autostacking controls
  #define FRONT_STACK_BTN 1, 6, JOY_UP
  #define BACK_STACK_BTN  1, 6, JOY_DOWN
  //#endsubregion
  //#subregion measurements
  #define CONE_HEIGHT 1  //amount of stack height increase per cone
  #define LIFT_BASE_HEIGHT 22	//height of end actuator when all linkage joints are at angle 0 (above top of mobile goal)
  #define STACK_X_POS -5	//horizontal distance of cone stack from base of lift
  #define STACK_OFFSET 7	//distance above stack cone must be to be added (because of height of cones, etc.)
  #define CHAIN_BAR_OFFSET -1	//offset from the top of the cone stack when chain bar starts rotating from angle 0 to its final position (to avoid collision)
  #define LIFT_OFFSET 10 //amount lift overshoots before coming down to place cone
  //#endsubregion
  //#subregion timing
  #define LIFT_SAMPLE_TIME 30 //sample time for lift PID's
  #define INTAKE_DURATION 300	//amount of time rollers activate when intaking/expelling
  //#endsubregion
  //#subregion global externs
  extern ParallelDrive drive; //TODO: move?
  extern MotorGroup lift;
  extern MotorGroup chainBar;
  extern MotorGroup coneIntake;
  extern ButtonGroup goalIntake;
  //#endsubregion
//#endregion

#endif
