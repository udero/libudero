/**
 * @file UderoImpl.cpp
 * @author Yuki Suga (ysuga@yusga.net)
 */

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

#include "Thread.h"
#include "udero/UderoLogger.h"
#include "UderoImpl.h"

#include "can_param.h"
#define INCLUDE_WITH_DEFINE
#include "global_parameter.h"
#include "SettingFileParser.h"



double g_foldOutAngle[6] = {
  0.01,
  0.78,
  1.57,
  0.01,
  -0.78,
  0.01
};

double g_workAngle[6] = {
  0.78,
  0.78,
  1.57,
  -0.9,
  -1.3,
  0.5
};

double g_foldInAngle[6] = {
  0.0,
  // -1.3,
  -1.5,
  //  2.8,
  3.0,
  0.0,
  0.0,
  0.0
};

double g_foldingSpeed = 0.2;

double g_gear_ratio[7] = {
  100,
  //  202, // J2 
  2020.0 / 7,
  //  400, // J2 Version 2.0
  100,
  // 50, // J4 (J3.5)
  50, // J4 (J3.5) Version 2.0
  729 / 25.0 * 4,
  729 / 25.0 * 4,
  34.392,
};


// static const float g_foldingSpeed = 0.2;
using namespace technotools;
/// TODO: この値は新1号機のための値
//double v4_offset = 2.975;
//double v3_offset = M_PI;
//double v4_limit = 1.79;

/// TODO: この値は新1号機のための値
double v4_offset = 0;
double v3_offset = 0;//M_PI;
double v4_limit = 1.79;
double v4_dir = -1;
double v5_dir = 1;

//double v4_offset = 0;
UderoImpl::UderoImpl() : numJoints(0), foldingSpeed(g_foldingSpeed), task_("ready") {
  UTRACE("UderoImpl::UderoImpl()");
  for (int i = 0; i < UDERO_MAX_JOINT; i++) { ppJoints[i] = NULL; }

  for (int i = 0; i < UDERO_MAX_JOINT; i++) {
    foldingVelocity.push_back(foldingSpeed);
  }
  UTRACE("UderoImpl::UderoImpl() ended");
}

UderoImpl::UderoImpl(const std::string& settingFilename) : numJoints(0), foldingSpeed(g_foldingSpeed), task_("ready") {
  UTRACE("UderoImpl::UderoImpl(%s)", settingFilename.c_str());
  parseSettingFile(settingFilename);
  for (int i = 0; i < UDERO_MAX_JOINT; i++) { ppJoints[i] = NULL; }

  for (int i = 0; i < UDERO_MAX_JOINT; i++) {
    foldingVelocity.push_back(foldingSpeed);
  }
  UTRACE("UderoImpl::UderoImpl() ended");
}


UderoImpl::~UderoImpl() {
  for (uint32_t i = 0; i < numJoints; i++) {
    delete ppJoints[i];
  }
}

void UderoImpl::init() {
  UTRACE("UderoImpl::init() called()");
  for (uint32_t i = 0; i < numJoints; i++) {
    ppJoints[i]->init();
  }

  jointPos_.clear();
  for (int i = 0;i < 6;i++) {
    jointPos_.push_back(0);
  }

  UTRACE("UderoImpl::init() ended()");
}

void UderoImpl::goHome(bool blocking, bool force) {
  UDEBUG("UderoImpl::goHome(%d, %d)", blocking, force);
  if (blocking) {
    goHomeImpl(force);
  } else {
    waitReady();
    task_ = "homing";
    homingForce = force;
    Start();
  }
  UDEBUG("UderoImpl::goHome() ends");
}


void UderoImpl::goHomeImpl(bool force) {
  for(int i = 0;i < 5;i++) {
    try {
      goHomeInner(force);
      return;
    } catch (std::exception& ex) {
      std::cerr << "Exception in UderoImpl::goHomeImpl:" << ex.what() << std::endl;
      UWARN("Exception in UderoImpl::goHomeImpl:%s", ex.what());
      UWARN("Try again");
    }
  }
  UERROR("Exception in UderoImpl::goHomeImpl(): Error Exit.");
}

