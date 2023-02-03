

#pragma once
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iomanip>

#include "Thread.h"
#include "Timer.h"
#include "SerialPort.h"

namespace reharo {

  class ReadTimeoutException : public std::exception {

  };


  class ABSEncoder {
private:
  ssr::SerialPort* pSerialPort;
public:
  ABSEncoder(const std::string& filename, const int32_t baudrate) {
    pSerialPort = new ssr::SerialPort(filename.c_str(), baudrate);
  }

  ~ABSEncoder() {
    delete pSerialPort;
  }

  void writeStr(const std::string& s) {
    for(int i = 0;i < s.length();i++) {
      pSerialPort->Write(s.c_str()+i, 1);
      ssr::Thread::Sleep(1);
    }
  }


  bool echo(const int id) {
    std::stringstream ss;
    ss << 'x' << std::hex << std::setw(1) << (int)id << "es";
    writeStr(ss.str());
    ssr::Thread::Sleep(30);
    std::string c = read(id);
    if (id != (c.c_str()[1] - '0')) { // Invalid ID
      return false;
    }
    if (c.c_str()[2] == 'E') {
      return true;
    }
    return false;
  }

  bool writeID(const int id, const int newID) {
    std::stringstream ss;
    ss << 'x' << std::hex << std::setw(1) << (int)id << 'i'
       << std::hex << std::setw(1) << newID << 's';
    writeStr(ss.str());
    ssr::Thread::Sleep(30);
    std::string c = read(id);
    if (newID != (c.c_str()[1] - '0')) { // Invalid ID
      return false;
    }
    if (c.c_str()[2] == 'I') {
      return true;
    }
    return false;
  }

  bool setZero(const int id) {
    std::stringstream ss;
    ss << 'x' << std::hex << std::setw(1) << (int)id << "os";
    writeStr(ss.str());
    ssr::Thread::Sleep(30);
    std::string c = read(id);
    if (id != (c.c_str()[1] - '0')) { // Invalid ID
      return false;
    }
    if (c.c_str()[2] == 'O') {
      std::string d = c.substr(3, 6);
      int a = atoi(d.c_str());
      //std::cout << "offset = " << d << std::endl;
      return true;
    }
    return false;
  }

  bool readAngle(const int id, int* angle) {
    std::stringstream ss;
    ss << 'x' << std::hex << std::setw(1) << (int)id << "as";
    writeStr(ss.str());
    ssr::Thread::Sleep(30);
    std::string c = read(id);
    if (id != (c.c_str()[1] - '0')) { // Invalid ID
      return false;
    }
    if (c.c_str()[2] == 'A') {
      std::string d = c.substr(3, 6);
      *angle = atoi(d.c_str());
      //std::cout << "offset = " << d << std::endl;
      return true;
    }
    return false;
    
  }

  std::string read(const uint8_t id) {
    ssr::Timer timer;
    timer.tick();

    while(true) {
      ssr::TimeSpec ts;
      if (pSerialPort->GetSizeInRxBuffer() > 0) {
	char c;
	pSerialPort->Read(&c, 1);
	//	std::cout << "c:" << c;
	if (c == 'X') break;
      }
      timer.tack(&ts);
      if (ts.getUsec() > 1000000) {
	throw new ReadTimeoutException();
      }
    }

    std::stringstream ss;
    ss << 'X';
    while(true) {
      ssr::TimeSpec ts;
      if (pSerialPort->GetSizeInRxBuffer()) {
	char c;
	pSerialPort->Read(&c, 1);
	ss << c;
	if (c == 'S') break;
      }
      timer.tack(&ts);
      if (ts.getUsec() > 1000000) {
	throw new ReadTimeoutException();
      }
    }
    return ss.str();
  }
};



  inline bool abs_check(const std::string& filename, const int id, const int32_t baudrate = 9600) {
	try {
		ABSEncoder a(filename, baudrate);
		return a.echo(id);
	}
	catch (std::exception& e) {
		return false;
	}
  }


  inline bool abs_read(const std::string& filename, const int id, int32_t *angle, const int32_t baudrate=9600) {
    try {
      ABSEncoder a(filename, baudrate);
      return a.readAngle(id, angle);
    } catch (std::exception& e) {
      return false;
    }
  }


  inline bool abs_set_id(const std::string& filename, const int id, const int new_id, const int32_t baudrate = 9600) {
	  try {
		  ABSEncoder a(filename, baudrate);
		  return a.writeID(id, new_id);
	  }
	  catch (std::exception& e) {
		  return false;
	  }
  }

  inline bool abs_set_zero(const std::string& filename, const int id, const int32_t baudrate = 9600) {
	  try {
		  ABSEncoder a(filename, baudrate);
		  return a.setZero(id);
	  }
	  catch (std::exception& e) {
		  return false;
	  }
  }


}

