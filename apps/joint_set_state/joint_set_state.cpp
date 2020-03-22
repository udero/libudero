#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;




int main(const int argc, const char* argv[]) {
  try {
      technotools::initLogger(argc, argv);
    std::cout << "Udero Joint State version 1.0.0" << std::endl;
    UderoConnectionProfile prof = parseArgs(argc, argv);

    IUdero *udero;
    while(true) {
      try {
	udero = createUdero(prof);
	break;
      } catch (std::exception &ex) {
	std::cout << __FILE__ << "Exception in initialize Udero:" << ex.what() << std::endl;
      }
    }
    ssr::Thread::Sleep(200);

    int argc_ = prof.unknown_args.size();
    std::vector<std::string> argv_ = prof.unknown_args;
    for(int i = 0;i < argc_;i++) {
      std::cout << __FILE__ << "Commandline arg = " << argv_[i] << std::endl;
    }


    if (argc_ == 3) {
      int i = atoi(argv[1]);
      int s = atoi(argv[2]);
      JOINT_STATE js =udero->getJointState(i);
      if (js == ENABLED) std::cout << i << ":ENABLED" << std::endl;
      else if (js == DISABLED) std::cout << i << ":DISABLED" << std::endl;
      else {
	std::cout <<i << ":FAULT" << std::endl;
	udero->faultResetJoint(i);
      }
    } else {
      for(int i = 0;i < 7;i++) {
	JOINT_STATE js =udero->getJointState(i);
	if (js == ENABLED) std::cout << i << ":ENABLED" << std::endl;
	else if (js == DISABLED) std::cout << i << ":DISABLED" << std::endl;
	else {
	  std::cout <<i << ":FAULT" << std::endl;
	  udero->faultResetJoint(i);
	}
      }
    }

    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << __FILE__ << "Exception: " << ex.what() << std::endl;
    std::cout << __FILE__ << "Error Exit(-1)" << std::endl;
    UERROR("Exception:%s", ex.what());
    return -1;
  }
  return 0;
}

