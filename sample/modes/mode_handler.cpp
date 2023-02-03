#include "discrete_key_control.h"
//#include "logger.h"
#include "udero/UderoLogger.h"
#include "Console.h"
#define ENTER 0x0D
#define ESC 0x1B

bool exit_program = false;

void exitProgram() {
  exit_program = true;
}


// called when first pressed
MoveCommand onKeyPressed(const MoveCommand& cmd, int keyPressed) {
  UTRACE("onKeyPressed called");
  UINFO("KeyPressed: keyPressed = %c(0x%02X)", keyPressed, keyPressed);
  if (keyPressed == ENTER || keyPressed == 10) {
    if (controlMode == CONTROL_START) {
      onExitStartControlMode();
      controlMode = CONTROL_CARTESIAN;
      onEnterCartesianControlMode();
    }
    else if (controlMode == CONTROL_CARTESIAN) {
      onExitCartesianControlMode();
      controlMode = CONTROL_JOINT;
      onEnterJointControlMode();
    }
    else if (controlMode == CONTROL_JOINT) {
      onExitJointControlMode();
      controlMode = CONTROL_START;
      onEnterStartControlMode();
    }
  }
  else if (keyPressed == ESC) {
    exitProgram();
  } 
#ifndef WIN32
  else if (keyPressed == ESCAPE ) {
    exitProgram();
  }
#endif
  else {

    if (controlMode == CONTROL_START) {
      onKeyPressedInStartMode(keyPressed);
    }
    else if (controlMode == CONTROL_CARTESIAN) {
      onKeyPressedInCartesianMode(keyPressed);
    }
    else if (controlMode == CONTROL_JOINT) {
      onKeyPressedInJointMode(keyPressed);
    }
  }
  UTRACE("onKeyPressed exit");
  return MoveCommand(moveMode, movingDirection, speed);
}

MoveCommand onKeyReleased(const MoveCommand& cmd, int keyPressed) {
  UTRACE("onKeyReleased called");
  UINFO("KeyRelased: keyPressed = %c(0x%02X)", keyPressed, keyPressed);
  if (controlMode == CONTROL_START) {
    onKeyReleasedInStartMode(keyPressed);
  }
  else if (controlMode == CONTROL_CARTESIAN) {
    onKeyReleasedInCartesianMode(keyPressed);
  }
  else if (controlMode == CONTROL_JOINT) {
    onKeyReleasedInJointMode(keyPressed);
  }
  UTRACE("onKeyReleased exit");
  return MoveCommand(moveMode, movingDirection, speed);
}


MoveCommand keyHandler(const MoveCommand& cmd, const KeyContext& ctx) {
  //MoveCommand cmd;
  //UTRACE("checkKey called");
  UTRACE("n: %d(%c), o: %d(%c)", ctx.new_key, (char)ctx.new_key, ctx.old_key, (char)ctx.old_key);
  if (ctx.old_key == ctx.new_key) {
    // do nothing
  } else if (ctx.old_key != 0 && ctx.new_key != 0) {
    onKeyReleased(cmd, ctx.old_key);
    return onKeyPressed(cmd, ctx.new_key);
  } else if (ctx.old_key == 0 && ctx.new_key != 0) {
    return onKeyPressed(cmd, ctx.new_key);
  } else if (ctx.new_key == 0 && ctx.old_key != 0) {
    return onKeyReleased(cmd, ctx.old_key);
  }
  //  UTRACE("checkKey exit");
  return cmd;
}