void UderoImpl::goHomeInner(bool force) {
  UDEBUG("UderoImpl::goHomeInner(%d)", force ? 1 : 0);

  if (!force) {
    if (ppJoints[0]->isHomed() &&
	ppJoints[1]->isHomed() &&
	ppJoints[2]->isHomed() &&
	ppJoints[3]->isHomed() &&
	ppJoints[4]->isHomed() &&
	ppJoints[5]->isHomed() &&
	ppJoints[6]->isHomed()) {
      UDEBUG("UderoImpl::goHomeInner(): Already Homed.");
      return;
    }
  }

  UTRACE(" HOMING: 1st Stage:");
  //ppJoints[4]->setMode(reharo::MODE_INACTIVE);
  //ppJoints[5]->setMode(reharo::MODE_INACTIVE);

  int timeout = 30000;
#if 1
  if (force || !(ppJoints[1]->isHomed() && ppJoints[2]->isHomed())) {
    UTRACE(" HOMING: J[1], J[2]");
    ppJoints[1]->goHome();
    ppJoints[2]->goHome();
    ppJoints[1]->waitHoming();
    ppJoints[2]->waitHoming();
  }
  else {
    UTRACE(" HOMING: J[1], J[2] is already homed.");
    ppJoints[1]->setMode(technotools::MODE_POSITION);
    ppJoints[2]->setMode(technotools::MODE_POSITION);
    ppJoints[1]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[2]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[1]->waitStopped(timeout);
    ppJoints[2]->waitStopped(timeout);
  }
#endif
  UTRACE(" HOMING: 2nd Stage:");
  if (force || !(ppJoints[4]->isHomed() && ppJoints[5]->isHomed())) {
    for (int i = 0; i < 20; i++) {
      try {
	    if (!ppJoints[4]->isHomed()) {
		  UTRACE(" HOMING: J[4]");
		  ppJoints[5]->setMode(technotools::MODE_VELOCITY);
	      ppJoints[5]->setAcceleration(10);
		  ppJoints[4]->goHome();
          while (1) {
			  ppJoints[5]->moveVelocity(ppJoints[4]->getActualVelocity());
			  if (ppJoints[4]->isHomed()) {
				  break;
			  }
			  ssr::Thread::Sleep(100);
		  }
		  ppJoints[5]->quickStop();
	      ppJoints[5]->setMode(technotools::MODE_POSITION);
	    } else {
	      UTRACE(" HOMING: J[4] is already homed.");
	      ppJoints[4]->setMode(technotools::MODE_POSITION);
	      ppJoints[4]->movePositionWithVelocity(0, foldingSpeed);
	      ppJoints[4]->waitStopped(timeout);
		}
		break;
      }
      catch (std::exception& ex) {
	    UERROR("Exception in goHome: %s", ex.what());
	    ssr::Thread::Sleep(500);
      }
    }
		
    for (int i = 0; i < 20; i++) {
      try {
	if (!ppJoints[5]->isHomed()) {
	  UTRACE(" HOMING: J[5]");

	  ppJoints[4]->setMode(technotools::MODE_VELOCITY);
	  ppJoints[4]->setAcceleration(10);
	  ppJoints[5]->goHome();
	  while (1) {
		  ppJoints[4]->moveVelocity(-ppJoints[5]->getActualVelocity());
		  if (ppJoints[5]->isHomed()) {
			  break;
		  }
		  ssr::Thread::Sleep(100);
	  }
	  ppJoints[4]->quickStop();
	  ppJoints[4]->setMode(technotools::MODE_POSITION);
	  ppJoints[5]->setMode(technotools::MODE_POSITION);
	}
	else {
	  UTRACE(" HOMING: J[5] already homed.");
	  ppJoints[5]->setMode(technotools::MODE_POSITION);
	  ppJoints[5]->movePositionWithVelocity(0, foldingSpeed);
	  ppJoints[5]->waitStopped(timeout);
	}
	break;
      }
      catch (std::exception& ex) {
	UERROR("Exception in goHome: %s", ex.what());
	ssr::Thread::Sleep(500);
      }
    }
  }
  else {
    UTRACE(" HOMING: J[4] J[5] already homed.");
    ppJoints[4]->setMode(technotools::MODE_POSITION);
    ppJoints[5]->setMode(technotools::MODE_POSITION);
    ppJoints[4]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[5]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[4]->waitStopped(timeout);
    ppJoints[5]->waitStopped(timeout);
  }

  UTRACE(" HOMING: 3rd Stage:");
  if (force || !(ppJoints[0]->isHomed() && ppJoints[3]->isHomed() && ppJoints[6]->isHomed())) {
    UTRACE(" HOMING: J[0] J[3] J[6]");
    ppJoints[0]->goHome();
    ppJoints[3]->goHome();
    ppJoints[6]->goHome();

    for (int i = 0; i < 20; i++) {
      try {
	ppJoints[0]->waitHoming();
	ppJoints[3]->waitHoming();
	ppJoints[6]->waitHoming();
	break;
      }
      catch (std::exception& ex) {
	UERROR("Exception in goHome: %s", ex.what());
	ssr::Thread::Sleep(1000);
      }
    }
  }
  else {
    UTRACE(" HOMING: J[0] J[3] J[6] already homed.");
    ppJoints[0]->setMode(technotools::MODE_POSITION);
    ppJoints[3]->setMode(technotools::MODE_POSITION);
    ppJoints[6]->setMode(technotools::MODE_POSITION);
    ppJoints[0]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[3]->movePositionWithVelocity(0, foldingSpeed);
    ppJoints[6]->movePositionWithVelocity(50, 10);
    ppJoints[0]->waitStopped(timeout);
    ppJoints[3]->waitStopped(timeout);
    ppJoints[6]->waitStopped(timeout);
  }

  UTRACE(" HOMING: Okay. Now Fold In");
  foldIn();
  UDEBUG("UderoImpl::goHomeInner() ends");
}

