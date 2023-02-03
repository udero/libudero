#include <iostream>
#include "abs.h"


using namespace reharo;


int main(const int argc, const char* argv[]) {
  const char* filename = argv[1];
  const int id = atoi(argv[2]);
  std::string cmd = argv[3];

  try {
    if (cmd == "echo") {
      ABSEncoder a(filename, 9600);
      if (a.echo(id)) {
	std::cout << "OK" << std::endl;
      } else {
	std::cout << "NG" << std::endl;
      }
      
    } else if(cmd == "setid") {
      ABSEncoder a(filename, 9600);
      int newID = atoi(argv[4]);
      if (a.writeID(id, newID)) {
	std::cout << "OK" << std::endl;
      } else {
	std::cout << "NG" << std::endl;
      }
    } else if (cmd == "setzero") {
      ABSEncoder a(filename, 9600);
      if (a.setZero(id)) {
	std::cout << "OK" << std::endl;
      } else {
	std::cout << "NG" << std::endl;
      }
    } else if (cmd == "angle") {
      int angle;
      ABSEncoder a(filename, 9600);
      if(a.readAngle(id, &angle)) {
	std::cout << "OK" << std::endl;
	std::cout << angle << std::endl;
      } else {
	std::cout << "NG" << std::endl;
      }

    }
  } catch (std::exception& e) {
    std::cout << "ER" << std::endl;
  }
  return 0;
}
