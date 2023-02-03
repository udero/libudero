#include "discrete_key_control_base.h"
//#include "logger.h"
#include "udero/UderoLogger.h"
#include "devices/ledmatrix.h"
extern const char* matrix_port;
void onEnterStartControlMode() {
	UTRACE("onEnterStartControlMode called");
#ifndef WIN32
	reharo::ledmatrix_write(matrix_port, S);
#endif
	UTRACE("onEnterStartControlMode exit");
}


void onExitStartControlMode() {
	UTRACE("onExitStartControlMode called");

	UTRACE("onExitStartControlMode exit");
}

void onKeyPressedInStartMode(int key) {
	UTRACE("onKeyPressedInStartMode(%c(0x%02X)) called", key);
	//jointMatrixInKeyPressed = forwardKinematics;
	//wristMatrixInKeyPressed = forwardKinematicsWrist;
	switch (key) {
	case '.':
		moveMode = MOVE_FOLDIN;
		break;
	case '0':
		moveMode = MOVE_FOLDOUT;
		break;

	case '6':
	  moveMode = MOVE_MOVING;
	  movingDirection = MOVING_BASE_POSITIVE;

	  break;
	case '3':
	  moveMode = MOVE_MOVING;
	  movingDirection = MOVING_BASE_NEGATIVE;
	  break;
	case 'h':
	  //LOGP(LOGLEVEL_VERBOSE, "udero->goHome calling");
	  //udero->goHome();
	  moveMode = MOVE_GOHOME;
		break;
	case 'f':
	  //LOGP(LOGLEVEL_VERBOSE, "udero->goHome FORCE calling");
	  //udero->goHome(true);
	  moveMode = MOVE_FORCEGOHOME;
		break;
	}
	UTRACE("onKeyPressedInStartMode exit");
}

void onKeyReleasedInStartMode(int key) {
  UTRACE("onKeyReleasedInStartMode(%c(0x%02X)) called", key);
  switch (key) {
  case '6':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
    break;
  case '3':
    moveMode = MOVE_STOP;
    movingDirection = MOVING_STOP;
  }

    /*;
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
    */
  UTRACE("onKeyReleasedInStartMode exit");
}

