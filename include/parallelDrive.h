/* General class for modeling a parallel drive system

  Motors modeled by two JoystickGroups. Supports taking user input, managing and
  accessing JoystickGroup sensors as well as a gyroscope, position tracking, and
  autonomous movement. */

#ifndef PARALLEL_DRIVE_INCLUDED
#define PARALLEL_DRIVE_INCLUDED

#include "coreIncludes.h" //also includes cmath
#include <API.h>

class JoystickGroup;
class PID;
class Ramper;
class Timer;

//#region enums
enum encoderConfig { UNASSIGNED, LEFT, RIGHT, AVERAGE };
/* How encoderVal() (which is used in many internal functions in addition to
    being accessible to user) is calculated by default.

    LEFT and RIGHT only use values from one side of the drive, AVERAGE uses
    their mean. */
enum gyroCorrectionType { NO, MEDIUM, FULL };
/* How gyro is used during position tracking.

    NO does not use gyro. MEDIUM (currently most reliable) uses gyro to track
    orientation only. FULL attempts to correct encoder values based on gyro input. */
enum correctionType { NONE, GYRO, ENCODER, AUTO };
/* How correction is performed during automovement. GYRO tries to maintain a
    gyro value of 0, ENCODER tries to maintain an difference in the drive side
    encoder counts of zero, and AUTO will cause the program to choose
    automatically based on the available sensors (encoders given preference) */
//#endregion

//#region defaults
struct TurnDefaults {
  angleType defAngleType;
  bool useGyro;
  char brakePower;
  unsigned short waitAtEnd, sampleTime, brakeDuration;
  double rampConst1, rampConst2, rampConst3, rampConst4, rampConst5;  // initialPower/kP, maxPower/kD, finalPower/error, 0/maneuver timeout, irrelevant/kI
};
extern TurnDefaults tDefs;

struct DriveDefaults {
  correctionType defCorrectionType;
  bool rawValue;            //whether to use encoder clicks (as opposed to inches)
  char brakePower;
  unsigned short waitAtEnd, sampleTime, brakeDuration, moveTimeout;
  double rampConst1, rampConst2, rampConst3, rampConst4, rampConst5; //same as turn
  double kP_c, kI_c, kD_c;  //correction PID constants
  double minSpeed;  //minimum speed (inches or clicks per second) which will not trigger a move timeout
};
extern DriveDefaults dDefs;
//#endregion

class ParallelDrive {
  public:
    //#region main methods
    void takeInput();
    void setLeftPower(char power);
    void setRightPower(char power);
    void setDrivePower(char left, char right);
    //#endregion

    //#region constructors
    ParallelDrive(unsigned char numMotorsL, unsigned char numMotorsR, unsigned char leftMotors[], unsigned char rightMotors[], unsigned char lEncPort1, unsigned char lEncPort2, bool lReversed, unsigned char rEncPort1, unsigned char rEncPort2, bool rReversed, double wheelDiameter, double gearRatio=1);
    ParallelDrive(unsigned char numMotorsL, unsigned char numMotorsR, unsigned char leftMotors[], unsigned char rightMotors[], double coeff=1, double powMap=1, unsigned char maxAcc100ms=0, unsigned char deadband=10, unsigned char leftAxis=3, unsigned char rightAxis=2, unsigned char joystick=1); //configures tank input
    ParallelDrive(unsigned char movementAxis, unsigned char turningAxis, unsigned char numMotorsL, unsigned char numMotorsR, unsigned char leftMotors[], unsigned char rightMotors[], double coeff=1);  //configures arcade input
    //#endregion

