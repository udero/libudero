#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoBase.h"

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
    IUderoBase* base = createUderoBase(udero);

    bool blocking = true;
    bool force = false;
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    if (argc == 1) {
      blocking = argv[1] == "1";
    } else if (argc == 2) {
      blocking = argv[1] == "1";
      force = argv[2] == "1";
    }

    std::cout << "goHome with blocking="<<blocking<<" force="<<force<<std::endl;
    base->goHome(blocking=false, force=false);

    deleteUdero(udero);
    deleteUderoBase(base);
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
