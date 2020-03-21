/**
 * @file SDOProtocol.cpp
 * @copyright RT Corp. 2012. Allrights Reserved.
 */

#include "Timer.h"
// #include "Mutex.h"
#include "Thread.h"
#include "CANMSGBuffer.h"
#include "CANopen_defines.h"
#include "CANopen.h"
#include "SDOProtocol.h"

using namespace canopen;


SDOProtocol::SDOProtocol(CANopen* pCANopen, const uint16_t nodeId) : m_pCANopen(pCANopen), m_NodeId(nodeId), m_pNode(0)
{

}

SDOProtocol::~SDOProtocol()
{

}

void SDOProtocol::sendDownloadMessage(const uint16_t index, 
						  const uint8_t subIndex, 
						  const uint8_t* data, 
						  const uint8_t dataByte){
	  CANMSG sdo;
	  sdo.id = COBID_TXSDO_OFFSET + m_NodeId;
	  sdo.rtr = 0;
	  sdo.length = 4 + dataByte;
	  sdo.data[0] = SDO_FUNC_DOWNLOAD | SDO_SIZE(dataByte) | SDO_EXPEDITED_TRANSFER | SDO_SIZE_INDICATED;
	  sdo.data[1] = index & 0xFF; // LSB
	  sdo.data[2] = (index >> 8) & 0xFF; //MSB
	  sdo.data[3] = subIndex;

	  for(int i = 0;i < dataByte;i++) {
		  sdo.data[4 + i] = data[i];
	  }
	  m_pCANopen->send(sdo);
}


void SDOProtocol::sendUploadMessage(const uint16_t index, const uint8_t subIndex){
	CANMSG sdo;

	sdo.id = COBID_TXSDO_OFFSET + m_NodeId;
	sdo.length = 4;
	sdo.rtr = 0;

	sdo.data[0] = SDO_FUNC_UPLOAD;
	sdo.data[1] = index & 0xFF; //LSB
	sdo.data[2] = (index >> 8) & 0xFF; //MSB
	sdo.data[3] = subIndex;
	m_pCANopen->send(sdo);
}

void SDOProtocol::waitMessage(const uint16_t index, const uint8_t subIndex, CANMSG* pMessage, const ssr::TimeSpec& timeOut) throw(TimeOutException)
{
	ssr::TimeSpec currentTime;
	m_Timer.tick();
	while(1) {
		CANMSG msg;
		if(!popMessage(&msg, timeOut)) { // Buffer Empty
			m_Timer.tack(&currentTime); // Measure waiting time...
			if(currentTime > timeOut) {
				std::stringstream ss;
				ss << "SDOProtocol::waitMessage(nodeId=" << m_NodeId << ")";
				throw TimeOutException(ss.str().c_str());
			}
			continue;
		}
		
		if( (msg.data[1] != (uint8_t)((index     ) & 0x00FF)) ||
			(msg.data[2] != (uint8_t)((index >> 8) & 0x00FF)) ||
			(msg.data[3] != subIndex)
		) {
			// If SDO message does not contain target index/subindex data...
			// It's trush!! throw away!!!
			continue;
		}

		*pMessage = msg;

		if (msg.data[0] & SDO_FUNC_ABORTED) {
			uint32_t errorCode = 0;
			for(int i = 0;i < 4;i++) {
				errorCode |= msg.data[4+i] << (i*8);
			}

			switch(errorCode) {
			case SDO_ABORT_VALUERANGE:
			case SDO_ABORT_VALUETOOHIGH:
			case SDO_ABORT_VALUETOOLOW:
				throw RangeException(errorCode);
			default:
				throw SDOAbortedException(errorCode);
			}
		}

		return;
	}
}
				
void SDOProtocol::read(const uint16_t index, const uint8_t subIndex, CANMSG* pMessage, 
					 const ssr::TimeSpec & timeOut)
					 throw(TimeOutException, SDOAbortedException, CANException)
{
	m_pNode->checkNodeErrorState();
	sendUploadMessage(index, subIndex);
	waitMessage(index, subIndex, pMessage, timeOut);
}

void SDOProtocol::write(const uint16_t index, const uint8_t subIndex, 
					 const uint8_t* pData, const uint8_t size,
					 const ssr::TimeSpec & timeOut /* = ZEROTIME */)
					 throw(TimeOutException, SDOAbortedException, CANException)
{
	m_pNode->checkNodeErrorState();
	sendDownloadMessage(index, subIndex, pData, size);
	CANMSG dummy;
	waitMessage(index, subIndex, &dummy, timeOut);
}
