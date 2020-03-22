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

    if (argv.size() != 3) {
      return -1;
    }

    udero->moveHand(atof(argv[1].c_str())*1000-80, atof(argv[2].c_str())*1000);
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -1;
  }
  return 0;
}
