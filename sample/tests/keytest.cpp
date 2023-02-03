#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

#include "Thread.h"
#include "udero/Udero.h"
//#include "kinematics.h"
#include "Console.h"
#define KBHIT ssr::myKbhit
#define GETCH ssr::myGetch


#ifdef WIN32
#include <SFML/Window/Joystick.hpp>
#else
#endif

using namespace reharo;

int id = 0;
int axis[8];
int button[12];
int button_old[12];

IUdero* udero;

double joint_pos[7];

int key;

enum AXIS {
  LX,
  LY,
  RX,
  RY,
  LZ,
  RZ,
  HX,
  HY,
};


int update_key() {
  key = -1;
  if (KBHIT()) {
    key = GETCH();
    if (key == 0x1b) { return -1; } // ESC

    key = tolower(key);
  }
  return 0;
}

static int init;

void move_arm() {
  //  ssr::clear_scr();
  std::cout << "KEY: " << key << "/" << (char)key << std::endl;
	
  if (key == 'i') {
    std::cout << "Udero.init()" << std::endl;
    udero->init();
    init = 0;
  }

  if (key == 'h') {
    udero->goHome();
  }
	
  if (key == 'f') {
    udero->foldIn();
  }
	
  if (key == 'o') {
    udero->foldOut();
  }

  if(key == 's') {
    for (int i = 0; i < 7; i++) {
      udero->setJointMode(i, MODE_INACTIVE);
    }
  }


  std::vector<double> poss;
  for (int i = 0; i < 7; i++) {
    joint_pos[i] = udero->getJointPosition(i);
    poss.push_back(joint_pos[i]);
  }

  for (int i = 0; i < 7; i++) {
    std::cout << "J[" << i + 1 << "]: " << joint_pos[i] << std::endl;
  }


  Matrix44 currentPose = forward_kinematics(poss);

  std::cout << "Current:" << str(currentPose) << std::endl;
	
  if (key == 'l') {
    currentPose.v[1][3] = currentPose.v[1][3] + 10;
    std::vector<double> tgt = inverse_kinematics(currentPose);

    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }

    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }

  if (key == 'r') {
    currentPose.v[1][3] = currentPose.v[1][3] - 10;
    std::vector<double> tgt = inverse_kinematics(currentPose);

    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }

    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }

  if (key == 'u') {
    currentPose.v[2][3] = currentPose.v[2][3] + 10;
    std::vector<double> tgt = inverse_kinematics(currentPose);

    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }

    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }

  if (key == 'd') {
    currentPose.v[2][3] = currentPose.v[2][3] - 10;
    std::vector<double> tgt = inverse_kinematics(currentPose);

    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }

    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }

  if (key == 'e') {
    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    poss[4] = poss[4] + 0.1;
    poss.pop_back();
    udero->moveJoints(poss, vel);
  }

  if (key == 'c') {
    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    poss[4] = poss[4] - 0.1;
    poss.pop_back();
    udero->moveJoints(poss, vel);
  }


  if (key == 'y') {
    currentPose = forward_kinematics_wrist(poss);
    Matrix44 pp;
    double th = 10.0 / 180.0 * M_PI;
    pp.v[0][0] = cos(th); pp.v[0][1] = -sin(th); pp.v[0][2] = 0; pp.v[0][3] = 0;
    pp.v[1][0] = sin(th); pp.v[1][1] = cos(th); pp.v[1][2] = 0; pp.v[1][3] = 0;
    pp.v[2][0] = 0; pp.v[2][1] = 0; pp.v[2][2] = 1; pp.v[2][3] = 0;
    pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
    Matrix44 p = pp * currentPose;
    p.v[0][3] = currentPose.v[0][3];
    p.v[1][3] = currentPose.v[1][3];
    p.v[2][3] = currentPose.v[2][3];
    std::vector<double> tgt = inverse_kinematics_wrist(p);
    tgt = joint_filter2(poss, tgt);

    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }


    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }
  if (key == 'b') {
    currentPose = forward_kinematics_wrist(poss);
    Matrix44 pp;
    double th = -10.0 / 180.0 * M_PI;
    pp.v[0][0] = cos(th); pp.v[0][1] = -sin(th); pp.v[0][2] = 0; pp.v[0][3] = 0;
    pp.v[1][0] = sin(th); pp.v[1][1] = cos(th); pp.v[1][2] = 0; pp.v[1][3] = 0;
    pp.v[2][0] = 0; pp.v[2][1] = 0; pp.v[2][2] = 1; pp.v[2][3] = 0;
    pp.v[3][0] = 0; pp.v[3][1] = 0; pp.v[3][2] = 0; pp.v[3][3] = 1;
    Matrix44 p = pp * currentPose;
    p.v[0][3] = currentPose.v[0][3];
    p.v[1][3] = currentPose.v[1][3];
    p.v[2][3] = currentPose.v[2][3];
    std::vector<double> tgt = inverse_kinematics_wrist(p);
    tgt = joint_filter2(poss, tgt);
    for (int i = 0; i < 6; i++) {
      std::cout << "Target[" << i << "] = " << tgt[i] << std::endl;
    }


    std::vector<double> vel;
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);
    vel.push_back(0.8);

    udero->moveJoints(tgt, vel);
  }


  /*
    if (axis[0] < -40) {
    udero->ppJoints[0]->movePositionWithVelocity(-0.02 + joint_pos[0], 1.5);
    }
    else if (axis[0] > 40) {
    udero->ppJoints[0]->movePositionWithVelocity(0.02 + joint_pos[0], 1.5);
    }
    else {
    //udero->ppJoints[0]->movePositionWithVelocity(joint_pos[0], 1.5);
    }

    if (axis[1] < -40) {
    udero->ppJoints[1]->movePositionWithVelocity(-0.02 + joint_pos[1], 1.5);
    }
    else if (axis[1] > 40) {
    udero->ppJoints[1]->movePositionWithVelocity(0.02 + joint_pos[1], 1.5);
    }
    else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
    }

    if (axis[3] < -40) {
    udero->ppJoints[2]->movePositionWithVelocity(-0.01 + joint_pos[2], 1.5);
    }
    else if (axis[3] > 40) {
    udero->ppJoints[2]->movePositionWithVelocity(0.01 + joint_pos[2], 1.5);
    }
    else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
    }

    if (axis[2] < -40) {
    udero->ppJoints[3]->movePositionWithVelocity(-0.01 + joint_pos[3], 1.5);
    }
    else if (axis[2] > 40) {
    udero->ppJoints[3]->movePositionWithVelocity(0.01 + joint_pos[3], 1.5);
    }
    else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
    }

    if (axis[7] < -40) {
    udero->ppJoints[4]->movePositionWithVelocity(-0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(0.01 + joint_pos[5], 1.5);
    }
    else if (axis[7] > 40) {
    udero->ppJoints[4]->movePositionWithVelocity(0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(-0.01 + joint_pos[5], 1.5);
    }
    else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
    }

    if (axis[6] > 40) {
    udero->ppJoints[4]->movePositionWithVelocity(-0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(-0.01 + joint_pos[5], 1.5);
    }
    else if (axis[6] < -40) {
    udero->ppJoints[4]->movePositionWithVelocity(0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(0.01 + joint_pos[5], 1.5);
    }
    else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
    }


    if (button[0]) {
    udero->ppJoints[6]->movePositionWithVelocity(-0.007 + joint_pos[6], 1.0);
    }
    if (button[1]) {
    udero->ppJoints[6]->movePositionWithVelocity(0.007 + joint_pos[6], 1.0);
    }

  */
}

