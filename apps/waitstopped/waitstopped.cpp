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

int main(const int argc, const char* argv[]) {
  try {
      technotools::initLogger(argc, argv);
    UderoConnectionProfile prof = parseArgs(argc, argv);
    IUdero* udero = createUdero(prof);
    
    ssr::Thread::Sleep(1000);
    while(1) {
      try {
	udero->waitStopped();
	break;
      } catch (std::exception &ex) {
      }
    }
    deleteUdero(udero);
  } catch (std::exception &ex) {
	UERROR("Exception:%s", ex.what());
    std::cout << "Exception: " << ex.what() << std::endl;
    return -EXCEPTION_OCCURRED;
  }
  return 0;
}
