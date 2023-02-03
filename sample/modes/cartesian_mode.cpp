#include "discrete_key_control.h"
#include "udero/UderoLogger.h"
#include "devices/ledmatrix.h"
extern const char* matrix_port;
void onEnterCartesianControlMode() {
  UTRACE("onEnterCartesianControlMode called");
#ifndef WIN32
  reharo::ledmatrix_write(matrix_port, C);
#endif
  UTRACE("onEnterCartesianControlMode exit");
}

void onExitCartesianControlMode() {
  UTRACE("onExitCartesianControlMode called");

  UTRACE("onExitCartesianControlMode exit");
}

void onKeyPressedInCartesianMode(int key) {
  UTRACE("onKeyPressedInCartesianMode(%c(0x%02X)) called", key);
  jointMatrixInKeyPressed = forwardKinematics;
  wristMatrixInKeyPressed = forwardKinematicsWrist;
  std::cout << "PRESSED" << std::endl;
  switch (key) {
  case '4':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TZ_POSITIVE;
    break;
  case '1':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TZ_NEGATIVE;
    break;
  case '5':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TX_POSITIVE;
    break;
  case '2':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TX_NEGATIVE;
    break;
  case '6':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TY_NEGATIVE;
    break;
  case '3':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_TY_POSITIVE;
    break;
  case '/':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RY_POSITIVE;
    break;
  case '7':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RY_NEGATIVE;
    break;
  case '*':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RP_NEGATIVE;
    break;
  case '8':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RP_POSITIVE;
    break;
  case '9':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RR_NEGATIVE;
    break;
  case '-':
    moveMode = MOVE_MOVING;
    movingDirection = MOVING_RR_POSITIVE;
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
    {
      int c = toggleSpeed();
#ifndef WIN32
      if (c == 0) {
	reharo::ledmatrix_write(matrix_port, ONE);
      } else if (c == 1) {
	reharo::ledmatrix_write(matrix_port, TWO);
      } else if (c == 2) {
	reharo::ledmatrix_write(matrix_port, THREE);
      }
      ssr::Thread::Sleep(300);
      reharo::ledmatrix_write(matrix_port, C);
#endif
    }
    break;

  default:
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  }
  UTRACE("onKeyPressedInCartesianMode exit");
}


void onKeyReleasedInCartesianMode(int key) {
  UTRACE("onKeyReleasedInCartesianMode(%c(0x%02X)) called", key);
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
  UTRACE("onKeyReleasedInCartesianMode exit");
}
