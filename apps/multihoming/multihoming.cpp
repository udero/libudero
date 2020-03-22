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

    udero->spin();
    for(int i = 1;i < argv_.size();i++) {
      int id = atoi(argv_[i].c_str());
      

      if (udero->isJointHomed(id)) {
	return -ALREADY_HOMED;
      } else {
	udero->setJointMode(id, technotools::MODE_POSITION);
	udero->goHomeJoint(id);
      }
      
    }
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}
