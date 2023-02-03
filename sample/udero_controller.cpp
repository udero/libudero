#include "discrete_key_control.h"
//#include "logger.h"
#include "udero/UderoLogger.h"
#include "Thread.h"
#include "udero/kinematics.h"

using namespace reharo;

void setupFoldOutPosition(ParamLoader* loader);
void udero_homing_init(reharo::IUdero* udero_, ParamLoader* loader);

void udero_load(reharo::IUdero* udero_, ParamLoader* loader) {
  udero_homing_init(udero_, loader);
  setupFoldOutPosition(loader);
}

reharo::IUdero* udero_init(const int argc, const char* argv[]) {
  UTRACE("udero_init called");
  UderoConnectionProfile prof = parseArgs(argc, argv);
  reharo::IUdero* udero_ = createUdero(prof);
  UVERBOSE("udero created");
  UVERBOSE("udero initialized");
  for (int i = 0; i < 7; i++) {
    jointPos.push_back(0);
  }

  for (int i = 0; i < 7; i++) {
    udero_->setJointMode(i, reharo::MODE_POSITION);
    ssr::Thread::Sleep(100);
  }
  UVERBOSE("udero Position Mode set");
  for (int i = 0; i < 6; i++) {
    udero_->setJointAcceleration(i, 0.8);
  }
  udero_->setJointAcceleration(6, 30);
  UVERBOSE("udero default Acceleration set");
  for (int i = 0; i < 7; i++) {
    jointPos[i] = udero_->getJointPosition(i);
  }
  UVERBOSE("udero Read Initial Position");

  UTRACE("udero_init exit");
  return udero_;
}


void udero_homing_init(reharo::IUdero* udero_, ParamLoader* loader) {

  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_00", "-10000").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_01", "-85100").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_02", "-95000").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_03", "-46300").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_04", "-78000").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_05", "+90000").c_str()));
  udero_->setJointHomeOffset(0, atoi(loader->get("HOMING_OFFSET_06", "600000").c_str()));
  /*
    udero->ppJoints[0]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_00", "-10000").c_str()));
    udero->ppJoints[1]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_01", "-85100").c_str()));
    udero->ppJoints[2]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_02", "-95000").c_str()));
    udero->ppJoints[3]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_03", "-46300").c_str()));
    udero->ppJoints[4]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_04", "-78000").c_str()));
    udero->ppJoints[5]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_05", "90000").c_str()));
    udero->ppJoints[6]->setHomeOffset(atoi(loader->get("HOMING_OFFSET_06", "600000").c_str()));
  */
}


void setupFoldOutPosition(ParamLoader* loader) {

  fold_out_position[0] = atof(loader->get("HOMING_POSITION_00", "0.01").c_str());
  fold_out_position[1] = atof(loader->get("HOMING_POSITION_01", "0.785398").c_str());
  fold_out_position[2] = atof(loader->get("HOMING_POSITION_02", "1.570796").c_str());
  fold_out_position[3] = atof(loader->get("HOMING_POSITION_03", "0.01").c_str());
  fold_out_position[4] = atof(loader->get("HOMING_POSITION_04", "-0.785398").c_str());
  fold_out_position[5] = atof(loader->get("HOMING_POSITION_05", "0.01").c_str());

  /*
  fold_out_position[0] = atof(loader->get("HOMING_POSITION_00", "0.78").c_str());
  fold_out_position[1] = atof(loader->get("HOMING_POSITION_01", "0.78").c_str());
  fold_out_position[2] = atof(loader->get("HOMING_POSITION_02", "1.570796").c_str());
  fold_out_position[3] = atof(loader->get("HOMING_POSITION_03", "-0.9").c_str());
  fold_out_position[4] = atof(loader->get("HOMING_POSITION_04", "-1.3").c_str());
  fold_out_position[5] = atof(loader->get("HOMING_POSITION_05", "0.5").c_str());
  */
}


void udero_fini(reharo::IUdero* udero_) {
  UTRACE("udero_fini_called");
  udero_->fini();
  delete udero_;
  UTRACE("udero_fini_ended");
}


