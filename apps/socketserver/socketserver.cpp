#include "ServerSocket.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include "Thread.h"
#include "udero/Udero.h"


using namespace reharo;

void homing_wrist(IUdero* udero, int id);

int port = 8000;

void write_line(ssr::Socket& s, const std::string& line) {
  s.Write(line.c_str(), line.length());
  const char c = 0x0a;
  s.Write(&c, 1);
}

void show_help(ssr::Socket& s) {
  write_line(s, "command [arguments]");
  write_line(s, "* disconnect (Disconnect from Udero");
  write_line(s, "* lock (Servo ON)");
  write_line(s, "* unlock (Servo OFF)");
  write_line(s, "* foldin (Move to FoldIn pose)");
  write_line(s, "* foldout (Move to FoldOut pose)");
  write_line(s, "* waitstopped (Wait until robot is stopped. Then return OK message)");
  write_line(s, "* get_angles (Get Angles of Joints. J1, J2, J3, J4, J5, J6, Gripper will return)");
  write_line(s, "* set_velocities J1, J2, J3, J4, J5, J6, Gripper (Set move velocity [rad/sec])");
  write_line(s, "* set_angles J1, J2, J3, J4, J5, J6, Gripper (Set Target Angles of Joints)");
  write_line(s, "* get_tcp (Get TCP position in Cartesian Space. This will return Euler ZYZ orientation representation.");
  write_line(s, "* get_tcp_mat (Get TCP position in Cartesian Space. This will return Matrix representation (v00 v01 v02 v03 v10 v11 v12 v13 v20 v21 v22 v23)). The matrix's 4th row will be ignored becuase the value is always 0 0 0 1.)");
  write_line(s, "* set_step_time TIME (Set step_time to configure the cartesian move. This time is the interval to finish the motion of set_tcp_mat)");
  write_line(s, "* set_tcp_mat V00 V01 V02 V03 V10 V11 V12 V13 V20 V21 V22 V23 (Set TCP position and orientation. This function uses Matrix representation. The 4th row is ignored because the value is always 0 0 0 1)");
  write_line(s, "* move_gripper HANDGAPmeter SPD (Move Gripper)");
  write_line(s, "* close_gripper (Close gripper)");
  write_line(s, "* open_gripper (Open gripper)");
}

std::vector<std::string> split(std::string str, char del) {
  int first = 0;
  int last = str.find_first_of(del);
  
  std::vector<std::string> result;
  
  while (first < str.size()) {
    std::string subStr(str, first, last - first);
    result.push_back(subStr);
    first = last + 1;
    last = str.find_first_of(del, first);
    
    if (last == std::string::npos) {
      last = str.size();
    }
  }
  
  return result;
}

std::string read_line(ssr::Socket& s) {
  std::stringstream ss;
  while(1) { // Wait Line
    
    int len = 0;
    char c;
    do {
      ssr::Thread::Sleep(1);
      len = s.GetSizeInRxBuffer();
    } while(len == 0);
    
    for(int i = 0;i< len;i++) {
      s.Read(&c, 1);
      ss << c;
      if (c == 0x0a) {
	break;
      }
    }
    
    if (c == 0x0a) {
      break;
    }
  }
  return ss.str();
}


std::string trim(const std::string& string, const char* trimCharacterList = " \t\v\r\n") {
  std::string result;
  std::string::size_type left = string.find_first_not_of(trimCharacterList);
  if (left != std::string::npos) {
    std::string::size_type right = string.find_last_not_of(trimCharacterList);
    result = string.substr(left, right - left + 1);
  }
  return result;
}

