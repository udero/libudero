#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;

void homing_wrist(IUdero* udero, int id, int force);

enum RESULT_T {
  ALREADY_HOMED = 1,
  EXCEPTION_OCCURRED = 2,
};

int main(const int argc, const char* argv[]) {
  try {
      technotools::initLogger(argc, argv);
    std::cout << "Udero Homing version 1.0.1" << std::endl;
    UderoConnectionProfile prof = parseArgs(argc, argv);
    IUdero* udero = createUdero(prof);
    
    ssr::Thread::Sleep(200);
    int argc_ = prof.unknown_args.size();
    std::vector<std::string> argv_ = prof.unknown_args;
    
    for(int i = 0;i < argc_;i++) {
      std::cout << "arg = " << argv_[i] << std::endl;
    }
    if (argc_ >= 2) {
      
      int id = atoi(argv_[1].c_str());
      int force = 0;
      if (argc_ >= 3) {
	force = atoi(argv_[2].c_str());
      //      std::cout << "Now start homing for joint(" << id << ")" << std::endl;
      }

      int wait = 1;
      if (argc_ >= 4) {
	wait = atoi(argv_[3].c_str());
      }
      
      int here_is_zero = 0;
      if (argc_ >= 5) {
	here_is_zero = atoi(argv_[4].c_str());
	if (here_is_zero) {
	  std::cout << "here is " <<here_is_zero << std::endl;
	}
      }
	
      if (id == 4 || id == 5) {
	udero->setJointMode(id, technotools::MODE_POSITION);
	udero->spin();
	//ssr::Thread::Sleep(100);
	//udero->setJointMode(5, technotools::MODE_POSITION);
	udero->spin();
	ssr::Thread::Sleep(100);
	homing_wrist(udero, id, force);
      }
      else {
	udero->spin();
	if (udero->isJointHomed(id) && (!force) ) {
	  return -ALREADY_HOMED;
	} else {
	  std::cout << "Set Joint Mode " << std::endl;
	  udero->setJointMode(id, technotools::MODE_POSITION);
	  if (here_is_zero) {
	    std::cout << "Set Current Position as Zero" << std::endl;
	    udero->setJointHomingMode(id, technotools::ABSOLUTE_POSITION);
	  }
	  udero->goHomeJoint(id);
	}
      }
      
    }
    else {
      udero->goHome(false);
      udero->spin();
      ssr::Thread::Sleep(100);
    }
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "udero2:homing: Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}


void homing_wrist(IUdero* udero, int id, int force) {
    if (id == 4) {
      if (udero->isJointHomed(4) && !force) {
	std::cout << "Already Homed" << std::endl;
	return;
      }
        udero->setJointMode(5, technotools::MODE_VELOCITY);
	udero->spin();
        udero->setJointAcceleration(5, 10);	
	udero->spin();
        udero->goHomeJoint(4);
	udero->spin();
        while (1) {
            udero->moveJointVelocity(5, udero->getJointActualVelocity(4));
	    udero->spin();
            if (udero->isJointHomed(4)) {
                break;
            }
            ssr::Thread::Sleep(100);
        }
        udero->quickStopJoint(5);
	udero->spin();
    }
    else {
      if (udero->isJointHomed(5) && !force) {
	std::cout << "Already Homed" << std::endl;
	return;
      }

        udero->setJointMode(4, technotools::MODE_VELOCITY);
	udero->spin();
        udero->setJointAcceleration(4, 10);
	udero->spin();
        udero->goHomeJoint(5);
	udero->spin();
        while (1) {
            udero->moveJointVelocity(4, -udero->getJointActualVelocity(5));
	    udero->spin();
            if (udero->isJointHomed(5)) {
                break;
            }
            ssr::Thread::Sleep(100);
        }
        udero->quickStopJoint(4);
	udero->spin();
    }
}