void updateUdero(reharo::IUdero* udero_) {
  UTRACE("updateUdero called");
  for (int i = 0; i < 7; i++) {
    jointPos[i] = udero_->getJointPosition(i);
  }

  UINFO("JOINT:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", jointPos[0], jointPos[1], jointPos[2], jointPos[3], jointPos[4], jointPos[5], jointPos[6]);
  forwardKinematics = forward_kinematics(jointPos);
  //forwardKinematics = udero_->forwardKinematics();
  Vector3 r = MatrixToEulerZYZ(forwardKinematics);
  UVERBOSE("EEPOS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", forwardKinematics.v[0][3], forwardKinematics.v[1][3], forwardKinematics.v[2][3],
       r.v[0], r.v[1], r.v[2]);
  forwardKinematicsWrist = forward_kinematics_wrist(jointPos);
  //forwardKinematicsWrist = udero_->forwardKinematicsWrist();
  UTRACE("updateUdero exit");
}


void stop(reharo::IUdero* udero_) {
  UTRACE("stop() called");
  udero_->quickStop();
  UTRACE("stop() exit.");
}

void moveJoint(reharo::IUdero* udero_, int index, double step, double joint_vel) {
  UTRACE("moveJoint(%02d,%02.3f) called", index, step);
  if (index < 4) {
    udero_->moveJoint(index, jointPos[index] + step, joint_vel);
  }
  else {
    std::vector<double> pos;
    std::vector<double> vel;
    for (int i = 0; i < 6; i++) {
      if (i == index) {
	pos.push_back(jointPos[i] + step);
      }
      else {
	pos.push_back(jointPos[i]);
      }
      vel.push_back(joint_vel);
    }
    udero_->moveJoints(pos, vel);
  }
  UTRACE("moveJoint() exit");
}

void handOpen(reharo::IUdero* udero_, const ControlSpeed& spd) {
  UTRACE("handOpen called");
  UVERBOSE("hand_step: %f", spd.hand_step);
  UVERBOSE("hand_speed: %f", spd.hand_speed);
  udero_->moveHandRel(spd.hand_step, spd.hand_speed);
  UTRACE("handOpen exit");
}

void handClose(reharo::IUdero* udero_, const ControlSpeed& spd) {
  UTRACE("handClose called");
  UVERBOSE("hand_step: %f", spd.hand_step);
  UVERBOSE("hand_speed: %f", spd.hand_speed);
  udero_->moveHandRel(-spd.hand_step, spd.hand_speed);
  UTRACE("handClose exit");
}

void transXYZ(reharo::IUdero* udero_, double x, double y, double z, double step_time) {
  UTRACE("transXYZ(%f, %f, %f) called", x, y, z);
  if (x != 0.0) {
    jointMatrixInKeyPressed.v[0][3] = forwardKinematics.v[0][3] + x;
  }
  if (y != 0.0) {
    jointMatrixInKeyPressed.v[1][3] = forwardKinematics.v[1][3] + y;
  }
  if (z != 0.0) {
    jointMatrixInKeyPressed.v[2][3] = forwardKinematics.v[2][3] + z;
  }
  /*
  std::vector<double> tgt = inverse_kinematics(jointMatrixInKeyPressed);
  tgt = joint_filter2(jointPos, tgt);
  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back(fabs(tgt[i] - jointPos[i]) / step_time);
  }
  UVERBOSE("TARGET_POS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", tgt[0], tgt[1], tgt[2], tgt[3], tgt[4], tgt[5]);
  UVERBOSE("TARGET_VEL:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  udero->moveJoints(tgt, vel);
  */
  udero_->moveCartesian(jointMatrixInKeyPressed, step_time);
  UTRACE("transXYZ exit");
}

void rollYaw(reharo::IUdero* udero_, double th, double step_time) {
  UTRACE("rollYaw(%f) called", th);
  Matrix44 pp;
  pp.v[0][0] = cos(th); pp.v[0][1] = -sin(th); pp.v[0][2] = 0; pp.v[0][3] = 0;
  pp.v[1][0] = sin(th); pp.v[1][1] = cos(th); pp.v[1][2] = 0; pp.v[1][3] = 0;
  pp.v[2][0] = 0; pp.v[2][1] = 0; pp.v[2][2] = 1; pp.v[2][3] = 0;
  pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
  Matrix44 p = pp * forwardKinematicsWrist;
  p.v[0][3] = forwardKinematicsWrist.v[0][3];
  p.v[1][3] = forwardKinematicsWrist.v[1][3];
  p.v[2][3] = forwardKinematicsWrist.v[2][3];
  udero_->moveCartesianWrist(p, step_time);

/*
std::vector<double> tgt = inverse_kinematics_wrist(p);
  tgt = joint_filter2(jointPos, tgt);

  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back(fabs(tgt[i] - jointPos[i]) / step_time);
  }
  UVERBOSE("TARGET_POS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", tgt[0], tgt[1], tgt[2], tgt[3], tgt[4], tgt[5]);
  UVERBOSE("TARGET_VEL:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  */
  //udero->moveJoints(tgt, vel);
  UTRACE("rollYaw exit");
}

