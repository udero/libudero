/**
 * @file Node.cpp
 */


#include <sstream>
#include <iostream>
#include "CANopen.h"
#include "Node.h"

#include "Thread.h"
using namespace canopen;
using namespace ssr;

Node::Node(CANopen* pCANopen, const uint8_t nodeId) :
m_NodeId(nodeId), 
m_pCANopen(pCANopen), 
m_SDO(pCANopen, nodeId), 
m_PDO(pCANopen, nodeId), 
m_NMTState(NMT_PREOPERATIONAL), 
m_NodeErrorState(0)
{
	m_SDO.setParentNode(this);
	m_PDO.setParentNode(this);

	m_HeartBeatTimer.tick();
	m_HeartBeatTimeOut = INFINITETIME;

}

Node::~Node(void)
{
}

void Node::initialize(void)
{
  try {
	ssr::TimeSpec timeOut(0, 500*1000); // 500 msec
	for (int i = 0; i < 15; i++) {
		////this->reset();
		try {
			enterPreOperational();
			start();
			m_EmergencyId = readCOBIDEmergency(timeOut); // Throw if TimeOUT
			m_ProducerHeartBeatTime = readProducerHeartBeatTime(timeOut);
			if (m_ProducerHeartBeatTime == 0) {
				m_HeartBeatTimeOut = INFINITETIME;
			}
			else {
				m_HeartBeatTimeOut = TimeSpec((m_ProducerHeartBeatTime + 5) * 1000);
			}
			return;
		}
		catch (TimeOutException& tex) {
		  std::cerr << "TimeoutException in Node::initialize()" << std::endl;
		  std::cerr << " trying to reset node...." << std::endl;
			reset();
			ssr::Thread::Sleep(500);
		}
	}
	std::stringstream ss;
	ss << "Node::initialize Timeout (id=" << m_NodeId << ")" << std::endl;
	throw TimeOutException("Node::initialize Timeout.");
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::initialize() : " << ex.what() << std::endl;
    throw ex;
  }
}


void Node::checkNodeErrorState(void)
{
	if(m_NodeErrorState & ERROR_HEARTBEATTIMEOUT) {
		throw HeartBeatTimeOutException();
	}

}

void Node::checkHeartBeatTimeOut() throw()
{
	//if(m_NMTState == NMT_OPERATIONAL) {
	if(m_ProducerHeartBeatTime > 0) { // Heart Beat Protocol = ON
		TimeSpec time;
		m_HeartBeatTimer.tack(&time);
		if(time > m_HeartBeatTimeOut) {
			setNodeErrorState(ERROR_HEARTBEATTIMEOUT);
		}
	}

	//}
}

void Node::start(void) {
  try {
    m_PDO.sendNMT(START_REMOTE_NODE);
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::start() : " << ex.what() << std::endl;
    throw ex;
  }
}

void Node::stop(void) {
  try {
    m_PDO.sendNMT(STOP_REMOTE_NODE);
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::stop() : " << ex.what() << std::endl;
    throw ex;
  }
}

void Node::reset(void) {
  try {
    m_PDO.sendNMT(RESET_NODE);
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::reset() : " << ex.what() << std::endl;
    throw ex;
  }
}

void Node::enterPreOperational(void) {
  try {
    m_PDO.sendNMT(ENTER_PREOPERATIONAL);
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::enterPreOperational() : " << ex.what() << std::endl;
    throw ex;
  }
}

void Node::resetCommunication(void) {
  try {
	m_PDO.sendNMT(RESET_COMMUNICATION);
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::resetCommunication() : " << ex.what() << std::endl;
    throw ex;
  }
}

void Node::sync(void)
{
  try {
	m_PDO.sendSync();
  } catch (std::exception& ex) {
    std::cerr << "Exception in Node::sync() : " << ex.what() << std::endl;
    throw ex;
  }
}
