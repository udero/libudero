#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "devices/abs.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const int id = argv[2][0] - '0';

  int32_t angle = 0;
  reharo::abs_read(filename, id, &angle);

  if (angle > 16383/2) {
    angle = -(16383 - 255 - angle);
  }

  double folding_angle = -1.3;
  double fangle = angle * 2 * M_PI / 16383;

  if (argc == 4 && argv[3][0] == 'n') {
    std::cout << -fangle << std::endl;
  } else {
    std::cout << fangle << std::endl;
  }
  return 0;
}