void rollPitch(reharo::IUdero* udero_, double th, double step_time) {
  UTRACE("rollPitch(%f) called", th);

  Matrix44 pp;
  pp.v[0][0] = cos(th); pp.v[0][1] = 0; pp.v[0][2] = sin(th); pp.v[0][3] = 0;
  pp.v[1][0] = 0; pp.v[1][1] = 1; pp.v[1][2] = 0; pp.v[1][3] = 0;
  pp.v[2][0] = -sin(th); pp.v[2][1] = 0; pp.v[2][2] = cos(th); pp.v[2][3] = 0;
  pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
  Matrix44 p = forwardKinematicsWrist * pp;
  p.v[0][3] = forwardKinematicsWrist.v[0][3];
  p.v[1][3] = forwardKinematicsWrist.v[1][3];
  p.v[2][3] = forwardKinematicsWrist.v[2][3];
  udero_->moveCartesianWrist(p, step_time);
  /*
  std::vector<double> tgt = inverse_kinematics_wrist(p);
  tgt = joint_filter2(jointPos, tgt);

  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    //		vel.push_back(0.2);
    vel.push_back(fabs(tgt[i] - jointPos[i]) / step_time);
  }
  UVERBOSE("TARGET_POS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", tgt[0], tgt[1], tgt[2], tgt[3], tgt[4], tgt[5]);
  UVERBOSE("TARGET_VEL:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  */
  //udero->moveJoints(tgt, vel);
  UTRACE("rollPitch exit");
}


void rollPitch_(reharo::IUdero* udero_, double th) {

  std::vector<double> j = jointPos;
  std::vector<double> vel;
  vel.push_back(0.8);
  vel.push_back(0.8);
  vel.push_back(0.8);
  vel.push_back(0.8);
  vel.push_back(0.8);
  vel.push_back(0.8);
  j[4] = j[4] + th;
  j.pop_back();
  //	UVERBOSE("TARGET_POS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", tgt[0], tgt[1], tgt[2], tgt[3], tgt[4], tgt[5]);
  //	UVERBOSE("TARGET_VEL:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  udero_->moveJoints(j, vel);
}

void rollRoll(reharo::IUdero* udero_, double th, double step_time) {
  UTRACE("rollRoll(%f) called", th);
  std::vector<double> tgt = jointPos;
  tgt[5] = tgt[5] + th;
  tgt.pop_back();
  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    //		vel.push_back(0.2);
    vel.push_back((tgt[i] - jointPos[i]) / step_time);
  }
  UVERBOSE("TARGET_POS:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", tgt[0], tgt[1], tgt[2], tgt[3], tgt[4], tgt[5]);
  UVERBOSE("TARGET_VEL:%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
  udero_->moveJoints(tgt, vel);
  UTRACE("rollRoll exit");
}


void foldOut(reharo::IUdero* udero_) {
  UINFO("moveMode == MOVE_FOLDOUT");
  udero_->foldOut();
  std::vector<double> pos;
  std::vector<double> vel;
  for (int i = 0; i < 6; i++) {
    pos.push_back(fold_out_position[i]);
    vel.push_back(0.2);
  }
  UINFO("moveFoldOutPosition(%02.3f,%02.3f,%02.3f,%02.3f,%02.3f,%02.3f", pos[0], pos[1], pos[2], pos[3], pos[4], pos[5]);
  udero_->moveJoints(pos, vel);
  udero_->waitStopped();
}

