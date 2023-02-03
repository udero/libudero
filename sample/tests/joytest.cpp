#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"

#ifdef WIN32
#include <SFML/Window/Joystick.hpp>
#else

#endif

#define _USE_MATH_DEFINES
#include <math.h>

#include <conio.h>

using namespace reharo;

int id = 0;
int axis[8];
int button[12];
int button_old[12];
UderoConnectionProfile prof;
IUdero* udero;

double joint_pos[7];

enum AXIS {
  LX,
  LY,
  RX,
  RY,
  LZ,
  RZ,
  HX,
  HY,
};


int update_joystick() {
  sf::Joystick::update();
  if (!sf::Joystick::isConnected(id)) {
    std::cout << "[RTC::SFMLJoystick] Joystick (id=" << id << ") not found." << std::endl;
    return -1;
  }

  bool buttonStateUpdated = false;
  for (size_t i = 0; i < sf::Joystick::getButtonCount(id); i++) {
    button_old[i] = button[i];
    button[i] = sf::Joystick::isButtonPressed(id, i);
    buttonStateUpdated |= button[i] != button_old[i];
  }

  axis[0] = sf::Joystick::getAxisPosition(id, sf::Joystick::X);
  axis[1] = sf::Joystick::getAxisPosition(id, sf::Joystick::Y);
  axis[2] = sf::Joystick::getAxisPosition(id, sf::Joystick::Z);

  axis[3] = sf::Joystick::getAxisPosition(id, sf::Joystick::R);
  axis[4] = sf::Joystick::getAxisPosition(id, sf::Joystick::U);
  axis[5] = sf::Joystick::getAxisPosition(id, sf::Joystick::V);

  axis[6] = sf::Joystick::getAxisPosition(id, sf::Joystick::PovX);
  axis[7] = sf::Joystick::getAxisPosition(id, sf::Joystick::PovY);
  return 0;
}

static int init;
void move_arm() {
  system("cls");
  std::cout << "AXIS:    X,    Y,    Z,    R,    U,    V,  PovX,  PovY, \n";
  std::cout.width(4);
  std::cout << "     ";
  for (int i = 0; i < 8; i++) {
    std::cout << axis[i] << ", ";
  }

  std::cout << "\nBTN :";
  for (size_t i = 0; i < sf::Joystick::getButtonCount(id); i++) {
    std::cout << button[i];
  }
  std::cout << std::endl;

  //if (button_old[2] && !button[2]) {
  if (!init) {
    init++;
    for (int i = 0; i < 7; i++) {
      udero->setJointMode(i, MODE_POSITION);
      //udero->ppJoints[i]->setAcceleration(15.0);
    }
    for (int i = 0; i < 7; i++) {
      //udero->ppJoints[i]->setMode(MODE_POSITION);
      udero->setJointAcceleration(i, 15.0);
    }

  }
	
  if (button_old[3] && !button[3]) {
    std::cout << "Udero.init()" << std::endl;
    udero->init();
    init = 0;
  }

  /*
    if(button_old[7] && !button[7]) {
    for (int i = 0; i < 7; i++) {
    udero->ppJoints[i]->setMode(MODE_INACTIVE);
    }
    }

    if (button_old[10] && !button[10]) {
    udero->goHome();
    }
  */

  for (int i = 0; i < 7; i++) {
    joint_pos[i] = udero->getJointPosition(i);
  }

  for (int i = 0; i < 7; i++) {
    std::cout << "J[" << i + 1 << "]: " << joint_pos[i] << std::endl;
  }

  if (axis[0] < -40) {
    udero->moveJoint(0, -0.02 + joint_pos[0], 1.5);
  }
  else if (axis[0] > 40) {
    udero->moveJoint(0, 0.02 + joint_pos[0], 1.5);
  }
  else {
    //udero->ppJoints[0]->movePositionWithVelocity(joint_pos[0], 1.5);
  }

  /*
  if (axis[1] < -40) {
    udero->ppJoints[1]->movePositionWithVelocity(-0.02 + joint_pos[1], 1.5);
  }
  else if (axis[1] > 40) {
    udero->ppJoints[1]->movePositionWithVelocity(0.02 + joint_pos[1], 1.5);
  }
  else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
  }

  if (axis[3] < -40) {
    udero->ppJoints[2]->movePositionWithVelocity(-0.01 + joint_pos[2], 1.5);
  }
  else if (axis[3] > 40) {
    udero->ppJoints[2]->movePositionWithVelocity(0.01 + joint_pos[2], 1.5);
  }
  else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
  }

  if (axis[2] < -40) {
    udero->ppJoints[3]->movePositionWithVelocity(-0.01 + joint_pos[3], 1.5);
  }
  else if (axis[2] > 40) {
    udero->ppJoints[3]->movePositionWithVelocity(0.01 + joint_pos[3], 1.5);
  }
  else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
  }

  if (axis[7] < -40) {
    udero->ppJoints[4]->movePositionWithVelocity(-0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(0.01 + joint_pos[5], 1.5);
  }
  else if (axis[7] > 40) {
    udero->ppJoints[4]->movePositionWithVelocity(0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(-0.01 + joint_pos[5], 1.5);
  }
  else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
  }

  if (axis[6] > 40) {
    udero->ppJoints[4]->movePositionWithVelocity(-0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(-0.01 + joint_pos[5], 1.5);
  }
  else if (axis[6] < -40) {
    udero->ppJoints[4]->movePositionWithVelocity(0.01 + joint_pos[4], 1.5);
    udero->ppJoints[5]->movePositionWithVelocity(0.01 + joint_pos[5], 1.5);
  }
  else {
    //udero->ppJoints[1]->movePositionWithVelocity(joint_pos[1], 1.5);
  }
  */

  /*
  if (button[0]) {
    udero->ppJoints[6]->movePositionWithVelocity(-0.007 + joint_pos[6], 1.0);
  }
  if (button[1]) {
    udero->ppJoints[6]->movePositionWithVelocity(0.007 + joint_pos[6], 1.0);
  }
  */
}

inline double inc_to_pos(int32_t inc, double gearRatio, double encoderRes) {
  //return inc;
  return ((inc)* 2 * M_PI) / (gearRatio * 4 * encoderRes);
}

int main(const int argc, const char* argv[]) {
 {
		udero = createUdero(parseArgs(argc, argv));

		sf::Joystick::update();
		if (!sf::Joystick::isConnected(id)) {
			std::cout << "[RTC::SFMLJoystick] Joystick (id=" << id << ") not found." << std::endl;
			return -1;
		}

		udero->init();

		for (int i = 0; i < 7; i++) {
			joint_pos[i] = udero->getJointPosition(i);
		}


		for (int i = 0; i < 7; i++) {
			udero->setJointMode(i, MODE_POSITION);
			ssr::Thread::Sleep(100);
		}

		update_joystick();

		for (int i = 0; i < 7; i++) {
			//udero->ppJoints[i]->setMode(MODE_POSITION);
			//udero->ppJoints[i]->setAcceleration(15.0);
			Sleep(100);
		}

		while (true) {

			try {
				ssr::Thread::Sleep(100);
				if (update_joystick() < 0) return -1;

				move_arm();

				if (_kbhit()) {
					int c = _getch();

					if (c == 'w') {
						std::cout << "hogehogefoo" << std::endl;
					}
				}
			}
			catch (std::exception& ex) {
				std::cout << "Exception: " << ex.what() << std::endl;
				//getchar();
				//return -1;
			}
		}

	}
}