    //#region input config
    void configureTankInput(double coeff=1, double powMap=1, unsigned char maxAcc100ms=0, unsigned char deadband=10, unsigned char leftAxis=3, unsigned char rightAxis=2, unsigned char joystick=1);
    void configureArcadeInput(unsigned char movementAxis=1, unsigned char turningAxis=2, double coeff=1);
    //#endregion
    //#region sensors
    void addSensor(unsigned char encPort1, unsigned char encPort2, bool reversed, encoderConfig side, double wheelDiameter=0, double gearRatio=1); //encCoeff calculated from diameter and gear ratio (from wheel to encoder)
    void addSensor(unsigned char gyroPort, gyroCorrectionType correction=MEDIUM, unsigned short multiplier=0);
    double encoderVal(encoderConfig side=UNASSIGNED, bool rawValue=false, bool absolute=true);
    /* Returns the result of calling encoderVal() on motor group of specified
        side. When side is UNASSIGNED, encConfig is used to determine which
        encoders to use. AVERAGE returns the mean of the two sides' values, or
        that of their absolute values if absolute is true. */
    void resetEncoders();                       //When side is UNASSIGNED, encConfig is used to determine which encoder to reset
    double gyroVal(angleType format=DEGREES);
    void resetGyro();
    double absAngle(angleType format=DEGREES);  //gyroVal() + angleOffset
    //#endregion
    //#region position tracking
    void updatePosition();  //takes encoder (and possibly gyro) input and updates robot's current position
    double calculateWidth(unsigned short duration=10000, unsigned short sampleTime=200, char power=80, unsigned short reverseDelay=750);
    /* Causes robot to spin and uses gyro and encoder input to calculate width
        of its drive, which is returned and automatically set. Power is the
        motor power used in turning, and reverse delay is the amount of time for
        which samples are not taken as drive changes spinning direction. */
    //#endregion
    //#region automovement
    void turn(double angle, bool runAsManeuver=false, double rc1=tDefs.rampConst1, double rc2=tDefs.rampConst2, double rc3=tDefs.rampConst3, double rc4=tDefs.rampConst4, double rc5=tDefs.rampConst5, angleType format=tDefs.defAngleType, unsigned short waitAtEnd=tDefs.waitAtEnd, unsigned short sampleTime=tDefs.sampleTime, char brakePower=tDefs.brakePower, unsigned short brakeDuration=tDefs.brakeDuration, bool useGyro=tDefs.useGyro);
    void drive(double dist, bool runAsManeuver=false, double rc1=dDefs.rampConst1, double rc2=dDefs.rampConst2, double rc3=dDefs.rampConst3, double rc4=dDefs.rampConst4, double rc5=dDefs.rampConst5, unsigned short waitAtEnd=dDefs.waitAtEnd, double kP=dDefs.kP_c, double kI=dDefs.kI_c, double kD=dDefs.kD_c, correctionType correction=dDefs.defCorrectionType, bool rawValue=dDefs.rawValue, double minSpeed=dDefs.minSpeed, unsigned short moveTimeout=dDefs.moveTimeout, char brakePower=dDefs.brakePower, unsigned short brakeDuration=dDefs.brakeDuration, unsigned short sampleTime=dDefs.sampleTime);
    /* rc args explained in turnDefaults definition */
    void executeManeuver();                             //executes turn and drive maneuvers
    double maneuverProgress(angleType format=DEGREES);  //returns absolute value odistance traveled or angle turned while maneuver is in progress
    bool maneuverExecuting();
    //#endregion
    //#region accessors and mutators
      //#subregion sensors
    void setEncoderConfig(encoderConfig config);
    void setAbsAngle(double angle=0, angleType format=DEGREES); //sets angleOffset so that current absAngle is equal to specified angle
    bool hasGyro();
      //#endsubregion
      //#subregion position tracking
    void setWidth(double inches);
    void setRobotPosition(double x, double y, double theta, angleType format=DEGREES, bool updateAngleOffset=true); //sets angleOffset so that current absAngle is equal to theta if setAbsAngle if true
    double x(); double y(); double theta(angleType format=DEGREES);
      //#endsubregion
      //#subregion autonomous
    void setCorrectionType(correctionType type);
    void setTurnReversal(bool reversed);
      //#endsubregion
    //#endregion
  private:
    JoystickGroup* leftDrive;
    JoystickGroup* rightDrive;
    //#region arcade
    bool arcadeInput;
    double coeff;
    unsigned char moveAxis, turnAxis, joystick;
    //#endregion
    //#region sensors
    double wheelDiameter; //used for calculating encoder coefficients
    void updateEncConfig(); //automatically updates encConfig when a new encoder is attached
    encoderConfig encConfig;
    Gyro gyro;
    int angleOffset;  //amount added to gyro values to obtain absolute angle (degrees)
    //#endregion
    //#region position tracking
    double xPos, yPos, orientation; //orientation is in radians
    double width;                   //width of drive in inches (wheel well to wheel well)
    Timer* positionTimer;
    unsigned short minSampleTime; //minimum time between updates of robot's position
    gyroCorrectionType gyroCorrection;
    //#endregion
    //#region automovement
    void initializeDefaults();  //initializes default automovement values (called by constructors)
    double target;  //angle or distance
    Ramper* ramp;    //controls motor power ramping during maneuver
    unsigned short finalDelay, sampleTime, brakeDelay;
    char brakePower;
      //#subregion termination conditions
    bool maneuverFinished();
    /* Checks if termination conditions are met based on current robot state.
        Not to be confused with maneuverExecuting(), which simply checks
        isTurning and isDriving. */
    bool quadRamping; //if this is true, maneuver will terminate once progress surpasses <target>
                      //if it is false, maneuver will terminate once <maneuverTimer> surpasses <timeout>
    Timer* maneuverTimer; //tracks how long robot has been within <margin> of <target>
    unsigned short timeout;
    double margin;
      //#endsubregion
      //#subregion turning
    bool isTurning;
    angleType format;
    bool reverseTurns;
    bool usingGyro;
      //#endsubregion
      //#subregion driving
    bool isDriving;
    bool rawValue;
    double minSpeed;
    unsigned short moveTimeout; //amount of time with no movement after which a maneuver will terminate
    correctionType correction;
    PID* correctionPID;
    double leftDist, rightDist, totalDist;
    Timer* sampleTimer;
    Timer* moveTimer;
      //#endsubregion
    //#endregion
};

#endif
