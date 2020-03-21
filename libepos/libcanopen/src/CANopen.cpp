/**
 * @file CANopen.cpp
 */

#include "Thread.h"
#include "Timer.h"
#include "CANopen.h"
#include <iostream>

using namespace canopen;
using namespace ssr;

CANopen::CANopen(ICANInterface *pCAN) {
	m_pCAN = pCAN;

	memset(&syncMessage, 0, sizeof(syncMessage));
	syncMessage.id = 0x80;

	for(int i = 0;i < MAX_NODE_FACTORY;i++) {
		m_ppNodeFactory[i] = NULL;
	}

	for(int i = 0;i < MAX_NODE;i++) {
		m_ppNode[i] = NULL;
	}

	Thread::Start();
}

CANopen::~CANopen()
{
	m_Endflag = true;
	Thread::Join();
}


INodeFactory* CANopen::searchNodeFactory(const uint32_t vendorId, const uint32_t productId) {
	for(int i = 0;i < MAX_NODE_FACTORY;i++) {
		INodeFactory *nodeFactory = m_ppNodeFactory[i];
		if(nodeFactory != NULL) {
			uint32_t vid = nodeFactory->getVendorId();
			uint32_t pcode = nodeFactory->getProductCode();
			uint32_t pmask = nodeFactory->getProductCodeMask();

			if(vendorId == vid &&
				pcode == (productId & pmask)) {
					return nodeFactory;
			}
		}
	}
	return NULL;
}


Node* CANopen::initNode(const uint8_t nodeId)
{
	if(m_ppNode[nodeId] == NULL) {
		Node* pNode = new Node(this, nodeId);

		registerNode(pNode);
		pNode->initialize();

		//TimeSpec timeOut; timeOut.sec = 0; timeOut.usec = 100*1000;
		TimeSpec timeOut(0, 100*1000);
		uint32_t vendorId = m_ppNode[nodeId]->readVendorId(timeOut);
		uint32_t productId = m_ppNode[nodeId]->readProductCode(timeOut);

		unregisterNode(nodeId);

		INodeFactory* nodeFactory = searchNodeFactory(vendorId, productId);
		/*Node* */pNode = nodeFactory->createNode(this, nodeId);
		registerNode(pNode);
	}
	return m_ppNode[nodeId];
}

Node* CANopen::getNode(const uint8_t nodeId) throw()
{
	if(nodeId >= MAX_NODE) return NULL;
	MutexBinder binding(m_NodeListMutex);
	return m_ppNode[nodeId];
}

void CANopen::registerNode(Node* pNode) {
	uint8_t nodeId = pNode->getNodeId();
	MutexBinder binding(m_NodeListMutex);
	m_ppNode[nodeId] = pNode;
}

Node* CANopen::unregisterNode(const uint8_t nodeId) {
	Node* pNode = m_ppNode[nodeId];

	MutexBinder binding(m_NodeListMutex);
	m_ppNode[nodeId] = NULL;
	return pNode;
}


bool CANopen::dispatchCANMessageSub(const CANMSG& msg, 
									const uint32_t id_offset,
									void(Node::*pFunc)(const CANMSG&))
{
	if(msg.id >= id_offset) { // PDO1 message
		uint8_t nodeId = msg.id - id_offset;
		
		Node* pNode = getNode(nodeId);
		if(pNode != NULL) {
			// std::cout << "CANMSG:" << (int)nodeId << "/" << (int)msg.id << ":" << (int)msg.data[0] << " " << (int)msg.data[1] << "\n";
			(pNode->*pFunc)(msg);
			return true;
		}
	}
	return false;
}

void CANopen::dispatchCANMessage(const CANMSG& msg)
{
	if (dispatchCANMessageSub(msg, COBID_HEARTBEAT_OFFSET, &Node::pushHeartBeatMessage)) { return; }
	if (dispatchCANMessageSub(msg, COBID_RXSDO_OFFSET, &Node::pushSdoMessage)) { return; }
	if (dispatchCANMessageSub(msg, COBID_TXPDO4_OFFSET, &Node::pushPDOMessage<PDO4>)) { return; }
	if (dispatchCANMessageSub(msg, COBID_TXPDO3_OFFSET, &Node::pushPDOMessage<PDO3>)) { return; }
	if (dispatchCANMessageSub(msg, COBID_TXPDO2_OFFSET, &Node::pushPDOMessage<PDO2>)) { return; }
	if(dispatchCANMessageSub(msg, COBID_TXPDO1_OFFSET, &Node::pushPDOMessage<PDO1>)) { return; }
	for(uint8_t i = 0;i < MAX_NODE;i++) {
		Node* pNode = getNode(i);
		if(pNode != NULL && msg.id == pNode->getEmergencyId()) {
			pNode->pushEmergencyMessage(msg);
		}
	}
}

void CANopen::Run() 
{
	
	m_Status = true;
	m_Endflag = false;
	while(!m_Endflag) {
		CANMSG rx;
		try {
			if(m_pCAN->receive(&rx)) { // Not Empty
				dispatchCANMessage(rx);
			}

			for(uint8_t i = 0;i < MAX_NODE;i++) {
				Node* pNode = getNode(i);
				if(pNode != NULL) {
					pNode->checkHeartBeatTimeOut();
				}

			}
			
			//Thread::Sleep(1);
		} catch (CANException & ex) {
			std::cout << "[CANOpen::BackgroundJob] Error. " << ex.what() << std::endl;
			//m_Status = false;
		}
	}
}



void CANopen::send(const CANMSG& msg) {
	ssr::MutexBinder binder(m_Mutex);
	m_pCAN->send(msg);
}

void CANopen::receive(CANMSG* msg) {
	ssr::MutexBinder binder(m_Mutex);
	m_pCAN->receive(msg);
}


void CANopen::sendPDO(const uint8_t nodeId, const PDO_INDEX pdoIndex, const uint8_t length, const uint8_t data[])
{
	CANMSG msg;
	msg.id = 0x200 + 0x100 * pdoIndex + nodeId;
	msg.rtr = 0;
	msg.length = length;
	for(int i = 0;i < length;i++) {
		msg.data[i] = data[i];
	}
	m_pCAN->send(msg);
}
