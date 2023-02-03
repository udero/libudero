#include <iostream>
#include <iomanip>
#include <vector>
//#include <cstdio>
//#include <fcntl.h>
//#include <unistd.h>
//#include <stdlib.h>
//#include <sys/ioctl.h>
#include "Joystick.h"
#include "Thread.h"
//#define JOY_DEV "/dev/input/js0"

using namespace std;

int main()
{

  try {
  ssr::Joystick joy(0);

  while(true)
  {
    joy.update();

    cout<<"axis/10000: ";
    for(size_t i(0);i<joy.axis.size();++i)
      cout<<" "<<setw(2)<<joy.axis[i]/300.0;
    cout<<endl;

    cout<<"  button: ";
    for(size_t i(0);i<joy.buttons.size();++i)
      cout<<" "<<(int)joy.buttons[i];
    cout<<endl;

    ssr::Thread::Sleep(10);
    //system("clear");
  }
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
  }
  //close(joy_fd);
  return 0;
}
