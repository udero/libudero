#pragma once
#include <stdlib.h>
#include "param_loader.h"


class ControlSpeed {
 public:
  double trans_step;
  double rot_step;
  double step_time;
  double trans_stopping_step;
  double rot_stopping_step;
  double step_stopping_time;
  double hand_step;
  double hand_speed;
  double joint_step;
  double joint_vel;

  ControlSpeed() {
    trans_step = 10;
    rot_step = 10;
    step_time = 1;
    trans_stopping_step = 10;
    rot_stopping_step = 10;
    step_stopping_time = 1;
    hand_step = 10;
    hand_speed = 10.0;
    joint_step = 5;
    joint_vel = 5;
  }

  void setupSpeed(ParamLoader* loader);
  void toggleSpeed();
};

extern ControlSpeed speed;

void setupSpeed(ParamLoader* loader);
int toggleSpeed();
