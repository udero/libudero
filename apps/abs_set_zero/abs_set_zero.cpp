#include <iostream>


#include "devices/abs.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const char id = argv[2][0] - '0';

  if (reharo::abs_set_zero(filename, id)) {
	  std::cout << "ABS (id =" << id + '0' << ") set zero " << std::endl;
  }
  return 0;
}
