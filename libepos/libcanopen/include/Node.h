/**
 * @file Node.h
 */

#ifndef NODE_HEADER_INCLUDED
#define NODE_HEADER_INCLUDED


#include "Timer.h"
//#include "Mutex.h"
#include "Thread.h"
#include "CANMSGBuffer.h"
#include "CANopen_defines.h"
#include "SDOProtocol.h"
#include "PDOProtocol.h"

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {


	/**
	 * Exception class for CAN Interface errors.
	 */
	class NodeException : public CANException {
	
	public:
		NodeException(std::string msg) : CANException(msg) {
		}

    virtual ~NodeException() throw() {
		}
	};

	class HeartBeatTimeOutException : public NodeException {
	public:
		HeartBeatTimeOutException() : NodeException("HeartBeatTimeOut") {}
	};

	enum NODE_ERROR_STATE {
		ERROR_HEARTBEATTIMEOUT = 0x01,
	};

	class CANopen;


	/**
	 * @brief CANopen Node Class. Basic Functions for Node is implemented.
	 *
	 */
	class CANOPEN_API Node
	{
	private:
		friend class CANopen;
		friend class SDOProtocol;
		friend class PDOProtocol;

	private:



	protected:

		/**
		 *
		 */
		void setNodeErrorState(NODE_ERROR_STATE state) {
			m_NodeErrorState |= state;
		}

		/**
		 *
		 */
		void clearNodeErrorState(NODE_ERROR_STATE state) {
			m_NodeErrorState &= ~(static_cast<uint32_t>(state));
		}

		virtual void checkNodeErrorState(void);

	protected:
		ssr::Timer m_Timer;

	private:
		uint8_t m_NodeId;

		CANopen* m_pCANopen;

	public:
		uint8_t getNodeId() { return m_NodeId; }
	protected:
		SDOProtocol m_SDO;

		PDOProtocol m_PDO;

	private:
		NMT_STATE m_NMTState;

		uint32_t m_NodeErrorState;

		uint32_t m_EmergencyId;




	protected:

		CANMSGBuffer m_EMCYBuffer;

		CANMSGBuffer m_HeartBeatBuffer;


		uint16_t m_ProducerHeartBeatTime;

		ssr::Timer    m_HeartBeatTimer;
		ssr::TimeSpec m_HeartBeatTimeOut;
	public:


	public:

		/**
		 * @brief Constructor
		 * @param pCANopen CANopen object
		 * @param nodeId Node ID
		 */
		Node(CANopen* pCANopen, const uint8_t nodeId);

		/**
		 * @brief Destructor
		 */
		virtual ~Node(void);


		void initialize(void);
	public:

		/**
		 * @brief Get Node ID from buffer (no communication)
		 * @return CANopen Node ID
		 */
		uint8_t getNodeId() const throw() { return m_NodeId; }

		/**
		 * @brief Get EMCY ID from Buffer (no communication)
		 * @return EMCY ID
		 */
		uint32_t getEmergencyId() const throw() {
			return m_EmergencyId;
		}

		/**
		 * @brief Get NMT State from Buffer (no communication)
		 * @return NMT State
		 */
		NMT_STATE getNMTState() const throw() {
			return m_NMTState;
		}

		/**
		 * @brief Get Producer Heart Beat Time from buffer (no communication)
		 * @return Producer Heart Beat Time
		 */
		uint16_t getProducerHeartBeatTime() const { return m_ProducerHeartBeatTime; }
	private:
		/**
		 * @brief Push CAN message to SDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		void pushSdoMessage(const CANMSG& msg) throw() {
			m_SDO.pushMessage(msg);
		}

		/**
		 * @brief Pop CAN message from SDO message buffer.
		 *
		 * @param msg [OUT]CAN message
		 */
		bool popSdoMessage(CANMSG* msg, const ssr::TimeSpec& timeOut = INFINITETIME) throw() {
			return m_SDO.popMessage(msg, timeOut);
		}

		/**
		 * @brief Push CAN message to PDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		template<PDO_INDEX pdoIndex>
		void pushPDOMessage(const CANMSG& pdoMessage) throw() {
			m_PDO.pushMessage<pdoIndex>(pdoMessage);
		}


		/**
		 * @brief Pop CAN message to PDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		template<PDO_INDEX pdoIndex>
		bool popPDOMessage(CANMSG* pdoMessage, const ssr::TimeSpec& timeOut = INFINITETIME) throw() {
			return m_PDO.popMessage<pdoIndex>(pdoMessage, timeOut);
		}

		/**
		 * @brief Push CAN message to SDO Message Buffer
		 *
		 * @param msg CAN message
		 */
		void pushEmergencyMessage(const CANMSG& msg) throw() {
			m_EMCYBuffer.pushMessage(msg);
			onEmergency(msg);
		}

		/**
		 * @brief Pop CAN message from SDO message buffer.
		 *
		 * @param msg [OUT]CAN message
		 */
		bool popEmergencyMessage(CANMSG* msg, const ssr::TimeSpec& timeOut = INFINITETIME) throw() {
			return m_EMCYBuffer.popMessage(msg, timeOut);
		}

		/**
		 * @brief Push CAN message to Heart Beat Message Buffer
		 *
		 * @param msg CAN message
		 */
		void pushHeartBeatMessage(const CANMSG& msg) throw() {
			m_HeartBeatBuffer.pushMessage(msg);
			m_HeartBeatTimer.tick();
			m_NMTState = static_cast<NMT_STATE>(msg.data[0] & 0x7F);
		}

	public:

		void checkHeartBeatTimeOut() throw();

		/**
		 * @if jp
		 * @brief EMCYプロトコルメッセージを受け取った場合に呼ばれるイベントハンドラ．これをオーバーロードしてデバイス独自の処理を行う
		 * @param msg CANメッセージ
		 * @else
		 *
		 * @endif
		 */
		virtual void onEmergency(const CANMSG& msg) throw() {

		}

		uint8_t readErrorRegister(const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint8_t>(OD_INDEX_ERRORREGISTER, OD_SUBINDEX_ERRORREGISTER, timeOut);
		}

		uint8_t readNumberOfErrors(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint8_t>(OD_INDEX_NUMBEROFERRORS, OD_SUBINDEX_NUMBEROFERRORS, timeOut);
		}

		/**
		 * @brief Read Error History
		 * @param index Index of Error Histor [1-5]
		 * @return Error Number
		 */
		uint32_t readErrorHistory(const uint8_t index, const ssr::TimeSpec &timeOut =  INFINITETIME) throw(CANException) {
			if(index < 1 || index > 5) return 0;
			return m_SDO.read<uint32_t>(OD_INDEX_ERRORHISTORY, index, timeOut);
		}

		/**
		 * @brief Erase all Error History
		 * @param timeOut Timeout value
		 */
		void flushErrors(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			m_SDO.write<uint8_t>(OD_INDEX_NUMBEROFERRORS, OD_SUBINDEX_NUMBEROFERRORS, 0, timeOut);
		}

		/**
		 * @brief Save All Parameters
		 * @param timeOut Timeout value
		 */
		void saveAllParameters(const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			m_SDO.write<uint32_t>(OD_INDEX_SAVEALLPARAMETERS, OD_SUBINDEX_SAVEALLPARAMETERS, 0x65766173, timeOut);
		}

		/**
		 * @brief Load All Default Parameters
		 * @param timeOut Timeout value
		 */
		void restoreDefaultParameters(const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			m_SDO.write<uint32_t>(OD_INDEX_RESTOREDEFAULTPARAMETERS, OD_SUBINDEX_RESTOREDEFAULTPARAMETERS, 0x64616F6C, timeOut);
		}

		/**
		 * read COB-ID for EMCY protocol
		 * @param timeOut Time Out
		 * @return COB-ID EMCY
		 */
		uint32_t readCOBIDEmergency(const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint32_t>(OD_INDEX_COBIDEMCY, OD_SUBINDEX_COBIDEMCY, timeOut);
		}

		/**
		 * @brief Read Consumer 1 Heart Beat Time
		 * @param timeOut Time Out
		 * @return Consumer Heart Beat Time [ms]
		 */
		uint16_t readConsumer1HeartBeatTime(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			uint32_t buf = m_SDO.read<uint32_t>(OD_INDEX_CONSUMER1HEARTBEATTIME, OD_SUBINDEX_CONSUMER1HEARTBEATTIME, timeOut);
			return static_cast<uint16_t>(buf & 0xFFFF);
		}


		/**
		 * @brief Read Consumer 2 Heart Beat Time
		 * @param timeOut Time Out
		 * @return Consumer Heart Beat Time [ms]
		 */
		uint16_t readConsumer2HeartBeatTime(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			uint32_t buf = m_SDO.read<uint32_t>(OD_INDEX_CONSUMER2HEARTBEATTIME, OD_SUBINDEX_CONSUMER2HEARTBEATTIME, timeOut);
			return static_cast<uint16_t>(buf & 0xFFFF);
		}

		
		/**
		 * @brief Write Consumer 1 Heart Beat Time
		 * @param time Consumer Heart Beat Time [ms]
		 * @param timeOut Time Out
		 */
		void writeConsumer1HeartBeatTime(const uint16_t time, const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			uint32_t buf = (static_cast<uint32_t>(getNodeId()) << 16) | (time);
			m_SDO.write<uint32_t>(OD_INDEX_CONSUMER1HEARTBEATTIME, OD_SUBINDEX_CONSUMER1HEARTBEATTIME, buf, timeOut);
		}

		/**
		 * @brief Write Consumer 2 Heart Beat Time
		 * @param time Consumer Heart Beat Time [ms]
		 * @param timeOut Time Out
		 */
		void writeConsumer2HeartBeatTime(const uint16_t time, const ssr::TimeSpec & timeOut = INFINITETIME) throw(CANException) {
			uint32_t buf = (static_cast<uint32_t>(getNodeId()) << 16) | (time);
			m_SDO.write<uint32_t>(OD_INDEX_CONSUMER2HEARTBEATTIME, OD_SUBINDEX_CONSUMER2HEARTBEATTIME, buf, timeOut);
		}

		/**
		 * @brief Read Producer Heart Beat Time
		 * @return Producer Heat Beat Time [ms]
		 * @param timeOut Time Out
		 */
		uint16_t readProducerHeartBeatTime(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint16_t>(OD_INDEX_PRODUCERHEARTBEATTIME, OD_SUBINDEX_PRODUCERHEARTBEATTIME, timeOut);
		}

		/**
		 * @brief Read Producer Heart Beat Time
		 * @param time Producer Heat Beat Time [ms]
		 * @param timeOut Time Out
		 */
		void writeProducerHeartBeatTime(const uint16_t time, const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			m_SDO.write<uint16_t>(OD_INDEX_PRODUCERHEARTBEATTIME, OD_SUBINDEX_PRODUCERHEARTBEATTIME, time, timeOut);
			m_ProducerHeartBeatTime = time;
			m_HeartBeatTimeOut = ssr::TimeSpec((time+5) * 1000);
		}

		/**
		 * @brief Get Vendor Id
		 * 
		 * @param timeOut Time Out Span
		 */
		uint32_t readVendorId(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint32_t>(OD_INDEX_VENDORID, OD_SUBINDEX_VENDORID, timeOut);
		}

		/**
		 * @brief Get Product Code
		 * 
		 * @param timeOut Time Out Span
		 */
		uint32_t readProductCode(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint32_t>(OD_INDEX_PRODUCTCODE, OD_SUBINDEX_PRODUCTCODE, timeOut);
		}

		/**
		 * @brief Get Revision Number
		 * 
		 * @param timeOut Time Out Span
		 */
		uint32_t readRevisionNumber(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint32_t>(OD_INDEX_REVISIONNUMBER, OD_SUBINDEX_REVISIONNUMBER, timeOut);
		}

		/**
		 * @brief Get Revision Number
		 * 
		 * @param timeOut Time Out Span
		 */
		uint32_t readSerialNumber(const ssr::TimeSpec &timeOut = INFINITETIME) throw(CANException) {
			return m_SDO.read<uint32_t>(OD_INDEX_SERIALNUMBER, OD_SUBINDEX_SERIALNUMBER, timeOut);
		}


	public:

		/**
		 * @brief NMT start remote node
		 */
		void start(void);

		/**
		 * @brief  NMT stop remote node
		 */
		void stop(void);

		/**
		 * @brief NMT reset node
		 */
		void reset(void);

		/**
		 * @brief NMT enter pre operation
		 */
		void enterPreOperational(void);

		/**
		 * @brief NMT reset communication
		 */
		void resetCommunication(void);



		/**
		*/
		template<PDO_INDEX pdoIndex>
		void setupTXPDOParameter(const bool valid, const bool RTR, const ssr::TimeSpec& timeout = INFINITETIME) throw(CANException) {
			uint32_t value1 = !valid ? 0x80000000 : 0;
			value1 |= !RTR ? 0x40000000 : 0;
			uint16_t index1;
			uint8_t subindex1;

			switch (pdoIndex) {
			case PDO1:
				value1 |= COBID_TXPDO1_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDTRANSMITPDO1;
				subindex1 = OD_SUBINDEX_COBIDTRANSMITPDO1;
				break;
			case PDO2:
				value1 |= COBID_TXPDO2_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDTRANSMITPDO2;
				subindex1 = OD_SUBINDEX_COBIDTRANSMITPDO2;
				break;
			case PDO3:
				value1 |= COBID_TXPDO3_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDTRANSMITPDO3;
				subindex1 = OD_SUBINDEX_COBIDTRANSMITPDO3;
				break;
			case PDO4:
				value1 |= COBID_TXPDO4_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDTRANSMITPDO4;
				subindex1 = OD_SUBINDEX_COBIDTRANSMITPDO4;
				break;
			}
			m_SDO.write<uint32_t>(index1, subindex1, value1, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOTransmissionType(const bool synchronous, const bool RTR, const ssr::TimeSpec& timeout = INFINITETIME) {
			uint16_t index2;
			uint8_t subindex2;

			switch (pdoIndex) {
			case PDO1:
				index2 = OD_INDEX_TRANSMISSIONTYPETRANSMITPDO1;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO1;
				break;
			case PDO2:
				index2 = OD_INDEX_TRANSMISSIONTYPETRANSMITPDO2;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO2;
				break;
			case PDO3:
				index2 = OD_INDEX_TRANSMISSIONTYPETRANSMITPDO3;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO3;
				break;
			case PDO4:
				index2 = OD_INDEX_TRANSMISSIONTYPETRANSMITPDO4;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPETRANSMITPDO4;
				break;
			}

			uint8_t value2 = 0;
			if (synchronous) {
				value2 = 1;
			}
			else {
				if (RTR) {
					value2 = 253;
				}
				else {
					value2 = 255;
				}
			}
			m_SDO.write<uint8_t>(index2, subindex2, value2, timeout);

		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOTransmissionInhibitTime(const uint32_t usec, const ssr::TimeSpec& timeout = INFINITETIME) {
			uint16_t index2;
			uint8_t subindex2;

			switch (pdoIndex) {
			case PDO1:
				index2 = OD_INDEX_INHIBITTIMETRANSMITPDO1;
				subindex2 = OD_SUBINDEX_INHIBITTIMETRANSMITPDO1;
				break;
			case PDO2:
				index2 = OD_INDEX_INHIBITTIMETRANSMITPDO2;
				subindex2 = OD_SUBINDEX_INHIBITTIMETRANSMITPDO2;
				break;
			case PDO3:
				index2 = OD_INDEX_INHIBITTIMETRANSMITPDO3;
				subindex2 = OD_SUBINDEX_INHIBITTIMETRANSMITPDO3;
				break;
			case PDO4:
				index2 = OD_INDEX_INHIBITTIMETRANSMITPDO4;
				subindex2 = OD_SUBINDEX_INHIBITTIMETRANSMITPDO4;
				break;
			}

			m_SDO.write<uint16_t>(index2, subindex2, (uint16_t)(usec / 100), timeout);

		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOMappingNumber(const uint8_t number, const ssr::TimeSpec& timeout = INFINITETIME) {
			uint16_t index;
			uint8_t subindex;
			switch (pdoIndex) {
			case PDO1:
				index = OD_INDEX_TRANSMITPDO1MAPPINGNUMBER;
				subindex = OD_SUBINDEX_TRANSMITPDO1MAPPINGNUMBER;
				break;
			case PDO2:
				index = OD_INDEX_TRANSMITPDO2MAPPINGNUMBER;
				subindex = OD_SUBINDEX_TRANSMITPDO2MAPPINGNUMBER;
				break;
			case PDO3:
				index = OD_INDEX_TRANSMITPDO3MAPPINGNUMBER;
				subindex = OD_SUBINDEX_TRANSMITPDO3MAPPINGNUMBER;
				break;
			case PDO4:
				index = OD_INDEX_TRANSMITPDO4MAPPINGNUMBER;
				subindex = OD_SUBINDEX_TRANSMITPDO4MAPPINGNUMBER;
				break;
			}

			m_SDO.write<uint8_t>(index, subindex, number, timeout);
		}


		template<PDO_INDEX pdoIndex>
		void setupTXPDOMappingObjects(const uint32_t numObject, const uint32_t objects[], const ssr::TimeSpec& timeout = INFINITETIME) {
			setupTXPDOMappingNumber<pdoIndex>(0);
			uint16_t index;
			switch (pdoIndex) {
			case PDO1:
				index = OD_INDEX_TRANSMITPDO1MAPPINGOBJECT1;
				break;
			case PDO2:
				index = OD_INDEX_TRANSMITPDO2MAPPINGOBJECT2;
				break;
			case PDO3:
				index = OD_INDEX_TRANSMITPDO3MAPPINGOBJECT3;
				break;
			case PDO4:
				index = OD_INDEX_TRANSMITPDO4MAPPINGOBJECT4;
				break;
			}
			for (uint32_t i = 0; i < numObject; i++) {
				m_SDO.write<uint32_t>(index, OD_SUBINDEX_TRANSMITPDO1MAPPINGOBJECT1 + i, objects[i]);
			}
			setupTXPDOMappingNumber<pdoIndex>(numObject);
		}


		template<PDO_INDEX pdoIndex>
		void setupTXPDOMapping(const uint32_t object1, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[1] = { object1 };
			setupTXPDOMappingObjects<pdoIndex>(1, objects, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOMapping(const uint32_t object1, const uint32_t object2, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[2] = { object1, object2 };
			setupTXPDOMappingObjects<pdoIndex>(2, objects, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOMapping(const uint32_t object1, const uint32_t object2, const uint32_t object3, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[3] = { object1, object2, object3 };
			setupTXPDOMappingObjects<pdoIndex>(3, objects, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupTXPDOMapping(const uint32_t object1, const uint32_t object2, const uint32_t object3, const uint32_t object4, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[4] = { object1, object2, object3, object4 };
			setupTXPDOMappingObjects<pdoIndex>(4, objects, timeout);
		}

		/**
		*/
		template<PDO_INDEX pdoIndex>
		void setupRXPDOParameter(const bool valid, const bool RTR, const ssr::TimeSpec& timeout = INFINITETIME) throw(CANException) {
			uint32_t value1 = !valid ? 0x80000000 : 0;
			value1 |= !RTR ? 0x40000000 : 0;
			uint16_t index1;
			uint8_t subindex1;

			switch (pdoIndex) {
			case PDO1:
				value1 |= COBID_RXPDO1_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDRECEIVEPDO1;
				subindex1 = OD_SUBINDEX_COBIDRECEIVEPDO1;
				break;
			case PDO2:
				value1 |= COBID_RXPDO2_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDRECEIVEPDO2;
				subindex1 = OD_SUBINDEX_COBIDRECEIVEPDO2;
				break;
			case PDO3:
				value1 |= COBID_RXPDO3_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDRECEIVEPDO3;
				subindex1 = OD_SUBINDEX_COBIDRECEIVEPDO3;
				break;
			case PDO4:
				value1 |= COBID_RXPDO4_OFFSET | getNodeId();
				index1 = OD_INDEX_COBIDRECEIVEPDO4;
				subindex1 = OD_SUBINDEX_COBIDRECEIVEPDO4;
				break;
			}
			m_SDO.write<uint32_t>(index1, subindex1, value1, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOTransmissionType(const bool synchronous, const ssr::TimeSpec& timeout = INFINITETIME) {
			uint16_t index2;
			uint8_t subindex2;

			switch (pdoIndex) {
			case PDO1:
				index2 = OD_INDEX_TRANSMISSIONTYPERECEIVEPDO1;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO1;
				break;
			case PDO2:
				index2 = OD_INDEX_TRANSMISSIONTYPERECEIVEPDO2;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO2;
				break;
			case PDO3:
				index2 = OD_INDEX_TRANSMISSIONTYPERECEIVEPDO3;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO3;
				break;
			case PDO4:
				index2 = OD_INDEX_TRANSMISSIONTYPERECEIVEPDO4;
				subindex2 = OD_SUBINDEX_TRANSMISSIONTYPERECEIVEPDO4;
				break;
			}

			uint8_t value2 = 0;
			if (synchronous) {
				value2 = 1;
			}
			else {
				value2 = 255;
			}
			m_SDO.write<uint8_t>(index2, subindex2, value2, timeout);

		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMappingNumber(const uint8_t number, const ssr::TimeSpec& timeout = INFINITETIME) {
			uint16_t index;
			uint8_t subindex;
			switch (pdoIndex) {
			case PDO1:
				index = OD_INDEX_RECEIVEPDO1MAPPINGNUMBER;
				subindex = OD_SUBINDEX_RECEIVEPDO1MAPPINGNUMBER;
				break;
			case PDO2:
				index = OD_INDEX_RECEIVEPDO2MAPPINGNUMBER;
				subindex = OD_SUBINDEX_RECEIVEPDO2MAPPINGNUMBER;
				break;
			case PDO3:
				index = OD_INDEX_RECEIVEPDO3MAPPINGNUMBER;
				subindex = OD_SUBINDEX_RECEIVEPDO3MAPPINGNUMBER;
				break;
			case PDO4:
				index = OD_INDEX_RECEIVEPDO4MAPPINGNUMBER;
				subindex = OD_SUBINDEX_RECEIVEPDO4MAPPINGNUMBER;
				break;
			}

			m_SDO.write<uint8_t>(index, subindex, number, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMappingObjects(const uint32_t numObject, const uint32_t objects[], const ssr::TimeSpec& timeout = INFINITETIME) {
			setupRXPDOMappingNumber<pdoIndex>(0);
			uint16_t index;
			switch (pdoIndex) {
			case PDO1:
				index = OD_INDEX_RECEIVEPDO1MAPPINGOBJECT1;
				break;
			case PDO2:
				index = OD_INDEX_RECEIVEPDO2MAPPINGOBJECT1;
				break;
			case PDO3:
				index = OD_INDEX_RECEIVEPDO3MAPPINGOBJECT1;
				break;
			case PDO4:
				index = OD_INDEX_RECEIVEPDO4MAPPINGOBJECT1;
				break;
			}
			for (uint32_t i = 0; i < numObject; i++) {
				m_SDO.write<uint32_t>(index, OD_SUBINDEX_RECEIVEPDO1MAPPINGOBJECT1 + i, objects[i], timeout);
			}
			setupRXPDOMappingNumber<pdoIndex>(numObject);

		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMapping(const uint32_t object1, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[1] = { object1 };
			setupRXPDOMappingObjects<pdoIndex>(1, objects);
		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMapping(const uint32_t object1, const uint32_t object2, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[2] = { object1, object2 };
			setupRXPDOMappingObjects<pdoIndex>(2, objects, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMapping(const uint32_t object1, const uint32_t object2, const uint32_t object3, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[3] = { object1, object2, object3 };
			setupRXPDOMappingObjects<pdoIndex>(3, objects, timeout);
		}

		template<PDO_INDEX pdoIndex>
		void setupRXPDOMapping(const uint32_t object1, const uint32_t object2, const uint32_t object3, const uint32_t object4, const ssr::TimeSpec& timeout = INFINITETIME) {
			const uint32_t objects[4] = { object1, object2, object3, object4 };
			setupRXPDOMappingObjects<pdoIndex>(4, objects, timeout);
		}


		template<PDO_INDEX pdoIndex, typename T>
		void readByPDO(T* t, const ssr::TimeSpec& timeout = INFINITETIME) {
			m_PDO.readByPDO<pdoIndex, T>(t, timeout);
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		void readByPDO(T1 *t1, T2 *t2, const ssr::TimeSpec& timeout = INFINITETIME) {
			m_PDO.readByPDO<pdoIndex, T1, T2>(t1, t2, timeout);
		}

		template<PDO_INDEX pdoIndex, typename T>
		void sendByPDO(const T& t) {
			m_PDO.sendByPDO<pdoIndex, T>(t);
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		void sendByPDO(const T1& t1, const T2& t2) {
			m_PDO.sendByPDO<pdoIndex, T1, T2>(t1, t2);
		}

		template<PDO_INDEX pdoIndex, typename T1>
		bool tryReadByPDO(T1 *t1) {
			return m_PDO.tryReadByPDO<pdoIndex, T1>(t1);
		}

		template<PDO_INDEX pdoIndex, typename T1, typename T2>
		bool tryReadByPDO(T1 *t1, T2 *t2) {
			return m_PDO.tryReadByPDO<pdoIndex, T1, T2>(t1, t2);
		}
	public:
		void sync(void);
	};


}


#endif