int main(const int argc_, const char* argv_[]) {
  try {
	  ssr::SocketInitializer();
    UderoConnectionProfile prof = parseArgs(argc_, argv_);
    IUdero* udero = createUdero(prof);
    int argc = prof.unknown_args.size();
    std::vector<std::string> argv = prof.unknown_args;
    ssr::Thread::Sleep(1000);

    ssr::ServerSocket serverSocket;
    serverSocket.Bind(port);
    serverSocket.Listen(1);
    std::cout << "Listen (" << port << ")" << std::endl;
    while(1) {
      ssr::Socket s= serverSocket.Accept();
      std::cout << "Accepted" << std::endl;
      std::vector<double> velocities;
      for(int i = 0;i < 7;i++) {
	velocities.push_back(0.1);
      }
      double step_time = 1.0;
      try {
	while(1) { // Execution Loop
	  std::string cmdline = trim(read_line(s));
	  std::vector<std::string> cmds = split(cmdline, ' ');
	  if (cmds.size() == 0) { continue; }
	  std::string cmd = cmds[0];
	  
	  std::cout << "Command = " << cmd << std::endl;
	  if (cmd == "disconnect") {

	    break;
	  } else if (cmd == "lock") {
	    for(int i = 0;i < 7;i++) {
	      udero->setJointMode(i, reharo::MODE_POSITION);
	    }
	    write_line(s, "lock OK");
	  } else if (cmd == "unlock") {
	    for(int i = 0;i < 7;i++) {
	      udero->setJointMode(i, reharo::MODE_INACTIVE);
	    }
	    write_line(s, "unlock OK");
	  } else if (cmd == "foldin") {
	    udero->foldIn(true);
	    write_line(s, "foldin OK");
	  } else if (cmd == "foldout") {
	    udero->foldOut(true);
	    write_line(s, "foldout OK");
	  } else if (cmd == "waitstopped") {
	    udero->waitStopped();
	    write_line(s, "waitstopped OK");
	  } else if (cmd == "get_angles") {
	    udero->spin();
	    std::stringstream ss;
	    ss << "get_angles ";
	    for(int i = 0;i < 7;i++) {
	      ss << udero->getJointPosition(i) << ' ';
	    }
	    ss << "OK";
	    write_line(s, ss.str());
	  } else if (cmd == "set_velocities") {
	    if (cmds.size() != 7+1) {
	      write_line(s, "set_velocities NG # NUM OF ARGUMENT IS NOT 7");
	    } else {
	      for(int i = 0;i < 7;i++) {
		velocities[i] = atof(cmds[i+1].c_str());
	      }
	      write_line(s, "set_velocities OK");
	    }
	  } else if (cmd == "set_angles") {
	    std::vector<double> pos;
	    if (cmds.size() != 7+1) {
	      write_line(s, "set_angles NG # NUM OF ARGUMENT IS NOT 7");
	    } else {
	      for(int i = 0;i < 7;i++) {
		pos.push_back(atof(cmds[i+1].c_str()));
	      }
	      udero->moveJoints(pos, velocities);
	      udero->moveJoint(6, pos[6], velocities[6]);
	      write_line(s, "set_angles OK");
	    }

	  } else if (cmd == "get_tcp") {
	    udero->spin();
	    Matrix44 mat = udero->forwardKinematics();
	    Vector3 vec = MatrixToEulerZYZ(mat);
	    std::stringstream ss;
	    ss << "get_tcp ";
	    ss << mat.v[0][3] << ' ';
	    ss << mat.v[1][3] << ' ';
	    ss << mat.v[2][3] << ' ';
	    ss << vec.v[0] << ' ';
	    ss << vec.v[1] << ' ';
	    ss << vec.v[2] << ' ';
	    ss << "OK";
	    write_line(s, ss.str());
	  } else if (cmd == "get_tcp_mat") {
	    udero->spin();
	    Matrix44 mat = udero->forwardKinematics();
	    std::stringstream ss;
	    ss << "get_tcp_mat ";
	    for(int i = 0;i< 3;i++) {
	      for(int j = 0;j < 4;j++) {
		ss << mat.v[i][j] << ' ';
	      }
	    }
	    ss << "OK";
	    write_line(s, ss.str());
	  } else if (cmd == "set_tcp_mat") {
	    if (cmds.size() != 1+12) {
	      write_line(s, "set_tcp_mat NG # NUM OF ARGUMENT IS NOT 3x4");
	    } else {
	      Matrix44 mat = udero->forwardKinematics();;
	      mat.v[0][3] = atof(cmds[1+0].c_str());
	      mat.v[1][3] = atof(cmds[1+1].c_str());
	      mat.v[2][3] = atof(cmds[1+2].c_str());
	      udero->moveCartesian(mat, step_time);
	      write_line(s, "set_tcp_pos OK");
	    }
	  } else if (cmd == "set_step_time") {
	    if (cmds.size() != 1+1) {
	      write_line(s, "set_step_time NG # NUM OF ARGUMENT IS NOT 1");
	    } else {
	      step_time = atof(cmds[1].c_str());
	      write_line(s, "set_step_time OK");
	    }
	  } else if (cmd == "get_step_time") {
	    std::stringstream ss;
	    ss << "get_step_time " << step_time << " OK";
	    write_line(s, ss.str());
	  } else if (cmd == "open_gripper") {
	    udero->moveHand(0, 10);
	    write_line(s, "open_gripper OK");
	  } else if (cmd == "close_gripper") {
	    udero->moveHand(-80, 10);
	    write_line(s, "close_gripper OK");
	  } else if (cmd == "move_gripper") {
	    double gap = atof(cmds[1+0].c_str()) * 1000 - 80;
	    double spd = atof(cmds[1+1].c_str()) * 1000;
	    udero->moveHand(gap, spd);
	    write_line(s, "move_gripper OK");
	  } else if (cmd == "help") {
	    show_help(s);
	    write_line(s, "help OK");
	  } else {
	    write_line(s, "UNKNOWN COMMAND");
	  }
	}
	s.Close();
      } catch (ssr::SocketException& ex) {
	std::cout << "SocketException:" << ex.what() << std::endl;
      }
    }


    deleteUdero(udero);
  } catch (std::exception &ex) {
    std::cout << "Exception: " << ex.what() << std::endl;
    return -1;
  }
  return 0;
}