void UderoImpl::Run() {
  UTRACE("UderoImpl::Run(task=%s", task_.c_str());
  if (task_ == "homing") {
    UderoImpl::goHomeImpl(homingForce);
    task_ = "ready";
  } else if (task_ == "foldIn") {
    UderoImpl::foldInImpl();
    task_ = "ready";
  } else if (task_ == "foldOut") {
    UderoImpl::foldOutImpl();
    task_ = "ready";
  } else if (task_ == "waiting") {
    UderoImpl::waitStoppedImpl();
    task_ = "stopped";
  }
  UTRACE("UderoImpl::Run() ended");
}

void UderoImpl::waitReady() {
  UTRACE("UderoImpl::waitReady()");
  if (task_ == "stopped") {
    task_ = "ready";
  }
  while (task_ != "ready") {
    ssr::Thread::Sleep(10);
  }
  UTRACE("UderoImpl::waitReady() ends");
}

void UderoImpl::waitHoming() {
  UTRACE("UderoImpl::waitHoming()");
  while (task_ == "homing") {
    ssr::Thread::Sleep(10);
  }
  UTRACE("UderoImpl::waitHoming() ends");
}

bool UderoImpl::isHoming() {
  return task_ == "homing";
}

void UderoImpl::waitFoldIn() {
  while (task_ == "foldIn") {
    ssr::Thread::Sleep(10);
  }
}

bool UderoImpl::isFoldingIn() {
  return task_ == "foldIn";
}

void UderoImpl::waitFoldOut() {
  while (task_ == "foldOut") {
    ssr::Thread::Sleep(10);
  }
}

bool UderoImpl::isFoldingOut() {
  return task_ == "foldOut";
}

void UderoImpl::_goPose0() {
  UVERBOSE("UderoImpl::_goPose0()");
  std::vector<double> pos;
  pos.push_back(0.0);
  pos.push_back(0.0);
  pos.push_back(1.57);
  pos.push_back(0.0);
  pos.push_back(0.0);
  pos.push_back(0.0);
  std::vector<double> foldingSpeed;
  for(int i = 0;i < 6;i++) {
    foldingSpeed.push_back(g_foldingSpeed);
  }

  moveJoints(pos, foldingSpeed);
  waitStoppedImpl(30*1000);
  UVERBOSE("UderoImpl::_goPose0() ends");
}

