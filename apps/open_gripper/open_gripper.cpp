#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"

using namespace technotools;


int main(const int argc_, const char* argv_[]) {
  try {
    UderoConnectionProfile prof = parseArgs(argc_, argv_);
    IUdero* udero = createUdero(prof);
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;

    udero->moveHand(0, 10);
    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -1;
  }
  return 0;
}
