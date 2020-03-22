#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;

int main(const int argc_, const char* argv_[]) {
  try {
    technotools::initLogger(argc_, argv_);
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
    UERROR("Exception:%s", ex.what());
    return -1;
  }
  return 0;
}
