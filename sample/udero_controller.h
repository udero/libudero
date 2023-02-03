#pragma once

#include "param_loader.h"
#include "control_parameter_holder.h"

enum MOVE_MODE {
  MOVE_STOPPED = -1,
  MOVE_STOP,
  MOVE_MOVING,
  MOVE_FOLDIN,
  MOVE_FOLDOUT,
  MOVE_GOHOME,
  MOVE_FORCEGOHOME,
};

enum MOVING_DIRECTION {
  MOVING_STOP,
  MOVING_TX_POSITIVE,
  MOVING_TX_NEGATIVE,
  MOVING_TY_POSITIVE,
  MOVING_TY_NEGATIVE,
  MOVING_TZ_POSITIVE,
  MOVING_TZ_NEGATIVE,
  MOVING_RY_POSITIVE,
  MOVING_RY_NEGATIVE,
  MOVING_RP_POSITIVE,
  MOVING_RP_NEGATIVE,
  MOVING_RR_POSITIVE,
  MOVING_RR_NEGATIVE,
  MOVING_HAND_OPEN,
  MOVING_HAND_CLOSE,
  MOVING_J1_POSITIVE,
  MOVING_J1_NEGATIVE,
  MOVING_J2_POSITIVE,
  MOVING_J2_NEGATIVE,
  MOVING_J3_POSITIVE,
  MOVING_J3_NEGATIVE,
  MOVING_J4_POSITIVE,
  MOVING_J4_NEGATIVE,
  MOVING_J5_POSITIVE,
  MOVING_J5_NEGATIVE,
  MOVING_J6_POSITIVE,
  MOVING_J6_NEGATIVE,

};

struct MoveCommand {
  MOVE_MODE moveMode;
  MOVING_DIRECTION movingDirection;
  ControlSpeed speed;
  MoveCommand(MOVE_MODE m, MOVING_DIRECTION d, ControlSpeed spd): moveMode(m), movingDirection(d), speed(spd) {}
};




reharo::IUdero* udero_init(const int argc, const char* argv[]);
void udero_fini(reharo::IUdero* udero);
void updateUdero(reharo::IUdero* udero);

MoveCommand moveUdero(reharo::IUdero* udero, const MoveCommand& command);

void udero_load(reharo::IUdero* udero, ParamLoader* loader);

extern double fold_out_position[6];
