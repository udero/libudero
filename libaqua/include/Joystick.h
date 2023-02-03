/**********************************************************
 * Joystick.h
 *
 *
 * Portable Class Library for Windows and Unix.
 * @author ysuga@ysuga.net
 * @date 2019/04/12
 ********************************************************/
#pragma once


#include "aqua.h"
#include <exception>


#ifdef WIN32
#pragma comment(lib, "winmm.lib")

#include <windows.h>

#else

#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#endif


namespace ssr { 


  class JoystickNotFoundException : public std::exception {

  };

  class Joystick {
  public:
    std::vector<float> axis;
    std::vector<bool> buttons;
    std::vector<bool> old_buttons;

#ifdef WIN32
  private:
	JOYINFOEX joyInfo_;
    int num_of_axis;
    int num_of_buttons;
  public:
    Joystick() {
      joyInfo_.dwSize = sizeof(JOYINFOEX);
      joyInfo_.dwFlags = JOY_RETURNALL;

      num_of_buttons = 16;
      num_of_axis = 7;
      buttons.resize(num_of_buttons, 0);
      old_buttons.resize(num_of_buttons, 0);
      axis.resize(num_of_axis, 0);
    }
    
    
#else
  private:
    int joy_fd;
    int num_of_axis;
    int num_of_buttons;
    std::vector<char> joy_button;
    std::vector<int> joy_axis;
    char name_of_joystick[80];
  public:
    Joystick(const char* filename): num_of_buttons(0) {
      if((joy_fd=open(filename, O_RDONLY|O_NONBLOCK)) < 0) {
        perror(filename);
        throw new JoystickNotFoundException();
      }
      ioctl(joy_fd, JSIOCGAXES, &num_of_axis);
      ioctl(joy_fd, JSIOCGBUTTONS, &num_of_buttons);
      ioctl(joy_fd, JSIOCGNAME(80), &name_of_joystick);
      
      buttons.resize(num_of_buttons,0);
      old_buttons.resize(num_of_buttons,0);
      axis.resize(num_of_axis,0);
  
      fcntl(joy_fd, F_SETFL, O_NONBLOCK);   // using non-blocking mode
    }
#endif

  public:
    void update() {
#ifdef WIN32
      if(JOYERR_NOERROR != joyGetPosEx(0, &joyInfo_)) {
        return;
      }
      for (size_t i = 0; i < buttons.size(); i++) {
          old_buttons[i] = buttons[i];
          if (joyInfo_.dwButtons & (0x00000001 << i)) {
              buttons[i] = 1;
          }
          else {
              buttons[i] = 0;
          }
      }

      axis[0] = ((float)joyInfo_.dwXpos - 32767.0f) / 32767.0f;
      axis[1] = ((float)joyInfo_.dwYpos - 32767.0f) / 32767.0f;
      axis[3] = ((float)joyInfo_.dwZpos - 32767.0f) / 32767.0f;
      axis[2] = ((float)joyInfo_.dwUpos - 32767.0f) / 32767.0f;
      axis[4] = ((float)joyInfo_.dwRpos - 32767.0f) / 32767.0f;

      switch (joyInfo_.dwPOV) {
      case 0:
          axis[5] = 0.0f;
          axis[6] = -1.0f;
          break;
      case 9000:
          axis[5] = +1.0f;
          axis[6] = 0.0f;
          break;
      case 18000:
          axis[5] = 0.0f;
          axis[6] = +1.0f;
          break;
      case 27000:
          axis[5] = -1.0f;
          axis[6] = 0.0f;
          break;
      case 4500:
          axis[5] = +1.0f;
          axis[6] = -1.0f;
          break;
      case 13500:
          axis[5] = +1.0f;
          axis[6] = +1.0f;
          break;
      case 22500:
          axis[5] = -1.0f;
          axis[6] = +1.0f;
          break;
      case 31500:
          axis[5] = -1.0f;
          axis[6] = -1.0f;
          break;
      default:
          axis[5] = 0.0f;
          axis[6] = 0.0f;
          break;
      }

      //printf("axis");
      //for (int i = 0; i < num_of_axis; i++) {
      //    std::cout << axis[i] << ",";
      //}
      //std::cout << std::endl;
#else
      js_event js;
      read(joy_fd, &js, sizeof(js_event));
      switch (js.type & ~JS_EVENT_INIT) {
      case JS_EVENT_AXIS:
        if((int)js.number>=axis.size())  {std::cerr<<"Joystick()::update Error (Read Joystick Event Number is larger than Joystick Axis Size.(number/size)"<<(int)js.number << "/" << (int)axis.size()<<std::endl;}
        axis[(int)js.number]= js.value / 32767.0;
        break;
      case JS_EVENT_BUTTON:
        if((int)js.number>=buttons.size())  {std::cerr<<"err:"<<(int)js.number<<std::endl;}
        old_buttons[(int)js.number] = buttons[(int)js.number];
        buttons[(int)js.number]= js.value;
        break;
      }
#endif
    }
  };
  
};
