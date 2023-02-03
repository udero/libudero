#include "SerialPort.h"
#include <iostream>
#include "Thread.h"

#include "udero/UderoLogger.h"
#include "udero/kinematics.h"
#include "param_loader.h"
#include <ctime>


#include <stdarg.h>

const char* matrix_port = "/dev/ttymxc4";

#define _USE_MATH_DEFINES
#include <math.h>

#if WIN32
#include <conio.h>
#endif
#include "logger.h"
#include "discrete_key_control.h"
#include "devices/ledmatrix.h"

#ifdef WIN32
static void _CLEAR_SCR() { system("cls"); }
#else 
static void _CLEAR_SCR() { system("clear"); }
#endif

ssr::SerialPort *pSerial = NULL;
std::string portName;
CONTROL_MODE controlMode;
MOVE_MODE moveMode;
MOVING_DIRECTION movingDirection;


std::vector<double> jointPos;

//int keyPressed = 0;
int keyReleasedCount = 0;
int keyReleasedCountThreshold = 6;

Matrix44 forwardKinematics;
Matrix44 forwardKinematicsWrist;
Matrix44 jointMatrixInKeyPressed;
Matrix44 wristMatrixInKeyPressed;


double fold_out_position[6] = {
  0.01,
  M_PI / 4,
  M_PI / 2,
  0.01,
  -M_PI / 4,
  0.01
};

reharo::IUdero* udero;
ParamLoader* loader;


int update_display = 1;
int sleep_time = 30;


char modeMsg[4] = { 'M', ':', '0', 0x08 };
char spdMsg[4] = { 'S', ':', '0', 0x08 };
CONTROL_MODE oldControlMode = CONTROL_UNKNOWN;
int old_speed_level = -1;

void updateLCD();

void initLCD() {
  try {
    portName = loader->get("LCD_PORT", "COM1");
    UTRACE("initLCD called");
    pSerial = new ssr::SerialPort(portName.c_str(), 57600);
    updateLCD();
  }
  catch (std::exception& ex) {
    UERROR("LCD COM Port (%s) can not open.", portName.c_str());
    pSerial = NULL;
    UERROR("LCD will be ignored.");
  }
  UTRACE("initLCD exit");
}


void updateLCD() {
  UTRACE("updateLCD called");
  bool sent = false;
  if (pSerial) {
    if (controlMode != oldControlMode) {
      if (controlMode == CONTROL_START) {
	modeMsg[2] = '0';
      }
      else if (controlMode == CONTROL_CARTESIAN) {
	modeMsg[2] = '1';
      }
      else {
	modeMsg[2] = '2';
      }
      pSerial->Write(modeMsg, 4);
      oldControlMode = controlMode;
      sent = true;
    }

    /*
    if (old_speed_level != speed_counter) {
      spdMsg[2] = '0' + (speed_counter);
      if (sent) ssr::Thread::Sleep(2000);

      pSerial->Write(spdMsg, 4);
      old_speed_level = speed_counter;
      }*/

  }
  UTRACE("updateLCD exit");
}


void updateDisplay(const KeyContext& keyContext) {
  //ssr::clear_scr();
_CLEAR_SCR();
  static int frames;
  frames++;
  //extern int keyPressed; // defined in keypad_routine.cpp
  int keyPressed = keyContext.new_key;
  extern int speed_counter;
  std::cout << "FRAME      : " << frames << "\n";
  std::cout <<     "KEY_STATE    : " << keyPressed << " / " << (char)keyPressed << "\n";
  if (controlMode == CONTROL_START) {
    std::cout << "CONTORL_MODE : " << "START_MODE\n";
  }
  else if (controlMode == CONTROL_CARTESIAN) {
    std::cout << "CONTORL_MODE : " << "CARTESIAN_MODE\n";
  }
  else {
    std::cout << "CONTORL_MODE : " << "JOINT_MODE\n";
  }

  std::cout << "SPEED_LEVEL  : " << speed_counter + 1 << std::endl;

  std::cout << "JOINT        :\n";
  for (int i = 0; i < 7; i++) {
    std::cout << "   - " << i << "  :" << jointPos[i] << "\n";
  }

  std::cout << "CARTESIAN    :\n";
  Matrix44 mat = udero->forwardKinematics();
  std::cout << "        x    :" << mat.v[0][3] << "\n";
  std::cout << "        y    :" << mat.v[1][3] << "\n";
  std::cout << "        z    :" << mat.v[2][3] << "\n";

  Vector3 rpy = MatrixToEulerZYZ(mat);
  std::cout << "        roll :" << rpy.v[0] << "\n";
  std::cout << "        pitch:" << rpy.v[1] << "\n";
  std::cout << "        yaw  :" << rpy.v[2] << "\n";

  std::cout << "HOMING       : " << udero->isHoming() << "\n";
  std::cout << "FOLDING IN   : " << udero->isFoldingIn() << "\n";
  std::cout << "FOLDING OUT  : " << udero->isFoldingOut() << "\n";
}

int onInitialized(const int argc, const char* argv[]) {
  reharo::initLogger(argc, argv);
  loader = new ParamLoader("param.yaml");
  //  logger_init(loader);
  UTRACE("onInitialized called"); 
  if (initKeyInput(loader)) {
    keyReleasedCountThreshold = atoi(loader->get("KEYBOARD_RELEASE_COUNT_THRESHOULD", "5").c_str());
    sleep_time = atoi(loader->get("SLEEP_TIME", "30").c_str());
    update_display = atoi(loader->get("UPDATE_DISPLAY", "0").c_str());
    
    
    setupSpeed(loader);
    
    
    initLCD();
    udero = udero_init(argc, argv);
    udero_load(udero, loader);
    
    UTRACE("onInitialized exit");
    return 0;
  } 
  return -1;
}

int onFinalized() {
  udero->spin();
  std::cout << "onFinalized called" << std::endl;
  udero_fini(udero);
  finiKeyInput();

  return 0;
}

static KeyContext keyContext;
static MoveCommand cmd(MOVE_STOP, MOVING_STOP, speed);

int onExecuted() {
  UTRACE("onExecuted called");
  keyContext = checkKeyInput(keyContext);

  cmd = keyHandler(cmd, keyContext);

  //MoveCommand cmd = checkKey();
  cmd = moveUdero(udero, cmd);
  udero->spin();


  updateUdero(udero);
  if (update_display) {
    updateDisplay(keyContext);
  }

  updateLCD();


  UTRACE("onExecuted exit");
  return 0;
}

extern const char* matrix_port;


/**
 * メイン関数
 */
int main(const int argc, const char* argv[]) {
#ifndef WIN32
  reharo::ledmatrix_write(matrix_port, FULL);
#endif
  if (onInitialized(argc, argv) < 0) {
    UERROR("Can Not initialize process.");
    return -1;
  }
#ifndef WIN32
  reharo::ledmatrix_write(matrix_port, S);
#endif
  while (!exit_program) {
    try {
      onExecuted();
      //      fflush(logfile);
      ssr::Thread::Sleep(sleep_time);
    }
    catch (std::exception& ex) {
      //std::cout << "Exception: " << ex.what() << std::endl;
      UERROR("Exception in mainloop: %s", ex.what());
      //      fflush(logfile);
      UERROR("Exception in mainloop: %s", ex.what());
      //resetCANUdero(udero);
      udero->reset();
    }
  }
  onFinalized();
  return 0;
}

