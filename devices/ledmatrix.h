#pragma once

#include <sstream>
#include <iomanip>
#include <stdlib.h>

#include "mark.h"
#include "Thread.h"
#include "SerialPort.h"

namespace reharo {

// #ifndef WIN32
inline uint8_t bitflip8(uint32_t a)
{
  //  uint32_t a = (0x000000ff & (uint32_t)a_);
  uint32_t b = a;
  a = a & 0x55555555;
  b = b ^ a;
  a = a << 2;
  a = a | b;   //aba0

  b = a;
  a = a & (0x33333333<<1);
  b = b ^ a;
  a = a <<4;
  a = a | b;

  b = a;
  a = a & (0x0f0f0f0f<<3);
  b = b ^ a;
  a = a <<8;
  a = a | b;

  a = a >> 7;
  return 0x00ff & a;
}

class LEDMatrixController {
private:
  ssr::SerialPort* pSerialPort;
public:
  LEDMatrixController(const std::string& filename, const int32_t baudrate) {
    pSerialPort = new ssr::SerialPort(filename.c_str(), baudrate);
  }

  ~LEDMatrixController() {
    delete pSerialPort;
  }

  void full() {
    write(FULL);
  }

  void buzz(const int hz) {
    pSerialPort->Write("xb", 2);
    ssr::Thread::Sleep(5);
    std::stringstream ss;
    ss << std::setw(5) << std::setfill('0') << hz;
    pSerialPort->Write(ss.str().c_str(), 5);
    ssr::Thread::Sleep(5);
    pSerialPort->Write("s", 1);
  }

  void color(const char c) {
    pSerialPort->Write("xl", 2);
    ssr::Thread::Sleep(5);
    pSerialPort->Write(&c, 1);
    ssr::Thread::Sleep(5);
    pSerialPort->Write("s", 1);
  }


  void write(const uint8_t* str) {
    pSerialPort->Write("xm", 2);
    ssr::Thread::Sleep(5);
    for(int j = 0;j < 8;j++) {
      int i = 7-j;
      std::stringstream ss;
      //      ss << std::hex << std::setw(2) << std::setfill('0') << (int)bitflip8(*(str+i));
      ss << std::hex << std::setw(2) << std::setfill('0') 
	 << (int)(*(str+i));
      pSerialPort->Write(ss.str().c_str(), 1);
      ssr::Thread::Sleep(5);
      pSerialPort->Write(ss.str().c_str()+1, 1);
      //std::cout << ss.str () << " ";
      ssr::Thread::Sleep(5);
    }
    pSerialPort->Write("s", 1);
    //    std::cout << std::endl;
  }

  void off() {
    write(OFF);
  }

  
};

inline void ledmatrix_write(const std::string& filename, const uint8_t* ch, const int32_t baudrate=9600) {
   try {
   LEDMatrixController c(filename, baudrate);
   c.write(ch);
   } catch (std::exception& ex) {

   }
 }

 inline void ledmatrix_buzz(const std::string& filename, const int hz, const int32_t baudrate=9600) {
   try {
   LEDMatrixController c(filename, baudrate);
   c.buzz(hz);
   } catch (std::exception& ex) {

   }
 }

 inline void ledmatrix_color(const std::string& filename, const char ch, const int32_t baudrate=9600) {
   try {
   LEDMatrixController c(filename, baudrate);
   c.color(ch);
   } catch (std::exception& ex) {

   }


 }

// #endif
}
