#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"

using namespace technotools;

int main(const int argc_, const char* argv_[]) {
  try {
    UderoConnectionProfile prof = parseArgs(argc_, argv_);
    IUdero* udero = createUdero(prof);
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);
    if (argc == 2) {
      int id = atoi(argv[1].c_str());
      std::cout << 	udero->getJointDigitalInput(id) << std::endl;
    }
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -1;
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
