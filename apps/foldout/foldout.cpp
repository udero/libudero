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

int main(const int argc, const char* argv[]) {
  try {
    UderoConnectionProfile prof = parseArgs(argc, argv);
    IUdero* udero = createUdero(prof);
    
    ssr::Thread::Sleep(1000);
    udero->foldOut(true);
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
