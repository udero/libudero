#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

#define _USE_MATH_DEFINES
#include <math.h>

// #include <conio.h>
#include "signal.h"
#include "joystick.h"

using namespace reharo;

IUdero* udero = NULL;

void update_udero();
void move_arm();
void debug_print();
int counter = 0;
bool endFlag = false;
double joint_pos[7];

Matrix44 forwardKinematics;
Matrix44 forwardKinematicsWrist;
Matrix44 jointMatrixInKeyPressed;
Matrix44 wristMatrixInKeyPressed;

double step_deg = 0.01;
double step_time = 0.3;
double tstep = 15;
double astep = 15 / 180.0 * M_PI;
double hstep = 10;
double hspd = 10.0;

bool moving = false;

/**
 * SIGINTがきた場合に呼ばれる関数
 */
void sigint_handler(int sig) {
  endFlag = true;
}

#define DEBUG

int main(const int argc, const char* argv[]) {
  
  try {
    std::cout << "joystick_control: 1.0.0" << std::endl;
    // シグナル処理を追加する

    if (signal(SIGINT, sigint_handler) == SIG_ERR ) {
      std::cout << "signal(SIGINT) failed." << std::endl;
      return -1;
    }
    init_joystick();

    udero = createUdero(parseArgs(argc, argv));    

    for (int i = 0; i < 7; i++) {
      udero->setJointMode(i, MODE_POSITION);
    }
    for (int i = 0; i < 7; i++) {
      //udero->setJointAcceleration(i, 15.0);
    }

    while(!endFlag) {
      //      debug_print();
      update_joystick();

      //if (button[11] && !button_old[11]) {
      //break;
      //}
      
      update_udero();
      if (!(udero->isFoldingOut() || udero->isFoldingIn())) {
	move_arm();
      }

      ssr::Thread::Sleep(20);
    }
    udero->fini();
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cerr << "Exception in main: " << ex.what() << std::endl;
    UERROR("Exception in main %s", ex.what());
    return -1;
  }
  return 0;
}



void update_udero() {
  udero->spin();
  for (int i = 0; i < 7; i++) {
    joint_pos[i] = udero->getJointPosition(i);
  }
  forwardKinematics = udero->forwardKinematics();
  forwardKinematicsWrist = udero->forwardKinematicsWrist();
}


void debug_print() {
  system("clear");
  std::cout << "JOYSTICK_CONTROL FRAME=" << counter++ << "\n";
  std::cout << "AXIS:    X,    Y,    Z,    R,    U,    V,  PovX,  PovY, \n";
  std::cout.width(4);
  std::cout << "     ";
  for (int i = 0; i < 8; i++) {
    std::cout << axis[i] << ", ";
  }

  std::cout << "\nBTN :";
  for (size_t i = 0; i < 12; i++) {
    std::cout << button[i];
  }
  std::cout << std::endl;

  for (int i = 0; i < 7; i++) {
    std::cout << "J[" << i + 1 << "]: " << joint_pos[i] << std::endl;
  }
}

void save_mat() {
  //jointMatrixInKeyPressed = udero->forwardKinematics();
  //wristMatrixInKeyPressed = udero->forwardKinematicsWrist();
}

void transXYZ(double x, double y, double z) {
  if (x != 0.0) {
    jointMatrixInKeyPressed.v[0][3] = forwardKinematics.v[0][3] + x;
  }
  if (y != 0.0) {
    jointMatrixInKeyPressed.v[1][3] = forwardKinematics.v[1][3] + y;
  }
  if (z != 0.0) {
    jointMatrixInKeyPressed.v[2][3] = forwardKinematics.v[2][3] + z;
  }
  udero->moveCartesian(jointMatrixInKeyPressed, step_time);
}

