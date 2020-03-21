#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"

using namespace reharo;

void homing_wrist(IUdero* udero, int id);

enum RESULT_T {
  ALREADY_HOMED = 1,
  EXCEPTION_OCCURRED = 2,
};

int main(const int argc_, const char* argv_[]) {
  try {
    UderoConnectionProfile prof = parseArgs(argc_, argv_);
    IUdero* udero = createUdero(prof);
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);
    udero->spin();
    if (argc == 2) {
      int id = atoi(argv[1].c_str());
      if (id == 4 || id == 5) {
	std::vector<double> poss;
	
	std::cout << udero->getJointPosition(id) << std::endl;
      }
      else {
	std::cout << udero->getJointPosition(id) << std::endl;
      }
      
    }
    else {
      std::vector<double> poss;
      for(int i = 0;i < 7;i++) {
	std::cout << udero->getJointPosition(i);
	if (i != 6) { 
	  std::cout << ", ";
	}
      }
      std::cout << std::endl;
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
        udero->setJointMode(5, reharo::MODE_VELOCITY);
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
        udero->setJointMode(4, reharo::MODE_VELOCITY);
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
