#include <iostream>


#include "ledmatrix.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  std::cout << "LED Matrix Test" << std::endl;

  const char* filename = argv[1];
  const char mark = argv[2][0];

#ifndef WIN32
  switch (mark) {
  case '1':
    ledmatrix_write(filename, ONE);
    break;
  case '2':
    ledmatrix_write(filename, TWO);
    break;
  case '3':
    ledmatrix_write(filename, THREE);
    break;
  case '4':
    ledmatrix_write(filename, FOUR);
    break;
  case '5':
    ledmatrix_write(filename, FIVE);
    break;
  case '6':
    ledmatrix_write(filename, SIX);
    break;
  case '_':
    ledmatrix_write(filename, OFF);
    break;
  case 'H':
    ledmatrix_write(filename, H);
    break;
  }
  ssr::Thread::Sleep(100);
#endif
  return 0;
}
