#include "discrete_key_control.h"
//#include "logger.h"
#include "udero/UderoLogger.h"
#include "devices/ledmatrix.h"
extern const char* matrix_port;
void onEnterJointControlMode() {
  UTRACE("onEnterJointControlMode called");
#ifndef WIN32
  reharo::ledmatrix_write(matrix_port, J);
#endif
  UTRACE("onEnterJointControlMode exit");
}


void onExitJointControlMode() {
  UTRACE("onExitJointControlMode called");

  UTRACE("onExitJointControlMode exit");
}

void onKeyPressedInJointMode(int key) {
  UTRACE("onKeyPressedInJointMode(%c(0x%02X)) called", key);
  switch (key) {
  case '/':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J4_POSITIVE;
    break;
  case '7':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J4_NEGATIVE;
    break;
  case '*':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J5_POSITIVE;
    break;
  case '8':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J5_NEGATIVE;
    break;
  case '-':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J6_POSITIVE;
    break;
  case '9':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J6_NEGATIVE;
    break;
  case '4':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J1_POSITIVE;
    break;
  case '1':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J1_NEGATIVE;
    break;
  case '2':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J2_POSITIVE;
    break;
  case '5':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J2_NEGATIVE;
    break;
  case '3':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J3_POSITIVE;
    break;
  case '6':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_J3_NEGATIVE;
    break;

  case '0':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_HAND_OPEN;
    break;
  case '.':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_HAND_CLOSE;
    break;
  case '+':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    toggleSpeed();
    break;
  case 'h':
    //LOGP(LOGLEVEL_VERBOSE, "udero->goHome calling");
    //udero->goHome();
    moveMode = MOVE_GOHOME;
    break;
  }
  UTRACE("onKeyPressedInJointMode exit");
}

void onKeyReleasedInJointMode(int key) {
  UTRACE("onKeyReleasedInJointMode(%c(0x%02X)) called", key);
  switch (key) {
  case '1':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '4':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '5':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '2':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '6':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '3':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '/':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '7':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '*':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '8':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '9':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '-':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '0':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '.':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '+':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  default:
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  }
  UTRACE("onKeyReleasedInJointMode exit");
}
