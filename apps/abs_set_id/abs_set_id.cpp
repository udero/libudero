#include <iostream>


#include "devices/abs.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const int id = argv[2][0] - '0';
  const int new_id = argv[3][0] - '0';

  if (reharo::abs_set_id(filename, id, new_id)) {
	  std::cout << "ABS (id=" << id  << ") set id to " << new_id << std::endl;

  }
  return 0;
}
