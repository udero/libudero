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
    if (argc >= 2) {
      bool flag = true;
      for(int i = 1;i < argv.size();i++) {
	udero->spin();
	int id = atoi(argv[i].c_str());
	flag = flag && udero->isJointHomed(id);
      }
      std::cout << flag << std::endl;
    }
    else {
      for(int i = 0;i < 7;i++) {
	udero->spin();
	std::cout << udero->isJointHomed(i);
	if (i != 6) { 
	  std::cout << ", ";
	}
      }
      std::cout << std::endl;
    }
    ssr::Thread::Sleep(100);
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}


