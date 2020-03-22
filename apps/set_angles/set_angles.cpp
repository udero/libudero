#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;

void homing_wrist(IUdero* udero, int id);

enum RESULT_T {
  ALREADY_HOMED = 1,
  EXCEPTION_OCCURRED = 2,
};

int main(const int argc_, const char* argv_[]) {
  try {
      technotools::initLogger(argc_, argv_);
    UderoConnectionProfile prof = parseArgs(argc_, argv_);
    IUdero* udero = createUdero(prof);
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);
    if (argc == 4) {
      int id = atoi(argv[1].c_str());
      double pos = atof(argv[2].c_str());
      double vel = atof(argv[3].c_str());
      if (id == 4 || id == 5) {
	udero->setJointMode(4, technotools::MODE_POSITION);
	ssr::Thread::Sleep(100);
	udero->setJointMode(5, technotools::MODE_POSITION);
	ssr::Thread::Sleep(100);
	udero->moveJoint(id, pos, vel);
      } else if (id == 6) {
	udero->moveHand(pos, vel);
      }
      else {
	udero->setJointMode(id, technotools::MODE_POSITION);
	ssr::Thread::Sleep(100);
	udero->moveJoint(id, pos, vel, true);
      }
    } else if (argc == 1+6+1) {
      std::cout << "moveJoints" << std::endl;
      std::vector<double> pos;
      std::vector<double> vel;
      double v = atof(argv[7].c_str());
      for(int i = 0;i < 6;i++) {
	pos.push_back(atof(argv[1+i].c_str()));
	vel.push_back(v);
      }
      udero->moveJoints(pos, vel);
    } else {
      return -3;
    }
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}


void homing_wrist(IUdero* udero, int id) {
    if (id == 4) {
        udero->setJointMode(5, technotools::MODE_VELOCITY);
        udero->setJointAcceleration(5, 10);
        udero->goHomeJoint(4);
        while (1) {
            udero->moveJointVelocity(5, udero->getJointActualVelocity(4));
            if (udero->isJointHomed(4)) {
                break;
            }
            ssr::Thread::Sleep(100);
        }
        udero->quickStopJoint(5);
    }
    else {
        udero->setJointMode(4, technotools::MODE_VELOCITY);
        udero->setJointAcceleration(4, 10);
        udero->goHomeJoint(5);
        while (1) {
            udero->moveJointVelocity(4, -udero->getJointActualVelocity(5));
            if (udero->isJointHomed(5)) {
                break;
            }
            ssr::Thread::Sleep(100);
        }
        udero->quickStopJoint(4);
    }
}
