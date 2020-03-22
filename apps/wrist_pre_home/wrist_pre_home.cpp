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
    size_t argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);

    udero->setJointMode(4, technotools::MODE_POSITION);
    ssr::Thread::Sleep(100);
    udero->setJointMode(5, technotools::MODE_POSITION);
    ssr::Thread::Sleep(100);
    
    bool initial = (udero->getJointDigitalInput(5) & 0x0002) > 0;
    std::cout << "initial : " << initial << std::endl;

    std::vector<double> poss;
    std::vector<double> vel;
    for(int i = 0;i < 7;i++) {
      poss.push_back(udero->getJointPosition(i));
      vel.push_back(0.2);
    }

    double target = poss[4];
    bool current = false;
    std::cout << " - initialPosiition:" << target << std::endl;
    while(true) {
      current = (udero->getJointDigitalInput(5) & 0x0002) > 0;
      std::cout << "current : " << current << std::endl;
      if((!current && initial) || (current && !initial)) break;

      target += (initial ? 1 : -1) * 0.1;
      std::cout << " - target = " << target << std::endl;
      poss[4] = target;
      udero->moveJoints(poss, vel);
      udero->waitStopped();
    }

    if (current) {
      initial = current;
      while(true) {
	current = (udero->getJointDigitalInput(5) & 0x0002) > 0;
	std::cout << "current : " << current << std::endl;
	if((!current && initial) || (current && !initial)) break;

	target += (initial ? 1 : -1) * 0.1;
	std::cout << " - target = " << target << std::endl;
	poss[4] = target;
	udero->moveJoints(poss, vel);
	udero->waitStopped();
      }
    }

    deleteUdero(udero);
  } catch (std::exception &ex) {
      UERROR("Exception:%s", ex.what());
      std::cout << "Exception: " << ex.what() << std::endl;
    return -1;
  }
  return 0;
}