inline double inc_to_pos(int32_t inc, double gearRatio, double encoderRes) {
  //return inc;
  return ((inc)* 2 * M_PI) / (gearRatio * 4 * encoderRes);
}

int test_move(void) {
  std::cout << "Move testing move." << std::endl;
  double tick = inc_to_pos(1000, 200, 512);
  for (int i = 0; i < 10; i++) {
    //udero->ppJoints[2]->movePositionWithVelocity(tick, 1.0);
    udero->moveJoint(2, tick, 1.0);
    ssr::Thread::Sleep(3 * 1000);
    //udero->ppJoints[2]->movePositionWithVelocity(0, 1.0);
    udero->moveJoint(2, 0, 1.0);
    ssr::Thread::Sleep(3 * 1000);
  }
  return 0;
}

int main(const int argc, const char* argv[]) {
  {
    std::cout << "Starting Udero_keytest" << std::endl;
    
    UderoConnectionProfile prof = parseArgs(argc, argv);
    udero = createUdero(prof);

    std::cout << "Initializing CANUdero" << std::endl;
    udero->init();
    std::cout << "Starting Get Joint Position" << std::endl;
    for (int i = 0; i < 7; i++) {
      joint_pos[i] = udero->getJointPosition(i);
    }

    for (int i = 0; i < 7; i++) {
      udero->setJointMode(i, MODE_POSITION);
      ssr::Thread::Sleep(100);
    }

    for (int i = 0; i < 7; i++) {
      udero->setJointAcceleration(i, 15.0);
    }



    while (true) {

      try {
	ssr::Thread::Sleep(100);
	if (update_key() < 0) return -1;
	move_arm();

      }
      catch (std::exception& ex) {
	std::cout << "Exception: " << ex.what() << std::endl;
      }
    }

    

  }

  delete udero;

  return 0;
}


