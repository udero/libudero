#include <iostream>


#include "devices/abs.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const int id = argv[2][0] - '0';

  int32_t angle = 0;

  if (reharo::abs_check(filename, id)) {
	  std::cout << "ABS (id=" << id  << ") is OK." << std::endl;
	  return 0;
  }
  else {
	  std::cout << "ABS (id=" << id << ") is FAILED." << std::endl;
	  return -1;
  }
  return 0;
}
