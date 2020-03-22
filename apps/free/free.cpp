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
    if (argc == 2) {
      int id = atoi(argv[1].c_str());
      if (id == 4 || id == 5) {
	udero->setJointMode(4, technotools::MODE_INACTIVE);
	ssr::Thread::Sleep(100);
	udero->setJointMode(5, technotools::MODE_INACTIVE);
	ssr::Thread::Sleep(100);
      }
      else {
	udero->setJointMode(id, technotools::MODE_INACTIVE);
      }
      
    }
    else {
      for(int i = 0;i < 7;i++) {
	udero->setJointMode(i, technotools::MODE_INACTIVE);
      }
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
