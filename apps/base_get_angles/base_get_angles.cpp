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

    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);
    
    std::cout << base->getJointPosition(0) << std::endl;
    deleteUderoBase(base);
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}


#if 0
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
#endif
