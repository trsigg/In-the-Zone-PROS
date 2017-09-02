/* Global configuration variables */

#ifndef CONFIG_INCLUDED
#define CONFIG_INCLUDED

#define E_TEAM

//#region defaults
#define NUM_JOYSTICKS 1
#define DEBUG_LEVEL 0 //0 is not debugging, higher levels yield increasingly detailed output (currently non-functional)
//#endregion


//#region E-Team
#ifdef E_TEAM
  //#subregion includes
  #include "parallelDrive.h"
  #include "buttonGroup.h"
  //#endsubregion
  //#subregion config
  #define DR4B false	//true if using double reverse as primary lift
  //#endsubregion
  //#subregion drive
  //motors (mps = motor ports)
  #define LD_MPS  { 1, 2 }   //left
  #define RD_MPS  { 9, 10 }  //right
  //sensors
  #define HYRO      1
  #define RIGHT_ENC 4, 3, false
  #define LEFT_ENC  2, 1, false
  //dimensions
  #define WHEEL_DIAMETER 4.0
  //#endsubregion
  //#subregion lift
  //motors
  #define LIFT_MPS  { 4, 5 }  //total guess
  //sensors
  #define LIFT_POT  2
  //dimensions
  #define LIFT_MIN_ANGLE  -42.0 //degrees
  #define LIFT_MAX_ANGLE  61.3
  #define LIFT_LEN        15.0  //inches
  //positions
  enum liftState		{ L_DEF };
  #define LIFT_POS  { -30 }
  //#endsubregion
  //#subregion chain bar
  //motors
  #define CHAIN_MPS { 3, 8 }  //guess
  //sensors
  #define CHAIN_ENC 6, 5, false
  //dimensions
  #define CHAIN_MIN_ANGLE -53.8 //degrees
  #define CHAIN_MAX_ANGLE 71.3
  #define CHAIN_LEN       16.5  //inches
  //positions
  enum chainState 	{ CH_DEF,	SAFE };	//when chain bar is SAFE, lift can move up and down without colliding with cone stack
  #define CHAIN_POS { -10,		0 }
  //#endsubregion
  //#subregion cone intake
  //motors
  #define CONE_INTAKE_MPS { 6 }
  //motor powers
  #define CONE_STILL_SPEED  10
  //#endsubregion
  //#subregion mobile goal intake
  //motors
  #define GOAL_INTAKE_MPS { 7 }  //total guess
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
  //#endsubregion
  //#subregion global externs
  extern ParallelDrive drive; //TODO: move?
  extern MotorGroup lift;
  extern MotorGroup chainBar;
  extern MotorGroup coneIntake;
  extern ButtonGroup goalIntake;
  //#endsubregion
#endif
//#endregion

#endif
