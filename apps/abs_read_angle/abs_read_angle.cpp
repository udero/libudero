#include <iostream>


#include "devices/abs.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const int id = argv[2][0] - '0';

  int32_t angle = 0;
  reharo::abs_read(filename, id, &angle);
    std::cout << "ABS (id=" << id  << ") angle is " << angle << std::endl;
    //std::cout << angle << std::endl;
  return 0;
}