void UderoImpl::_goPoseF() {
  UVERBOSE("UderoImpl::_goPoseF()");
  std::vector<double> pos;
  std::vector<double> foldingSpeed;
  for(int i = 0;i < 6;i++) {
    pos.push_back(g_foldOutAngle[i]);
    foldingSpeed.push_back(g_foldingSpeed);
  }
  moveJoints(pos, foldingSpeed);
  waitStoppedImpl(30*1000);
  UVERBOSE("UderoImpl::_goPoseF() ends");
}


void UderoImpl::_goPoseW() {
  UVERBOSE("UderoImpl::_goPoseW()");
  std::vector<double> pos;
  std::vector<double> foldingSpeed;
  for(int i = 0;i < 6;i++) {
    pos.push_back(g_workAngle[i]);
    foldingSpeed.push_back(g_foldingSpeed);
  }
  moveJoints(pos, foldingSpeed);
  waitStoppedImpl(30*1000);
  UVERBOSE("UderoImpl::_goPoseW() ends");
}

void UderoImpl::_goPoseI() {
  UVERBOSE("UderoImpl::_goPoseI()");
  std::vector<double> pos;
  std::vector<double> foldingSpeed;
  for(int i = 0;i < 6;i++) {
    pos.push_back(g_foldInAngle[i]);
    foldingSpeed.push_back(g_foldingSpeed);
  }

  moveJoints(pos, foldingSpeed);
  waitStoppedImpl(30*1000);
  UVERBOSE("UderoImpl::_goPoseI() ends");
}

void UderoImpl::foldIn(bool blocking) {
  UDEBUG("UderoImpl::foldIn(%d)", blocking);
  if(blocking) {
    foldInImpl();
  } else {
    waitReady();
    task_ = "foldIn";
    ssr::Thread::Start();
  }
  UDEBUG("UderoImpl::foldIn() ends");
}

void UderoImpl::foldInImpl()  {
  UDEBUG("UderoImpl::foldInImpl()");
  for (int i = 0; i < 7; i++) {
    ppJoints[i]->setMode(technotools::MODE_POSITION);
  }

  double j0 = ppJoints[0]->getPosition();
  if (j0 >= 0.1) {
    _goPoseF();
    _goPose0();
  }

  _goPoseI();
  UDEBUG("UderoImpl::foldInImpl() ends");
}

void UderoImpl::foldOut(bool blocking) {
  UDEBUG("UderoImpl::foldOut(%d)", blocking);
  if (blocking) {
    foldOutImpl();
  } else {
    waitReady();
    task_ = "foldOut";
    ssr::Thread::Start();
  }
  UDEBUG("UderoImpl::foldOut() ends");
}

void UderoImpl::foldOutImpl() {
  UDEBUG("UderoImpl::foldOutImpl()");
  for (int i = 0; i < 7; i++) {
    ppJoints[i]->setMode(technotools::MODE_POSITION);
  }

  double j1 = ppJoints[1]->getPosition();
  if (j1 <= 0.1) {
    _goPose0();
  }

  _goPoseF();
  //  _goPoseW();
  UDEBUG("UderoImpl::foldOutImpl() ends");
}

void UderoImpl::waitStopped(bool blocking) {
  UTRACE("UderoImpl::waitStopped(%d)", blocking);
  if (blocking) {
    waitStoppedImpl();
  } else {
    waitReady();
    task_ = "waiting";
    ssr::Thread::Start();
  }
  UTRACE("UderoImpl::waitStopped() ends");
}

bool UderoImpl::isStopped() {
  return strcmp(task_.c_str(), "stopped");
}

void UderoImpl::waitStoppedImpl(int32_t timeout) {
  UTRACE("UderoImpl::waitStoppedImpl()");
  for (int i = 0; i < 6; i++) {
    ppJoints[i]->waitStopped(timeout);
  }
  UTRACE("UderoImpl::waitStoppedImpl() ends");
}

