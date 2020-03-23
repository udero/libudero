#include <stdlib.h>

#include <iostream>
#include <iomanip>
#include <vector>
#include "Thread.h"
#include "Joystick.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

#ifdef WITH_LEDMATRIX
#include "devices/ledmatrix.h"
#else
// Without LEDMatrix
#define ledmatrix_write(x, y)
#define ledmatrix_buzz(x, y)
#endif

using namespace technotools;
const char* filename = "/dev/ttymxc4";

void debug_joy(ssr::Joystick* joy) {
  std::cout<<"axis: ";
  for(size_t i=0;i<joy->axis.size();++i)
    std::cout<<" "<<std::setw(2)<<joy->axis[i];
  std::cout<<std::endl;

  std::cout<<"  button: ";
  for(size_t i=0;i<joy->buttons.size();++i)
    std::cout<<" "<<(int)joy->buttons[i];
  std::cout<<std::endl;
}


void debug_udero(technotools::IUdero* udero) {
  for(int i = 0;i < 6;i++) {
    std::cout << udero->getJointPosition(i);
    if (i != 5) { 
      std::cout << ", ";
    }
  }
  std::cout << std::endl;
}

double update_joint(const double& input, const double& pos, const double threshold, const double e, bool* update) {
  if (input < -threshold) {
    *update = true;
    return pos - e;
  } else if (input > threshold) {
    *update = true;
    return pos + e;
  }
  return pos;
}


bool loop(technotools::IUdero* udero, ssr::Joystick* joy, bool moving[7], bool* folding, int *speed_mode, int max_speed_mode, double* joint_step, double* joint_vel, double* wrist_position_when_pushed) {
  try {
    joy->update();
  } catch (std::exception& ex) {
    std::cout << __FILE__ << "Exception occurred in Joystick::update():" << ex.what() << std::endl;
    throw ex;
  }
 
  std::vector<double> pos, vel;
  try {
    udero->spin();
    for(int i = 0;i < 6;i++) {
      pos.push_back(udero->getJointPosition(i));

    }
  } catch (std::exception& ex) {
    std::cout << __FILE__ << "Exception occurred in Udero::spin():" << ex.what() << std::endl;
    throw ex;
  }

  double axis_threshold = 0.3;
  double e = joint_step[*speed_mode];
  for(int i = 0;i < 6;i++) {
    vel.push_back(joint_vel[*speed_mode]);
  }
  bool joint_update[7] = {false, false, false, false, false, false, false};
  bool update = false;


  double inputs[] = {-joy->axis[0], joy->axis[1], joy->axis[4], joy->axis[3], joy->axis[6], joy->axis[5]};
  /// UPDATE JOINT
  for(int i = 0;i < 4;i++) {
    pos[i] = update_joint(inputs[i], pos[i], axis_threshold, e, &(joint_update[i]));
    update = update || joint_update[i];
  }

  /// FOR WRIST MOVE
  if (inputs[4] > axis_threshold || inputs[4] < -axis_threshold) {
    int i = 4;
    if (!moving[i]) {
      *wrist_position_when_pushed = pos[5];
    }
    pos[i] = update_joint(inputs[i], pos[i], axis_threshold, e, &(joint_update[i]));
    update = update || joint_update[i];
    pos[5] = *wrist_position_when_pushed;
  } else if (inputs[5] > axis_threshold || inputs[5] < -axis_threshold) {
    int i = 5;
    if (!moving[i]) {
      *wrist_position_when_pushed = pos[4];
    }
    pos[i] = update_joint(inputs[i], pos[i], axis_threshold, e, &(joint_update[i]));
    update = update || joint_update[i];
    pos[4] = *wrist_position_when_pushed;
  }


  if (joy->buttons[1]) {
    (*speed_mode)++;
    if(*speed_mode >= max_speed_mode) {
      *speed_mode = 0;
    }
    if (*speed_mode == 0) 
      ledmatrix_write(filename, ZERO);
    else if (*speed_mode == 1)
      ledmatrix_write(filename, ONE);
    else
      ledmatrix_write(filename, TWO);
  }
  
  /// FOLD
  if (joy->buttons[10] && !joy->buttons[11]) { // foldIn
    std::cout << "FoldIn" << std::endl;
    bool isFault = false;
    for(int i = 0;i < 7;i++) {
      JOINT_STATE js =udero->getJointState(i);
      if (js == FAULT) {
	std::cout << "Joint:" << i << " is FAULT." << std::endl;
	ledmatrix_write(filename, E);
	udero->faultResetJoint(i);
	isFault = true;
	udero->setJointMode(i, MODE_POSITION);
      }
    }
    
    
    ledmatrix_write(filename, I);
    udero->moveHand(0, 10);
    udero->foldIn(true);
    ledmatrix_write(filename, S);
    ssr::Thread::Sleep(500);
    *folding = true;
  } else if (joy->buttons[11] && !joy->buttons[10]) {
    std::cout << "FoldOut" << std::endl;
    ledmatrix_write(filename, O);
    udero->foldOut(true);
    ledmatrix_write(filename, A);
    ssr::Thread::Sleep(500);
    *folding = false;

    if (*speed_mode == 0) 
      ledmatrix_write(filename, ZERO);
    else if (*speed_mode == 1)
      ledmatrix_write(filename, ONE);
    else
      ledmatrix_write(filename, TWO);
  }

  if (joy->buttons[4] && !joy->buttons[5]) {
    for(int i = 0;i < 7;i++) {
      udero->setJointMode(i, technotools::MODE_INACTIVE);
    }
  } else if (joy->buttons[5] && !joy->buttons[4]) {
    for(int i = 0;i < 7;i++) {
      udero->setJointMode(i, technotools::MODE_POSITION);
    }
  }

  bool fault = false;
  for(int i = 0;i < 7;i++) {
    JOINT_STATE js =udero->getJointState(i);
    if (js == FAULT) {
      std::cout << "Joint:" << i << " is FAULT." << std::endl;
      fault = true;
      moving[i] = false;

      if (i == 6) {
	ledmatrix_buzz(filename, 400);
	ssr::Thread::Sleep(500);
	ledmatrix_buzz(filename, 0);
	bool reset = false;
	while(!reset) {
	  try {
	    udero->faultResetJoint(6);
	    udero->setJointMode(6, MODE_POSITION);
	    reset = true;
	  } catch (std::exception& ex) {
	    std::cout << "Exception in fault reset." << std::endl;
	    std::cout << ex.what() << std::endl;
	  }
	}
      }
    } 

  }
  if (fault) {
    return true;
  }
    
  if (*folding) {
    return false;
  }
  
  try {
    if (joy->buttons[2] && !joy->buttons[3]) { // open
      udero->moveHandRel(10, 10.0);
    } else if (joy->buttons[3] && !joy->buttons[2]) { // close
      udero->moveHandRel(-10, 10.0);
    } 
  } catch (std::exception& ex) {
    std::cout << "Detect Exception in Hand Moving." << std::endl;
    std::cout << ex.what() << std::endl;
    
    udero->faultResetJoint(6);
    udero->setJointMode(6, MODE_POSITION);
  }


  if (update) {
    udero->moveJoints(pos, vel);
  }

  for(int i = 0;i < 6;i++) {
    if (joint_update[i]) {
      moving[i] = true;
    } else if (moving[i]) {
      udero->quickStopJoint(i);
      moving[i] = false;
    } 
  }

  return false;
}


