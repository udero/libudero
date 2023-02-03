#include "control_parameter_holder.h"
//#include "logger.h"
#include "udero/UderoLogger.h"


ControlSpeed speed;

int speed_counter = 0;

static double speed_level[3][10] = {
  { 10, 10, 1, 10, 10, 1, 10, 10, 2, 2 },
  { 10, 10, 0.5, 10, 10, 1, 10, 20, 5, 5 },
  { 10, 10, 0.2, 10, 10, 1, 10, 30, 10, 10 },
};


void setupSpeed(ParamLoader* loader) {
  speed.setupSpeed(loader);
}
int toggleSpeed() {
  speed.toggleSpeed();
  return speed_counter;
}

void ControlSpeed::setupSpeed(ParamLoader* loader) {
  speed_level[0][0] = atof(loader->get("SPEED_LOW_TRANSLATION_STEP", "10").c_str());
  speed_level[0][1] = atof(loader->get("SPEED_LOW_ROTATION_STEP", "10").c_str());
  speed_level[0][2] = atof(loader->get("SPEED_LOW_MOVE_TIME", "1.0").c_str());
  speed_level[0][3] = atof(loader->get("SPEED_LOW_TRANSLATION_STOPPING_STEP", "1").c_str());
  speed_level[0][4] = atof(loader->get("SPEED_LOW_ROTATION_STOPPING_STEP", "10").c_str());
  speed_level[0][5] = atof(loader->get("SPEED_LOW_MOVE_STOPPING_TIME", "1.0").c_str());
  speed_level[0][6] = atof(loader->get("SPEED_LOW_HAND_STEP", "10").c_str());
  speed_level[0][7] = atof(loader->get("SPEED_LOW_HAND_SPEED", "10.0").c_str());
  speed_level[0][8] = atof(loader->get("SPEED_LOW_JOINT_STEP", "2").c_str());
  speed_level[0][9] = atof(loader->get("SPEED_LOW_JOINT_SPEED", "2").c_str());

  speed_level[1][0] = atof(loader->get("SPEED_MID_TRANSLATION_STEP", "10").c_str());
  speed_level[1][1] = atof(loader->get("SPEED_MID_ROTATION_STEP", "10").c_str());
  speed_level[1][2] = atof(loader->get("SPEED_MID_MOVE_TIME", "0.5").c_str());
  speed_level[1][3] = atof(loader->get("SPEED_MID_TRANSLATION_STOPPING_STEP", "5").c_str());
  speed_level[1][4] = atof(loader->get("SPEED_MID_ROTATION_STOPPING_STEP", "10").c_str());
  speed_level[1][5] = atof(loader->get("SPEED_MID_MOVE_STOPPING_TIME", "1.0").c_str());
  speed_level[1][6] = atof(loader->get("SPEED_MID_HAND_STEP", "10").c_str());
  speed_level[1][7] = atof(loader->get("SPEED_MID_HAND_SPEED", "20.0").c_str());
  speed_level[1][8] = atof(loader->get("SPEED_MID_JOINT_STEP", "5").c_str());
  speed_level[1][9] = atof(loader->get("SPEED_MID_JOINT_SPEED", "5").c_str());

  speed_level[2][0] = atof(loader->get("SPEED_HI_TRANSLATION_STEP", "10").c_str());
  speed_level[2][1] = atof(loader->get("SPEED_HI_ROTATION_STEP", "10").c_str());
  speed_level[2][2] = atof(loader->get("SPEED_HI_MOVE_TIME", "0.2").c_str());
  speed_level[2][3] = atof(loader->get("SPEED_HI_TRANSLATION_STOPPING_STEP", "10").c_str());
  speed_level[2][4] = atof(loader->get("SPEED_HI_ROTATION_STOPPING_STEP", "10").c_str());
  speed_level[2][5] = atof(loader->get("SPEED_HI_MOVE_STOPPING_TIME", "1.0").c_str());
  speed_level[2][6] = atof(loader->get("SPEED_HI_HAND_STEP", "10").c_str());
  speed_level[2][7] = atof(loader->get("SPEED_HI_HAND_SPEED", "30.0").c_str());
  speed_level[2][8] = atof(loader->get("SPEED_HI_JOINT_STEP", "10").c_str());
  speed_level[2][9] = atof(loader->get("SPEED_HI_JOINT_SPEED", "15").c_str());


  trans_step = speed_level[speed_counter][0];
  rot_step = speed_level[speed_counter][1];
  step_time = speed_level[speed_counter][2];
  trans_stopping_step = speed_level[speed_counter][3];
  rot_stopping_step = speed_level[speed_counter][4];
  step_stopping_time = speed_level[speed_counter][5];
  hand_step = speed_level[speed_counter][6];
  hand_speed = speed_level[speed_counter][7];
  joint_step = speed_level[speed_counter][8];
  joint_vel = speed_level[speed_counter][9];

  UINFO("SpeedLevel Update to %d", speed_counter);
  UVERBOSE("trans_step = %f", trans_step);
  UVERBOSE("rot_step = %f", rot_step);
  UVERBOSE("step_time = %f", step_time);
  UVERBOSE("trans_stopping_step = %f", trans_stopping_step);
  UVERBOSE("rot_stopping_step = %f", rot_stopping_step);
  UVERBOSE("step_stopping_time = %f", step_stopping_time);
  UVERBOSE("hand_step = %f", hand_step);
  UVERBOSE("hand_speed = %f", hand_speed);
  UVERBOSE("joint_step = %f", joint_step);
  UVERBOSE("joint_speed = %f", joint_vel);

}


void ControlSpeed::toggleSpeed() {
  UTRACE("toggleSpeed called");
  speed_counter++;
  if (speed_counter == 3) speed_counter = 0;
  UINFO("SpeedLevel Update to %d", speed_counter);

  trans_step = speed_level[speed_counter][0];
  rot_step = speed_level[speed_counter][1];
  step_time = speed_level[speed_counter][2];

  trans_stopping_step = speed_level[speed_counter][3];
  rot_stopping_step = speed_level[speed_counter][4];
  step_stopping_time = speed_level[speed_counter][5];
	
  hand_step = speed_level[speed_counter][6];
  hand_speed = speed_level[speed_counter][7];

  joint_step = speed_level[speed_counter][8];
  joint_vel = speed_level[speed_counter][9];

  UVERBOSE("trans_step = %f", trans_step);
  UVERBOSE("rot_step = %f", rot_step);
  UVERBOSE("step_time = %f", step_time);
  UVERBOSE("trans_stopping_step = %f", trans_stopping_step);
  UVERBOSE("rot_stopping_step = %f", rot_stopping_step);
  UVERBOSE("step_stopping_time = %f", step_stopping_time);
  UVERBOSE("hand_step = %f", hand_step);
  UVERBOSE("hand_speed = %f", hand_speed);
  UVERBOSE("joint_step = %f", joint_step);
  UVERBOSE("joint_speed = %f", joint_vel);
  UTRACE("toggleSpeed exit");
}