void UderoImpl::quickStop() {
  UVERBOSE("UderoImpl::quickStop()");
  for (int i = 0; i < 6; i++) {
    ppJoints[i]->quickStop();
  }

  ppJoints[6]->quickStop();
  for (int i = 0; i < 6; i++) {
    ppJoints[i]->waitStopped();
    ppJoints[i]->enableOperation();
  }
}

void UderoImpl::moveJoints(const std::vector<double>& pos_, const std::vector<double>& vel) {

  std::vector<double> pos = pos_;
  if (pos.size() >= 6 && vel.size() >= 6) {
    UVERBOSE("UderoImpl::moveJoints({%f,%f,%f,%f,%f,%f},{%f,%f,%f,%f,%f})",
	     pos[0], pos[1], pos[2], pos[3], pos[4], pos[5],
	     vel[0], vel[1], vel[2], vel[3], vel[4], vel[5]);
    for (int i = 0; i < 6; i++) {
      
      if (pos[i] < g_angle_min_max[i][0]) {
	    pos[i] = g_angle_min_max[i][0];
	    UVERBOSE(" - JointRange Error (index=%d, passedValue=%f, min=%f)", i, pos[i], g_angle_min_max[i][0]);
      } else if (pos[i] > g_angle_min_max[i][1]) {
	    pos[i] = g_angle_min_max[i][1];
	    UVERBOSE(" - JointRange Error (index=%d, passedValue=%f, max=%f)", i, pos[i], g_angle_min_max[i][1]);
      }
    }
    for (int index = 0; index < 6; index++) {
      if (0 <= index && index < 4) {
	    if (index == 3) {
	      ppJoints[index]->movePositionWithVelocity(pos[index] - v3_offset, vel[index]);
	    } else {
	      ppJoints[index]->movePositionWithVelocity(pos[index], vel[index]);
	    }
      } else if (index == 6) {
	    ppJoints[index]->movePositionWithVelocity(pos[index], vel[index]);
      }
    }
    
    //	if (pos[4] > v4_limit) { pos[4] = v4_limit; }
    //	else if (pos[4] < -v4_limit) { pos[4] = -v4_limit; }
    pos[4] = v4_dir * pos[4];
    pos[5] = v5_dir * pos[5];
    double vel4 = vel[4] * (pos[4] > 0 ? 1 : -1);
    double vel5 = vel[5] * (pos[5] > 0 ? 1 : -1);
    ppJoints[4]->movePositionWithVelocity((pos[4] - pos[5]) + v4_offset, fabs(vel4 - vel5) / 2 + fabs(vel4 + vel5) / 2);
    ppJoints[5]->movePositionWithVelocity((pos[4] + pos[5]), fabs(vel4 - vel5) / 2 + fabs(vel4 + vel5) / 2);
  } else {
    UWARN("UderoSocket::moveJoints(pos(size=%d), vel(size=%d): failed. Invalid size of vector", pos.size(), vel.size());
  }
}

double UderoImpl::getJointPos(const int index, const bool PDO) {
  _checkJointIndex(index);
  UVERBOSE("UderoImpl::getJointPos(index=%d, PDO=%d)", index, PDO);
  if ((index >= 0 && index < 4) ) {
	if (index == 3) {
	  return ppJoints[index]->getPosition(PDO) + v3_offset;
	}
    return ppJoints[index]->getPosition(PDO);
  }

  if (index == 6) {
    return ppJoints[index]->getPosition(false);
  }
  /**
  std::cout << "------------------------" << std::endl;
  std::cout << (ppJoints[4]->getPosition(false)) << std::endl;
  std::cout << (ppJoints[5]->getPosition(false)) << std::endl;
  std::cout << (ppJoints[4]->getPosition(true)) << std::endl;
  std::cout << (ppJoints[5]->getPosition(true)) << std::endl;
  std::cout << "------------------------" << std::endl;
  */
  const bool _pdo = false;
  if (index == 5) {
    return v5_dir * (ppJoints[5]->getPosition(_pdo)  - (ppJoints[4]->getPosition(_pdo) - v4_offset)) / 2;
  }
  else { // if (index == 4) {
    return v4_dir * (ppJoints[4]->getPosition(_pdo) - v4_offset + ppJoints[5]->getPosition(_pdo)) / 2;
  }
}

