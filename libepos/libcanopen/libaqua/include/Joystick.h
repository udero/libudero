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


#ifdef USE_SFML
#include <SFML/Window/Joystick.hpp>
#else

#ifdef WIN32



#elif LINUX

#include <iomanip>
#include <vector>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/joystick.h>

#endif

#endif

namespace ssr { 


  class JoystickNotFoundException : public std::exception {

  };

  class Joystick {
  public:
    std::vector<float> axis;
    std::vector<bool> buttons;
    std::vector<bool> old_buttons;

#ifdef USE_SFML
  private:
    int m_id;
  public:
    Joystick(const int id) {
      m_id = id;
      sf::Joystick::update();
      if (!sf::Joystick::isConnected(id)) {
	std::cout << "[RTC::SFMLJoystick] Joystick (id=" << id << ") not found." << std::endl;
	throw new JoystickNotFoundException();
      }
      axis.resize(8,0);
      buttons.resize(12,0);
      old_buttons.resize(12,0);
    }

#elif LINUX
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
#ifdef USE_SFML
      sf::Joystick::update();
      if (!sf::Joystick::isConnected(m_id)) {
	std::cout << "[RTC::SFMLJoystick] Joystick (id=" << m_id << ") not found." << std::endl;
	throw new JoystickNotFoundException();
      }
      
      bool buttonStateUpdated = false;
      for (size_t i = 0; i < sf::Joystick::getButtonCount(m_id); i++) {
	old_buttons[i] = buttons[i];
	buttons[i] = sf::Joystick::isButtonPressed(m_id, i);
	buttonStateUpdated |= buttons[i] != old_buttons[i];
      }
      
      axis[0] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::X);
      axis[1] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Y);
      axis[2] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::Z);
      
      axis[3] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::R);
      axis[4] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::U);
      axis[5] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::V);
      
      axis[6] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::PovX);
      axis[7] = sf::Joystick::getAxisPosition(m_id, sf::Joystick::PovY);
      
#elif LINUX
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
