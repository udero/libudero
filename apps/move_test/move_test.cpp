#include <stdlib.h>

#include <iostream>
#include "Thread.h"
#include "udero/Udero.h"
#include "udero/UderoLogger.h"

using namespace technotools;


int main(const int argc, const char* argv[]) {
  try {
      technotools::initLogger(argc, argv);
    UderoConnectionProfile prof = parseArgs(argc, argv);
    IUdero* udero = createUdero(prof);
    std::cout << "Starting Move Test" << std::endl;
    for (int i = 0; i < 7; i++) {
      udero->setJointMode(i, technotools::MODE_POSITION);
      ssr::Thread::Sleep(100);
    }
    std::cout << "Initialize OK." << std::endl;
    {
      std::cout << "Phase 1" << std::endl;
        udero->foldOut(true);
        std::vector<double> pose1;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
	  pose1.push_back(pos);

        }
	pose1[0] = 0;
        pose1[1] = M_PI / 2;
        pose1[2] = 0;
        pose1[3] = 0;
        pose1[4] = 0;
        pose1[5] = 0;
	double vel = 0.1;
	double pose1_vel_raw[] = { vel, vel, vel, vel, vel, vel };
        std::vector<double> pose1_vel;
	for(int i = 0;i < 6;i++) {
	  pose1_vel.push_back(pose1_vel_raw[i]);
	}
        udero->moveJoints(pose1, pose1_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
    }

    {
      std::cout << "Phase 2" << std::endl;
        udero->foldOut(true);
	
        std::vector<double> pose2;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
	  pose2.push_back(pos);
        }
        pose2[0] = 140.0 / 360.0 * 2 * M_PI;
	double vel = 0.2;
	double pose2_vel_raw[] = { vel, vel, vel, vel, vel, vel };
        std::vector<double> pose2_vel;
	for(int i = 0;i < 6;i++) {
	  pose2_vel.push_back(pose2_vel_raw[i]);
	}
        udero->moveJoints(pose2, pose2_vel);
        udero->waitStopped();

	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
	std::cout << "folding in...." << std::endl;
        udero->foldIn();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
    }

    {
      std::cout << "Phase 3" << std::endl;
        udero->foldOut(true);

        std::vector<double> pose3;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
	  pose3.push_back(pos);
        }
        pose3[1] = 0;
        pose3[2] = 0;
        pose3[3] = 0;
        pose3[4] = 0;
        pose3[5] = 0;
	double vel = 0.2;
	double pose3_vel_raw[] = { vel, vel, vel, vel, vel, vel };
        std::vector<double> pose3_vel;
	for(int i = 0;i < 6;i++) {
	  pose3_vel.push_back(pose3_vel_raw[i]);
	}
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
	std::cout << "---------------------------" << std::endl;

        pose3[3] = 170.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
	std::cout << "---------------------------" << std::endl;

	std::cout << "moving joint[4]" << std::endl;
        pose3[4] = 90.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
        pose3[4] = -90.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
        pose3[4] = 0.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
	std::cout << "---------------------------" << std::endl;

	std::cout << "moving joint[5]" << std::endl;
        pose3[5] = 90.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
        pose3[5] = -90.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
        pose3[5] = 0.0 / 180.0 * M_PI;
        udero->moveJoints(pose3, pose3_vel);
        udero->waitStopped();
	std::cout << "---------------------------" << std::endl;
        for (int i = 0; i < 6; i++) {
	  double pos = udero->getJointPosition(i);
	  std::cout << "J[" << i << "] = " << pos << std::endl;
        }
	std::cout << "---------------------------" << std::endl;


        udero->moveHand(0, 0.5);
        udero->waitStopped();
        udero->moveHand(1, 0.5);
        udero->waitStopped();
    }
    
    udero->foldIn();

    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    UERROR("Exception:%s", ex.what());
    return -1;
  }
  return 0;
}

