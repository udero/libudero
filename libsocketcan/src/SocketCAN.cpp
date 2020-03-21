/**
 * @file SocketCAN.cpp
 */

#include "SocketCAN.h"


#ifdef WIN32
#include <windows.h>

#include "PCANBasic.h"
	//#include "PCANBasic.h"

#else // For Linux

#include <fcntl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include "net/if.h"

#include <unistd.h>
#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <sstream>
#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif


#endif

using namespace canopen;
using namespace ssr;

SocketCAN::SocketCAN(const char* fileName /* = NULL */)
{
  m_FileName = fileName;
  m_InitFlag = false;
}

SocketCAN::~SocketCAN(void)
{
}


void SocketCAN::initialize(const ICAN_BAUD baudrate) throw(CANException)
{
  struct ifreq ifr; 
  std::stringstream ss;
  if((m_Fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    ss << "SocketCAN::initialize(socket()): " << strerror(errno);
    perror("Can not open CAN socket.");
    throw CANException(ss.str());
  }
  
  strcpy(ifr.ifr_name, m_FileName.c_str());
  ioctl(m_Fd, SIOCGIFINDEX, &ifr);

  //int loopback = 1;
  //setsockopt(sfd, SOL_CAN_RAW, CAN_RAW_LOOPBACK, &loopback,sizeof(loopback));
  
  can_addr.can_family = AF_CAN;
  can_addr.can_ifindex = ifr.ifr_ifindex;
 
  if(bind(m_Fd, (struct sockaddr *)&can_addr, sizeof(can_addr)) < 0) {
    //     std::cout << "bind error. please check can interface." << std::endl;
     close(m_Fd);
     ss << "SocketCAN::initialize(bind()): " << strerror(errno);
     perror("Error in socket bind");
     throw CANException(ss.str());
  }

  m_InitFlag = true;
}

void SocketCAN::uninitialize()
{
  close(m_Fd);
}

bool SocketCAN::receive(CANMSG* msg)
{
  std::stringstream ss;
  if(!m_InitFlag) {
    throw CANException("SocketCAN is not initialized. Call initialize().");
  }
  struct can_frame rxframe;
   
  if(read(m_Fd, &rxframe, sizeof(rxframe)) < 0) {
    ss << "SocketCAN::receive: " << strerror(errno);
    perror("Can not receive CAN socket.");
    throw CANException(ss.str());
  }
 
  msg->id = rxframe.can_id;
  msg->length = rxframe.can_dlc;
  memcpy(msg->data, rxframe.data, msg->length);
  return true;
}

void SocketCAN::send(const CANMSG& msg)
{
  std::stringstream ss;
  if(!m_InitFlag) {
    throw CANException("SocketCAN is not initialized. Call initialize().");
  }
  struct can_frame txframe;
  //  int res;
   
  txframe.can_id = msg.id; 
  txframe.can_dlc = msg.length;
  memcpy(txframe.data, msg.data, msg.length);
  
  if(::send(m_Fd, &txframe, sizeof(txframe), 0) < 0) {
    ss << "SocketCAN::send: " << strerror(errno);
    perror("Can not send CAN socket.");
    throw CANException(ss.str());
  }
}