void rollPitch(double th) {
  Matrix44 pp;
  pp.v[0][0] = cos(th); pp.v[0][1] = 0; pp.v[0][2] = sin(th); pp.v[0][3] = 0;
  pp.v[1][0] = 0; pp.v[1][1] = 1; pp.v[1][2] = 0; pp.v[1][3] = 0;
  pp.v[2][0] = -sin(th); pp.v[2][1] = 0; pp.v[2][2] = cos(th); pp.v[2][3] = 0;
  pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
  Matrix44 p = forwardKinematicsWrist * pp;
  p.v[0][3] = forwardKinematicsWrist.v[0][3];
  p.v[1][3] = forwardKinematicsWrist.v[1][3];
  p.v[2][3] = forwardKinematicsWrist.v[2][3];
  udero->moveCartesianWrist(p, step_time);
}

void rollYaw(double th) {
  Matrix44 pp;
  pp.v[0][0] = cos(th); pp.v[0][1] = -sin(th); pp.v[0][2] = 0; pp.v[0][3] = 0;
  pp.v[1][0] = sin(th); pp.v[1][1] = cos(th); pp.v[1][2] = 0; pp.v[1][3] = 0;
  pp.v[2][0] = 0; pp.v[2][1] = 0; pp.v[2][2] = 1; pp.v[2][3] = 0;
  pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
  Matrix44 p = pp * forwardKinematicsWrist;
  p.v[0][3] = forwardKinematicsWrist.v[0][3];
  p.v[1][3] = forwardKinematicsWrist.v[1][3];
  p.v[2][3] = forwardKinematicsWrist.v[2][3];

  /*
  std::vector<double> tgt = inverse_kinematics_wrist(p);
  std::vector<double> jointPos;
  for(int i = 0;i < 6;i++) {
    jointPos.push_back(joint_pos[i]);
  }
  tgt = joint_filter2(jointPos, tgt);

  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back(fabs(tgt[i] - joint_pos[i]) / step_time);
  }
  udero->moveJoints(tgt, vel);
  */
  udero->moveCartesianWrist(p, step_time);
}

void rollRoll(double th) {
  std::vector<double> tgt;
  for(int i = 0;i < 6;i++) {
    tgt.push_back(joint_pos[i]);
  }
  tgt[5] = tgt[5] + th;
  std::vector<double> vel;
  size_t s = tgt.size();
  for (int i = 0; i < s; i++) {
    vel.push_back((tgt[i] - joint_pos[i]) / step_time);
  }
  udero->moveJoints(tgt, vel);
}



void quickStop() {
  udero->quickStop();
}


void move_arm() {
  if (button[10] && !button_old[10]) {
    udero->foldOut();
  } else {

  }

  if (button[11] && !button_old[11]) {
    udero->foldIn();
  } else {

  }

  /// For ECM test

  if (button[2] && !button_old[2]) {
    while(true) {
      udero->foldIn();
      udero->foldOut();
    }
  }

  if (button[5]) {
    if (axis[1] < -40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollPitch(astep);
    }
    else if (axis[1] > 40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollPitch(-astep);
    }
    else if (axis[0] < -40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollYaw(astep);
    }
    else if (axis[0] > 40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollYaw(-astep);
    }   
    else if (axis[4] < -40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollRoll(astep);
    }
    else if (axis[4] > 40) {
      if (!moving) { 
	moving = true;
	save_mat();
      }
      rollRoll(-astep);
    } else {
      if (moving) {
	moving = false;
	quickStop();
      }
    }
      
  }
  else if (axis[1] < -40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(tstep, 0, 0);
  }
  else if (axis[1] > 40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(-tstep, 0, 0);
  }
  else if (axis[0] < -40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(0, tstep, 0);
  }
  else if (axis[0] > 40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(0, -tstep, 0);
  }

  else if (axis[4] < -40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(0, 0, tstep);
  }
  else if (axis[4] > 40) {
    if (!moving) { 
      moving = true;
      save_mat();
    }
    transXYZ(0, 0, -tstep);
  } else {
    if (moving) {
      moving = false;
      quickStop();
    }
  }


  if (button[0]) {
    udero->moveHandRel(hstep, hspd);
  } else if (button[1]) {
    udero->moveHandRel(-hstep, hspd);
  }
}

