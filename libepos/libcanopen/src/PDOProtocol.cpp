/**
 * @file PDOProtocol.cpp
 * @copyright RT Corp. 2012. Allrights Reserved.
 */


#include <iostream>
#include "Timer.h"
// #include "Mutex.h"
#include "Thread.h"
#include "CANMSGBuffer.h"
#include "CANopen_defines.h"
#include "CANopen.h"
#include "PDOProtocol.h"

using namespace canopen;



PDOProtocol::PDOProtocol(CANopen* pCANopen, const uint16_t nodeId) : m_pCANopen(pCANopen), m_NodeId(nodeId)
{

}

PDOProtocol::~PDOProtocol()
{

}


void PDOProtocol::checkNodeErrorState(void)
{
  m_pNode->checkNodeErrorState();
}

void PDOProtocol::send(const CANMSG& msg)
{
  m_pCANopen->send(msg);
}

void PDOProtocol::sendNMT(const NMT_MESSAGE status)
{
  try {
	m_pNode->checkNodeErrorState();

	CANMSG NMT;
	NMT.id = 0x00;
	NMT.length = 2;
	NMT.rtr = 0;
	NMT.data[0] = status;
	NMT.data[1] = (uint8_t)m_NodeId;
	m_pCANopen->send(NMT);
  } catch (CANException& ex) {
    std::cerr << "Exception in PDOProtocol::sendNMT(" << status << ") : "
	      << ex.what() << std::endl;
    perror("PDOProtocol::sendNMT()");
    throw ex;
    
  }
}

void PDOProtocol::sendSync(){
	m_pCANopen->sendSync();
}

void PDOProtocol::sendPDO(const PDO_INDEX pdoIndex, const uint8_t length, const uint8_t data[])
{
	m_pCANopen->sendPDO(m_NodeId, pdoIndex, length, data);
}
