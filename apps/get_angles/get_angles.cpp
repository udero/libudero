#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;

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
    UERROR("Exception:%s", ex.what());
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}