void UderoImpl::moveHand(const double gap, const double vel) {
  UTRACE("UderoImpl::moveHand(%f, %f)", gap, vel);
  ppJoints[6]->movePositionWithVelocity(gap, vel);
}

void UderoImpl::moveHandRel(const double gap, const double vel) {
  UTRACE("UderoImpl::moveHandRel(%f, %f)", gap, vel);
  double pos = ppJoints[6]->getPosition() + gap;
  ppJoints[6]->movePositionWithVelocity(pos, vel);
}

void UderoImpl::moveHandSync(const double gap, const double vel) {
  UTRACE("UderoImpl::moveHandSync(%f, %f)", gap, vel);
  ppJoints[6]->movePositionWithVelocity(gap, vel);
  try {
    ppJoints[6]->waitStopped(1000);
  } catch (std::exception& ex) {
    ppJoints[6]->init();
    ppJoints[6]->setMode(MODE_POSITION);
  }
}

void UderoImpl::moveHandRelSync(const double gap, const double vel) {
  UTRACE("UderoImpl::moveHandRelSync(%f, %f)", gap, vel);
  double pos = ppJoints[6]->getPosition() + gap;
  ppJoints[6]->movePositionWithVelocity(pos, vel);
  try {
    ppJoints[6]->waitStopped(1000);
  } catch (std::exception& ex) {
    ppJoints[6]->init();
    ppJoints[6]->setMode(MODE_POSITION);
  }
}


void UderoImpl::moveJoint(const int index, const double pos, const double vel, const bool minMaxOff) {
  UTRACE("UderoImpl::moveJoint(%d, %f, %f)", index, pos, vel);
  if (0 <= index && index < 4) {
    ppJoints[index]->movePositionWithVelocity(pos, vel, minMaxOff);
  }
  else if (index == 6) {
    ppJoints[index]->movePositionWithVelocity(pos, vel, minMaxOff);
  }
  else {
    UWARN(" - moveJoint function does not match to joint index [%d]", index);
    /*
      double vel4 = vel * (pos > 0 ? 1 : -1);
      double vel5 = vel * (pos > 0 ? 1 : -1);
      ppJoints[4]->movePositionWithVelocity(pos[4] - pos[5], fabs(vel4 - vel5) / 2 + fabs(vel4 + vel5) / 2);
      ppJoints[5]->movePositionWithVelocity(pos[4] + pos[5], fabs(vel4 - vel5) / 2 + fabs(vel4 + vel5) / 2);
    */
  }
}


void UderoImpl::spin() {
  UVERBOSE("UderoImpl::spin()");
  for (int i = 0; i < 6; i++) {
    jointPos_[i] = getJointPosition(i);
    UVERBOSE(" - J[%d] = ", i, jointPos_[i]);
  }

  forwardKinematics_ = forward_kinematics(jointPos_);
  forwardKinematicsWrist_ = forward_kinematics_wrist(jointPos_);
  //forward_kinematics_both(jointPos_, forwardKinematics_, forwardKinematicsWrist_);

  if (ISLOG(UDERO_TRACE)) {
    Matrix44& mat = forwardKinematics_;
    UVERBOSE(" - FK:");
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[0][0], mat.v[0][1], mat.v[0][2], mat.v[0][3]);
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[1][0], mat.v[1][1], mat.v[1][2], mat.v[1][3]);
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[2][0], mat.v[2][1], mat.v[2][2], mat.v[2][3]);
    
    mat = forwardKinematicsWrist_;
    UVERBOSE(" - FK(Wrist):");
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[0][0], mat.v[0][1], mat.v[0][2], mat.v[0][3]);
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[1][0], mat.v[1][1], mat.v[1][2], mat.v[1][3]);
    UVERBOSE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[2][0], mat.v[2][1], mat.v[2][2], mat.v[2][3]);
  }
}

