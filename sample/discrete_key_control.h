#pragma once
#include <stdlib.h>
#include <vector>
//#include "kinematics.h"
//#include "UderoCAN.h"
#include "udero/Udero.h"
#include "param_loader.h"
#include "control_parameter_holder.h"
#include "udero_controller.h"



enum CONTROL_MODE {
  CONTROL_START,
  CONTROL_CARTESIAN,
  CONTROL_JOINT, 
  CONTROL_UNKNOWN,
};


extern CONTROL_MODE controlMode;
extern MOVE_MODE moveMode;
extern MOVING_DIRECTION movingDirection;

//extern reharo::UderoCANConnectionProfile prof;
//extern reharo::IUdero* udero;
extern std::vector<double> jointPos;

//extern int keyPressed;
// extern int keyReleasedCount;
extern int keyReleasedCountThreshold;

struct KeyContext {
  int keyReleasedCount;
  int old_key;
  int new_key;
  KeyContext(): old_key(0), new_key(0), keyReleasedCount(0) {}
};

bool initKeyInput(ParamLoader* loader);
void finiKeyInput();
KeyContext checkKeyInput(const KeyContext& ctx);
MoveCommand keyHandler(const MoveCommand& cmd, const KeyContext& ctx);
//MoveCommand checkKey();

extern Matrix44 forwardKinematics;
extern Matrix44 forwardKinematicsWrist;
extern Matrix44 jointMatrixInKeyPressed;
extern Matrix44 wristMatrixInKeyPressed;


extern int update_display;
extern int sleep_time;

extern ParamLoader* loader;

void onEnterCartesianControlMode();
void onExitCartesianControlMode();
void onKeyPressedInCartesianMode(int key);
void onKeyReleasedInCartesianMode(int key);

void onEnterStartControlMode();
void onExitStartControlMode();
void onKeyPressedInStartMode(int key);
void onKeyReleasedInStartMode(int key);

void onEnterJointControlMode();
void onExitJointControlMode();
void onKeyPressedInJointMode(int key);
void onKeyReleasedInJointMode(int key);



extern bool exit_program;
