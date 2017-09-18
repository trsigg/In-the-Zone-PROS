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
  #define TESTING 0 //0 for normal behavior, 1 for sensor testing, 2 for test bot
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
  #define L_POS_SENSOR  2, true
  //dimensions
  #define LIFT_LEN 15
  //positions
  enum liftState { L_DEF, L_ZERO, PRELOAD, M_BASE_POS, S_BASE_POS };
  #define L_POS  { 3450,  2000,   2450,    3450,       2000 }
  //#endsubregion
  //#subregion chain bar
  //motors
  #define CHAIN_MPS { 6 }
  //sensors
  #define CH_POS_SENSOR 6, 5, false
  //positions
  enum chainState { CH_DEF,	INTAKE, SAFE, STACK };	//when chain bar is SAFE, lift can move up and down without colliding with cone stack
  #define CH_POS  { 150,     71,     340,  500 }
  //#endsubregion
  //#subregion cone intake
  //motors
  #define CONE_INTAKE_MPS { 7 }
  //motor powers
  #define CONE_STILL_SPEED  10
  //#endsubregion
  //#subregion mobile goal intake
  //motors
  #define GOAL_INTAKE_MPS { 3, 8 }
  //sensors
  #define GOAL_POS_SENSOR 3
  //input
  #define GOAL_BTN_GROUP    7
  #define GOAL_BTN_CONFIG   5
  #define GOAL_STILL_SPEED  0
  //#endsubregion
  //#subregion autostacking controls
  #define STACK_BTN 1, 6, JOY_UP
  //#endsubregion
  //#subregion measurements
  #define CONE_HEIGHT 1  //amount of stack height increase per cone
  #define RAD_TO_POT 880.1
  #define LIFT_BASE_HEIGHT 0
  //#endsubregion
  //#subregion timing
  #define LIFT_SAMPLE_TIME 30 //sample time for lift PIDs
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
//#region X team
#ifdef X_TEAM
  //#subregion lift
  //motors
  #define LIFT_MPS  { 6, 4 }
  //sensors
  #define L_POS_SENSOR  1
  //positons
             // { L_DEF, L_ZERO, PRELOAD, M_BASE_POS, S_BASE_POS };
  #define L_POS { 3450,  2000,   2450,    3450,       2000 };
  //#endsubregion
  //#subregion chain bar
  //motors
  #define CHAIN_MPS { 5 }
  //sensors
  #define CH_POS_SENSOR 8
  //positions
              // { CH_DEF, INTAKE, SAFE, STACK };	//when chain bar is SAFE, lift can move up and down without colliding with cone stack
  #define CH_POS  { 150,     71,     340,  500 };
  //#endsubregion
#endif
//#endregion

#endif
