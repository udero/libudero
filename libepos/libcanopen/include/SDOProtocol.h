/**
 * @file SDOProtocol.h
 * @copyright RT Corp. 2012 All rights reserved.
 */
#ifndef SDOPROTOCOL_HEADER_INCLUDED
#define SDOPROTOCOL_HEADER_INCLUDED

// #include "Mutex.h"
#include "Thread.h"
#include "CANException.h"
#include "CANMSGBuffer.h"
#include "CANopen_defines.h"
#include "Timer.h"

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {

	class CANopen;
	class Node;
	class CANOPEN_API SDOProtocol {

	private:

		ssr::Timer m_Timer;


		CANopen* m_pCANopen;
		uint16_t m_NodeId;
		Node *m_pNode;

		CANMSGBuffer m_BufferSDO;



	public:
		/**
		 *
		 */
		SDOProtocol(CANopen* pCANopen, const uint16_t nodeId);


		/**
		 *
		 */
		~SDOProtocol();

		
	private:


		void sendDownloadMessage(const uint16_t Index,
						 const uint8_t subIndex,
						 const uint8_t* data, 
						 const uint8_t dataByte);
		
		void sendUploadMessage(const uint16_t index, const uint8_t subIndex);

		void waitMessage(const uint16_t index, const uint8_t subIndex, CANMSG* msg, const ssr::TimeSpec& timeOut) throw(TimeOutException);

	public:
		void setParentNode(Node *pNode) {
			m_pNode = pNode;
		}

		/**
		 * @brief Push CAN message to SDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		void pushMessage(const CANMSG& msg) throw() {
			m_BufferSDO.pushMessage(msg);
		}

		/**
		 * @brief Pop CAN message from SDO message buffer.
		 *
		 * @param msg [OUT]CAN message
		 */
		bool popMessage(CANMSG* msg, const ssr::TimeSpec& timeOut = INFINITETIME) throw() {
			return m_BufferSDO.popMessage(msg, timeOut);
		}


		/**
		 * @brief Write Object Dictionary using SDO protocol
		 * 
		 * @param index Index of Object Dictionary
		 * @param subIndex Sub Index value of Object Dictionary
		 * @param pData Data Array
		 * @param length Length of Data Array
		 * @param timeOut Timeout time scale (INFINITE if this argument is skipped)
		 * @throws TimeOutException 
		 */
		void write(const uint16_t index, const uint8_t subIndex, const uint8_t* pData,
			const uint8_t length,
			const ssr::TimeSpec &timeOut = INFINITETIME) throw(TimeOutException, SDOAbortedException, CANException);

		/**
		 * @brief Read Object Dictionary using SDO protocol
		 * 
		 * @param index Index of Object Dictionary
		 * @param subIndex Sub Index value of Object Dictionary
		 * @param pMessage [OUT] Returned CAN message 
		 * @param timeOut Timeout time scale (INFINITE if this argument is skipped)
		 * @throws TimeOutException 
		 */
		void read(const uint16_t index, const uint8_t subIndex, CANMSG* pMessage, 
			const ssr::TimeSpec &timeOut = INFINITETIME) throw(TimeOutException, SDOAbortedException, CANException);
	
		/**
		 * @brief Write Object Dictionary using SDO protocol
		 * 
		 * @param index Index of Object Dictionary
		 * @param subIndex Sub Index value of Object Dictionary
		 * @param arg Data
		 * @param timeOut Timeout time scale (INFINITE if this argument is skipped)
		 * @throws TimeOutException 
		 */
		template<typename T>
		void write(const uint16_t index, const uint8_t subIndex, T arg,
			const ssr::TimeSpec & timeOut = INFINITETIME) throw (TimeOutException, CANException) {
			uint8_t data[4];
			for(unsigned int i = 0;i < sizeof(T);i++) {
				data[i] = (uint8_t)((arg >> i*8) & 0x00FF);
			}
			write(index, subIndex, data, sizeof(T), timeOut);
		}

		/**
		 * @brief Read Object Dictionary using SDO protocol
		 * 
		 * @param index Index of Object Dictionary
		 * @param subIndex Sub Index value of Object Dictionary
		 * @param timeOut Timeout time scale (INFINITE if this argument is skipped)
		 * @return Read Data.
		 * @throws TimeOutException 
		 */
		template<typename T>
		T read(const uint16_t index, const uint8_t subIndex,
			const ssr::TimeSpec &timeOut = INFINITETIME) throw(TimeOutException, CANException) {
			CANMSG sdoMessage;
			read(index, subIndex, &sdoMessage, timeOut);
			T retval = 0;
			for(unsigned int i = 0;i < sizeof(T);i++) {
				retval |= ((T)sdoMessage.data[4+i]) << (8*i);
			}
			return retval;
		}


	};

}

#endif // #ifndef SDOPROTOCOL_HEADER_INCLUDED
