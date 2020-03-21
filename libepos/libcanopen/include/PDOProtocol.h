/**
 * @file PDOProtocol.h
 * @copyright RT Corp. 2012 All rights reserved.
 */

#ifndef PDOPROTOCOL_HEADER_INCLUDED
#define PDOPROTOCOL_HEADER_INCLUDED

//#include "Mutex.h"
#include "Thread.h"
#include "CANException.h"
#include "CANMSGBuffer.h"
#include "CANopen_defines.h"
#include "Timer.h"

#include <sstream>


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

	class CANOPEN_API PDOProtocol {

	private:

		ssr::Timer m_Timer;

		CANopen* m_pCANopen;

		Node* m_pNode;

		CANMSGBuffer m_BufferPDO[4];

		uint16_t m_NodeId;

		void checkNodeErrorState(void);
		void send(const CANMSG& msg);
	public:
		/**
		 *
		 */
		PDOProtocol(CANopen* pCANopen, const uint16_t nodeId);

		/**
		 *
		 */
		~PDOProtocol();

			
		void setParentNode(Node* pNode) {
			m_pNode = pNode;
		}

		/**
		 * @brief Send NMT Message
		 *
		 * @param nodeId TargetNode ID
		 * @param message NMT message
		 * @see NMT_MESSAGE
		 */
		void sendNMT(const NMT_MESSAGE message);

		/**
		 * @brief Send Sync Object
		 */
		void sendSync(void);


		/**
		 * @brief Push CAN message to PDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		template<PDO_INDEX pdoIndex>
		void pushMessage(const CANMSG& pdoMessage) throw() {
			m_BufferPDO[pdoIndex].pushMessage(pdoMessage);
		}


		/**
		 * @brief Pop CAN message to PDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		template<PDO_INDEX pdoIndex>
		bool popMessage(CANMSG* pdoMessage, const ssr::TimeSpec& timeOut = INFINITETIME) throw() {
			return m_BufferPDO[pdoIndex].popMessage(pdoMessage, timeOut);
		}

		template<PDO_INDEX pdoIndex>
		CANMSG getCurrentMessage(void) throw() {
			return m_BufferPDO[pdoIndex].getCurrentMessage();
		}

	private:

		/**
		 * @brief Send PDO Message
		 *
		 * @param pdoIndex PDO index number 
		 * @param length Length of Data
		 * @param data Data Array
		 */
		void sendPDO(const PDO_INDEX pdoIndex, const uint8_t length, const uint8_t data[]);

	public:

		/**
		* @brief
		* @param
		* @param
		*/
		template<PDO_INDEX pdoIndex, typename T>
		void sendByPDO(T data) {
			checkNodeErrorState();

			CANMSG msg;
			msg.id = 0x200 + 0x100 * pdoIndex + m_NodeId;
			msg.rtr = 0;
			msg.length = sizeof(T);
			int dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T); i++) {
				msg.data[dataIndex] = (uint8_t)((data >> i * 8) & 0xFF);
				dataIndex++;
			}
			send(msg);
		}


		/** 
		 * @brief 
		 * @param
		 * @param
		 */
		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		void sendByPDO(T1 data1, T2 data2) {
			checkNodeErrorState();

			CANMSG msg;
			msg.id = 0x200 + 0x100 * pdoIndex + m_NodeId;
			msg.rtr = 0;
			msg.length = sizeof(T1) + sizeof(T2);
			int dataIndex = 0;
			for(unsigned int i = 0;i < sizeof(T1);i++) {
				msg.data[dataIndex] = (uint8_t)((data1 >> i*8) & 0xFF);
				dataIndex++;
			}
			for(unsigned int i = 0;i < sizeof(T2);i++) {
				msg.data[dataIndex] = (uint8_t)((data2 >> i*8) & 0xFF);
				dataIndex++;
			}
			send(msg);
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2, typename T3>
		void sendByPDO(T1 data1, T2 data2, T3 data3) {
			checkNodeErrorState();

			CANMSG msg;
			msg.id = 0x200 + 0x100 * pdoIndex + m_NodeId;
			msg.rtr = 0;
			msg.length = sizeof(T1) + sizeof(T2) + sizeof(T3);
			int dataIndex = 0;
			for(unsigned int i = 0;i < sizeof(T1);i++) {
				msg.data[dataIndex] = (uint8_t)((data1 >> i*8) & 0xFF);
				dataIndex++;
			}
			for(unsigned int i = 0;i < sizeof(T2);i++) {
				msg.data[dataIndex] = (uint8_t)((data2 >> i*8) & 0xFF);
				dataIndex++;
			}
			for(unsigned int i = 0;i < sizeof(T3);i++) {
				msg.data[dataIndex] = (uint8_t)((data3 >> i*8) & 0xFF);
				dataIndex++;
			}
			send(msg);
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		void readByPDO(T1* data1, T2* data2, const ssr::TimeSpec& timeOut = INFINITETIME) {
			checkNodeErrorState();

			CANMSG msg;
			if (!popMessage<pdoIndex>(&msg, timeOut)) {
				std::stringstream ss;
				ss << "PDOProtocol::readByPDO<" << "node_id = " << this->m_NodeId << ">(" << pdoIndex << ", " << "#2" << ")";
				throw TimeOutException(ss.str().c_str());
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T1); i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data2 = 0;
			for (unsigned int i = 0; i < sizeof(T2); i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
		}



		template<PDO_INDEX pdoIndex, typename T1, typename T2, typename T3>
		void readByPDO(T1* data1, T2* data2, T3* data3, const ssr::TimeSpec& timeOut = INFINITETIME) {
			checkNodeErrorState();

			CANMSG msg;
			if(!popMessage<pdoIndex>(&msg, timeOut)) {
				throw TimeOutException("PDOProtocol::readByPDO");
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for(unsigned int i = 0;i < sizeof(T1);i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
			*data2 = 0;
			for(unsigned int i = 0;i < sizeof(T2);i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
			*data3 = 0;
			for(unsigned int i = 0;i < sizeof(T3);i++) {
				*data3 |= ((T3)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
		}

		
		template<PDO_INDEX pdoIndex, typename T1, typename T2, typename T3>
		void getCurrentMessage(T1* data1, T2* data2, T3* data3) {
			checkNodeErrorState();

			CANMSG msg = getCurrentMessage<pdoIndex>();

			*data1 = 0;
			uint32_t dataIndex = 0;
			for(unsigned int i = 0;i < sizeof(T1);i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
			*data2 = 0;
			for(unsigned int i = 0;i < sizeof(T2);i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
			*data3 = 0;
			for(unsigned int i = 0;i < sizeof(T3);i++) {
				*data3 |= ((T3)msg.data[dataIndex]) << i*8;
				dataIndex++;
			}
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2, typename T3>
		void tryReadMessage(T1* data1, T2* data2, T3* data3) {
			checkNodeErrorState();

			CANMSG msg;
			if (!popMessage<pdoIndex>(&msg, ZEROTIME)) {
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T1); i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data2 = 0;
			for (unsigned int i = 0; i < sizeof(T2); i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data3 = 0;
			for (unsigned int i = 0; i < sizeof(T3); i++) {
				*data3 |= ((T3)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
		}

		template<PDO_INDEX pdoIndex, typename T1>
		bool tryReadByPDO(T1* data1) {
			checkNodeErrorState();

			CANMSG msg;
			if (!popMessage<pdoIndex>(&msg, ZEROTIME)) {
				return false;
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T1); i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			return true;
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		bool tryReadByPDO(T1* data1, T2* data2) {
			checkNodeErrorState();

			CANMSG msg;
			if (!popMessage<pdoIndex>(&msg, ZEROTIME)) {
				return false;
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T1); i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data2 = 0;
			for (unsigned int i = 0; i < sizeof(T2); i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			return true;
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2, typename T3>
		bool tryReadByPDO(T1* data1, T2* data2, T3* data3) {
			checkNodeErrorState();

			CANMSG msg;
			if (!popMessage<pdoIndex>(&msg, ZEROTIME)) {
				return false;
			}

			*data1 = 0;
			uint32_t dataIndex = 0;
			for (unsigned int i = 0; i < sizeof(T1); i++) {
				*data1 |= ((T1)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data2 = 0;
			for (unsigned int i = 0; i < sizeof(T2); i++) {
				*data2 |= ((T2)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			*data3 = 0;
			for (unsigned int i = 0; i < sizeof(T3); i++) {
				*data3 |= ((T3)msg.data[dataIndex]) << i * 8;
				dataIndex++;
			}
			return true;
		}



	};
}

#endif