// if true, OK, not invalid targetJoint position.
bool UderoImpl::checkTarget(const std::vector<double>& target, const Matrix44& mat) {
  UTRACE("UderoImpl::checkTarget()");
  bool flag = false;
  double epsilon_xyz = 0.01;
  for (int i = 0; i < 6; i++) {
	  if (isnan(target[i])) {
		  flag = true;
	  }
  }

  if (target[4] > v4_limit || target[4] < -v4_limit) {
	  return false;
  }

  Matrix44 fk = forward_kinematics(target);
  for(int i = 0;i < 3;i++) {
    UTRACE(" - %f, %f", fk.v[i][3], mat.v[i][3]);
    if (fabs(fk.v[i][3] - mat.v[i][3]) > epsilon_xyz) {
      flag = true;
    }
  }

  if (flag) {
    UTRACE(" - invalid target.");
  }
  return !flag;
}

bool UderoImpl::checkTargetWrist(const std::vector<double>& target, const Matrix44& mat) {
  UTRACE("UderoImpl::checkTargetWrist()");
  Matrix44 fk = forward_kinematics_wrist(target);
  bool flag = false;
  double epsilon_xyz = 0.01;
  for (int i = 0; i < 6; i++) {
	  if (isnan(target[i])) {
		  flag = true;
	  }
  }

  if (target[4] > v4_limit || target[4] < -v4_limit) {
	  return false;
  }


  for(int i = 0;i < 3;i++) {
    if (fabs(fk.v[i][3] - mat.v[i][3]) > epsilon_xyz) {
      flag = true;
    }
  }
  if (flag) {
    UTRACE(" - invalid target.");
  }

  return !flag;
}


int UderoImpl::moveCartesian(const Matrix44& mat, const UderoREAL step_time) {
  if (ISLOG(UDERO_DEBUG)) {
    UTRACE("UderoSocket::moveCartesian(mat=");
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[0][0], mat.v[0][1], mat.v[0][2], mat.v[0][3]);
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[1][0], mat.v[1][1], mat.v[1][2], mat.v[1][3]);
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[2][0], mat.v[2][1], mat.v[2][2], mat.v[2][3]);
    UTRACE(" step_time = %f", step_time);
  }
  
  std::vector<double> tgt = inverse_kinematics(mat);
  tgt = joint_filter2(jointPos_, tgt);

  if(!checkTarget(tgt, mat)) {
    UTRACE(" - moveFaied. Check Pass Error.");
    return -E_TARGETOUTOFRANGE;
  }

  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back(fabs(tgt[i] - jointPos_[i]) / step_time);
  }
  moveJoints(tgt, vel);
  return O_OK;
}

int UderoImpl::moveCartesianWrist(const Matrix44& mat, const UderoREAL step_time) {
  if (ISLOG(UDERO_DEBUG)) {
    UTRACE("UderoImpl::moveCartesianWrist(mat=");
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[0][0], mat.v[0][1], mat.v[0][2], mat.v[0][3]);
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[1][0], mat.v[1][1], mat.v[1][2], mat.v[1][3]);
    UTRACE(" | %03.3f, %03.3f, %03.3f %03.3f |", mat.v[2][0], mat.v[2][1], mat.v[2][2], mat.v[2][3]);
    UTRACE(" step_time = %f", step_time);
  }

  std::vector<double> tgt = inverse_kinematics_wrist(mat);
  tgt = joint_filter2(jointPos_, tgt);
  if (!checkTargetWrist(tgt, mat)) {
    UTRACE(" - moveFaied. Check Pass Error.");
    return -E_TARGETOUTOFRANGE;
  }
  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back(fabs(tgt[i] - jointPos_[i]) / step_time);
  }
  moveJoints(tgt, vel);
  return O_OK;
}


Matrix44 UderoImpl::forwardKinematics(){
  return forwardKinematics_;
}

Matrix44 UderoImpl::forwardKinematicsWrist() {
  return forwardKinematicsWrist_;
}