MoveCommand moveUdero(reharo::IUdero* udero_, const MoveCommand& command) {
  const ControlSpeed& spd = command.speed;
  MOVE_MODE moveMode_ = command.moveMode;
  MOVING_DIRECTION movingDirection_ = command.movingDirection;
  UTRACE("moveUdero called");
  if (moveMode_ == MOVE_MOVING) {
    UINFO("moveMode_ == MOVE_MOVING");
    switch (movingDirection_) {
    case MOVING_TZ_POSITIVE:
      UINFO("movingDirection == MOVING_TZ_POSITIVE");
      transXYZ(udero_, 0, 0, spd.trans_step, spd.step_time);
      break;
    case MOVING_TZ_NEGATIVE:
      UINFO("movingDirection == MOVING_TZ_NEGATIVE");
      transXYZ(udero_, 0, 0, -spd.trans_step, spd.step_time);
      break;
    case MOVING_TY_POSITIVE:
      UINFO("movingDirection == MOVING_TY_POSITIVE");
      transXYZ(udero_, 0, spd.trans_step, 0, spd.step_time);
      break;
    case MOVING_TY_NEGATIVE:
      UINFO("movingDirection == MOVING_TY_NEGATIVE");
      transXYZ(udero_, 0, -spd.trans_step, 0, spd.step_time);
      break;
    case MOVING_TX_POSITIVE:
      UINFO("movingDirection == MOVING_TX_POSITIVE");
      transXYZ(udero_, spd.trans_step, 0, 0, spd.step_time);
      break;
    case MOVING_TX_NEGATIVE:
      UINFO("movingDirection == MOVING_TX_NEGATIVE");
      transXYZ(udero_, -spd.trans_step, 0, 0, spd.step_time);
      break;
    case MOVING_RY_POSITIVE:
      UINFO("movingDirection == MOVING_RY_POSITIVE");
      rollYaw(udero_, spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_RY_NEGATIVE:
      UINFO("movingDirection == MOVING_RY_NEGATIVE");
      rollYaw(udero_, -spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_RP_POSITIVE:
      UINFO("movingDirection == MOVING_RP_POSITIVE");
      rollPitch(udero_, spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_RP_NEGATIVE:
      UINFO("movingDirection == MOVING_RP_NEGATIVE");
      rollPitch(udero_, -spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_RR_POSITIVE:
      UINFO("movingDirection == MOVING_RR_POSITIVE");
      rollRoll(udero_, spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_RR_NEGATIVE:
      UINFO("movingDirection == MOVING_RR_NEGATIVE");
      rollRoll(udero_, -spd.rot_step / 180.0 * M_PI, spd.step_time);
      break;
    case MOVING_HAND_OPEN:
      handOpen(udero_, spd);
      break;
    case MOVING_HAND_CLOSE:
      handClose(udero_, spd);
      break;
    case MOVING_J1_POSITIVE:
      moveJoint(udero_, 0, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J1_NEGATIVE:
      moveJoint(udero_, 0, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J2_POSITIVE:
      moveJoint(udero_, 1, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J2_NEGATIVE:
      moveJoint(udero_, 1, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J3_POSITIVE:
      moveJoint(udero_, 2, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J3_NEGATIVE:
      moveJoint(udero_, 2, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J4_POSITIVE:
      moveJoint(udero_, 3, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J4_NEGATIVE:
      moveJoint(udero_, 3, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J5_POSITIVE:
      moveJoint(udero_, 4, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J5_NEGATIVE:
      moveJoint(udero_, 4, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J6_POSITIVE:
      moveJoint(udero_, 5, spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;
    case MOVING_J6_NEGATIVE:
      moveJoint(udero_, 5, -spd.joint_step / 180.0 * M_PI, spd.joint_vel / 180.0 * M_PI);
      break;		default:
      break;
    }
  }
  else if (moveMode_ == MOVE_FOLDIN) {
    UINFO("moveMode_ == MOVE_FOLDIN");
    udero_->foldIn();
    moveMode_ = MOVE_STOPPED;
  }
  else if (moveMode_ == MOVE_FOLDOUT) {
    UINFO("moveMode_ == MOVE_FOLDOUT");
    foldOut(udero_);
    moveMode_ = MOVE_STOPPED;
  }
  else if (moveMode_ == MOVE_STOP) {
    stop(udero_);
    moveMode_ = MOVE_STOPPED;
  }
  else if (moveMode_ == MOVE_GOHOME) {
    udero_->goHome();
    moveMode_ = MOVE_STOPPED;
  }
  else if (moveMode_ == MOVE_FORCEGOHOME) {
    udero_->goHome(true);
    moveMode_ = MOVE_STOPPED;
  }
  
  UTRACE("moveUdero exit");
  return MoveCommand(moveMode_, movingDirection_, spd);
}
