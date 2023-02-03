#include <iostream>


#include "devices/ledmatrix.h"
using namespace reharo;

int main(const int argc, const char* argv[]) {
  //  std::cout << "LED Matrix Test" << std::endl;

  const char* filename = argv[1];
  const char mark = argv[2][0];


  if (mark == 'b') {
    const int hz = atoi(argv[3]);
    ledmatrix_buzz(filename, hz);

    return 0;
  } else if (mark == 'c') {
    const char c = argv[3][0];
    ledmatrix_color(filename, c);
  }

  switch (mark) {
  case '.':
    ledmatrix_write(filename, DOT);
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
  case '7':
    ledmatrix_write(filename, SEVEN);
    break;
  case '8':
    ledmatrix_write(filename, EIGHT);
    break;
  case '9':
    ledmatrix_write(filename, NINE);
    break;
  case '0':
    ledmatrix_write(filename, ZERO);
    break;
  case '_':
    ledmatrix_write(filename, OFF);
    break;
  case 'A':
    ledmatrix_write(filename, A);
    break;
  case 'B':
    ledmatrix_write(filename, B);
    break;
  case 'C':
    ledmatrix_write(filename, C);
    break;
  case 'D':
    ledmatrix_write(filename, D);
    break;
  case 'E':
    ledmatrix_write(filename, E);
    break;
  case 'F':
    ledmatrix_write(filename, F);
    break;
  case 'G':
    ledmatrix_write(filename, G);
    break;
  case 'H':
    ledmatrix_write(filename, H);
    break;
  case 'I':
    ledmatrix_write(filename, I);
    break;
  case 'J':
    ledmatrix_write(filename, J);
    break;
  case 'K':
    ledmatrix_write(filename, K);
    break;
  case 'L':
    ledmatrix_write(filename, L);
    break;
  case 'M':
    ledmatrix_write(filename, M);
    break;
  case 'N':
    ledmatrix_write(filename, N);
    break;
  case 'O':
    ledmatrix_write(filename, O);
    break;
  case 'P':
    ledmatrix_write(filename, P);
    break;
  case 'Q':
    ledmatrix_write(filename, Q);
    break;
  case 'R':
    ledmatrix_write(filename, R);
    break;
  case 'S':
    ledmatrix_write(filename, S);
    break;
  case 'T':
    ledmatrix_write(filename, T);
    break;
  case 'U':
    ledmatrix_write(filename, U);
    break;
  case 'V':
    ledmatrix_write(filename, V);
    break;
  case 'W':
    ledmatrix_write(filename, W);
    break;
  case 'X':
    ledmatrix_write(filename, X);
    break;
  case 'Y':
    ledmatrix_write(filename, Y);
    break;
  case 'Z':
    ledmatrix_write(filename, Z);
    break;
  }
  ssr::Thread::Sleep(100);

  return 0;
}