int main(const int argc, const char* argv[]) {
  try {
      technotools::initLogger(argc, argv);
    std::cout << "Udero JoyJointControl version 1.0.3" << std::endl;
    UderoConnectionProfile prof = parseArgs(argc, argv);
    ledmatrix_write(filename, J);
    ssr::Joystick* joy;
    while(true) {
      try {
#ifdef WIN32
        joy = new ssr::Joystick();
#else
	    joy = new ssr::Joystick("/dev/input/js0");
#endif
	break;
      } catch (std::exception& ex) {
	    std::cout << __FILE__ << "Exception in initializing Joystick:" << ex.what() << std::endl;
      }
    }
    ledmatrix_write(filename, U);
    IUdero *udero;
    while(true) {
      try {
	    udero = createUdero(prof);
	    break;
      } catch (std::exception &ex) {
	    std::cout << __FILE__ << "Exception in initialize Udero:" << ex.what() << std::endl;
      }
    }
    ssr::Thread::Sleep(200);

    bool do_not_foldin = false;
    int argc_ = prof.unknown_args.size();
    std::vector<std::string> argv_ = prof.unknown_args;
    for(int i = 0;i < argc_;i++) {
      std::cout << __FILE__ << "Commandline arg = " << argv_[i] << std::endl;
    }
    if (argc_ >= 2) {
      if (argv_[1] == "n") {
	    do_not_foldin = true;
      }
    }

    bool folding = false;
    if (!do_not_foldin) {
      ledmatrix_write(filename, I);
      udero->moveHand(0, 10);
      udero->foldIn();
      folding = true;
      ledmatrix_write(filename, S);
    }
    // ssr::Thread::Sleep(200);
    for(int i = 0;i < 100;i++) {
      joy->update();
    }


    int i = 0;
    double joint_step[3] = {0.05, 0.1, 0.2};
    double joint_vel[3]  = {0.05, 0.1, 0.2};
    int speed_mode = 1;
    int max_speed_mode = 3;
    bool moving[7] = {false, false, false, false, false, false, false};
    double wrist_position_when_pushed = 0;
    while(true) {
      if (loop(udero, joy, moving, &folding, &speed_mode, max_speed_mode, joint_step, joint_vel, &wrist_position_when_pushed)) {
	    ledmatrix_write(filename, F);
      }
      ssr::Thread::Sleep(1);
      i++;
      if (i % 10 == 0) {
	    debug_joy(joy);
	    debug_udero(udero);
      }

    }


    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << __FILE__ << "Exception: " << ex.what() << std::endl;
    std::cout << __FILE__ << "Error Exit(-1)" << std::endl;
    ledmatrix_write(filename, E);
    UERROR("Exception:%s", ex.what());
    return -1;
  }
  return 0;
}

