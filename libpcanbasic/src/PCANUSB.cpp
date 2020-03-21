/**
 * @file PCANUSB.cpp
 */

#include "PCANUSB.h"


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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef PF_CAN
#define PF_CAN 29
#endif

#ifndef AF_CAN
#define AF_CAN PF_CAN
#endif


#endif

using namespace canopen;
using namespace peaksystem;

#ifndef WIN32

PCANUSB::PCANUSB(const char* fileName /* = NULL */)
{
#ifdef WIN32

#else
  m_FileName = fileName;
#endif
  m_InitFlag = false;
}
#endif

PCANUSB::PCANUSB(const int32_t deviceID) : canopen::ICANInterface()
{
#ifdef WIN32
	m_deviceID = deviceID;
#else
	m_FileName = fileName;
#endif
	m_InitFlag = false;
}

PCANUSB::~PCANUSB(void)
{
#ifdef WIN32


#else


#endif
}


void PCANUSB::initialize(const ICAN_BAUD baudrate) throw(CANException)
{
#ifdef WIN32
	uint32_t baud;
	switch(baudrate) {
		case ICAN_BAUD_1M:
			baud = PCAN_BAUD_1M;
			break;
		case ICAN_BAUD_500K:
			baud = PCAN_BAUD_500K;
			break;
		default:
			break;
	}

	unsigned int ret;
	
	if ((ret = CAN_Initialize(m_deviceID, baud)) != PCAN_ERROR_OK) {
		switch(ret) {
#define __CLASSNAME__ "peaksystem::PCANUSB"
			case PCAN_ERROR_ILLHANDLE:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_ILLHANDLE");
				break;

			case PCAN_ERROR_ILLOPERATION:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_ILLOPERATION");
				break;
			case PCAN_ERROR_INITIALIZE:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_INITIALIZE");
				break; 
			case PCAN_ERROR_NETINUSE:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_NETINUSE");
				break;
			case PCAN_ERROR_HWINUSE:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_HWINUSE");
				break;
			case PCAN_ERROR_CAUTION:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_CAUTION");
				break;
			case PCAN_ERROR_NODRIVER:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_NODRIVER");
				break;
			case PCAN_ERROR_REGTEST:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_REGTEST");
				break; 
			case PCAN_ERROR_ILLHW:
				throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_ILLHW");
				break;
			default:
				break;
		}
		throw CANException(__CLASSNAME__ "::initialize() : unknown");
	}
	if ((ret = CAN_Reset(m_deviceID)) != PCAN_ERROR_OK) {
		switch (ret) {
#define __CLASSNAME__ "peaksystem::PCANUSB"
		case PCAN_ERROR_ILLHANDLE:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_ILLHANDLE");
			break;

		case PCAN_ERROR_ILLOPERATION:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_ILLOPERATION");
			break;
		case PCAN_ERROR_INITIALIZE:
			//throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_INITIALIZE");
			break;
		case PCAN_ERROR_NETINUSE:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_NETINUSE");
			break;
		case PCAN_ERROR_HWINUSE:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_HWINUSE");
			break;
		case PCAN_ERROR_CAUTION:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_CAUTION");
			break;
		case PCAN_ERROR_NODRIVER:
			throw CANException(__CLASSNAME__ "::initialize() : PCAN_ERROR_NODRIVER");
			break;

		default:
			throw CANException(__CLASSNAME__ "::initialize() : unknown");
			break;
		}
	}

#else // for Linux

  struct ifreq ifr; 
  if((m_Fd = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
    throw CANException("initialize");
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
     throw CANException(__FILE__ "::initialize()");
  }
#endif

  m_InitFlag = true;
}

void PCANUSB::uninitialize()
{
#ifdef WIN32
	CAN_Uninitialize(m_deviceID);
#else
	close(m_Fd);
#endif
}

bool PCANUSB::receive(CANMSG* msg)
{
	if(!m_InitFlag) {
		throw CANException("PCANUSB is not initialized. Call initialize().");
	}

#ifdef WIN32
	TPCANMsg msgBuf;
	TPCANTimestamp timestampBuffer;
	unsigned int ret;
	
	if((ret = CAN_Read(m_deviceID, &msgBuf, &timestampBuffer)) != PCAN_ERROR_OK) {
		if(ret == PCAN_ERROR_QRCVEMPTY) return false;

		std::ostringstream oss;
		oss << "peaksystem::PCANSUB::receive() Error : " << "code = " << ret;
		throw CANException(oss.str());
	}
	
	msg->id = msgBuf.ID;
	//msg->type = msgBuf.MSGTYPE;
	msg->length = msgBuf.LEN;
	memcpy(msg->data, msgBuf.DATA, msgBuf.LEN);
	return true;
#else


  struct can_frame rxframe;
   
  if(read(m_Fd, &rxframe, sizeof(rxframe)) < 0) {
    //std::cout << "Read Message error." << std::endl;
    throw CANException("receive");
  }
 
  msg->id = rxframe.can_id;
  msg->length = rxframe.can_dlc;
  memcpy(msg->data, rxframe.data, msg->length);
  return true;
#endif
}

void PCANUSB::send(const CANMSG& msg)
{
	if(!m_InitFlag) {
		throw CANException("PCANUSB is not initialized. Call initialize().");
	}

#ifdef WIN32
	TPCANMsg msgBuf;
	msgBuf.ID = msg.id;
	msgBuf.MSGTYPE = msg.rtr ? PCAN_MESSAGE_RTR : 0;
	msgBuf.LEN = msg.length;
	memcpy(msgBuf.DATA, msg.data, msg.length);
	
	unsigned int ret;
	if((ret=CAN_Write(m_deviceID, &msgBuf)) != PCAN_ERROR_OK) {
		throw CANException("send");
	}
#else
  struct can_frame txframe;
  //  int res;
   
  txframe.can_id = msg.id; 
  txframe.can_dlc = msg.length;
  memcpy(txframe.data, msg.data, msg.length);
  
  if(::send(m_Fd, &txframe, sizeof(txframe), 0) < 0){
    //    std::cout << "send message error." << std::endl;
    //    return false;
    throw CANException("send");
  }

#endif
}
