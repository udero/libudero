/**
 * @file EPOS.h
 */

#pragma once

/**
 * @brief DLL�G�N�X�|�[�g�̐ݒ�
 */
#ifdef WIN32                          // Windows�p�̐ݒ�

#define IMPORT_canopen_STATIC
#ifdef epos_EXPORTS                        // DLL���R���p�C���̏ꍇ
#define EPOS_API __declspec(dllexport)
#else                                 // ���[�U���R���p�C���̐ݒ�
#ifdef EXPORT_epos_STATIC               
#define EPOS_API                       // �X�^�e�B�b�N���C�u�����p                  
#else
#ifdef USE_canopen_STATIC
#define EPOS_API
#else
#define EPOS_API __declspec(dllimport) // �_�C�i�~�b�N�����N�p
#endif
#endif 
#endif
#else 
#define EPOS_API                       // Linux�p
#endif


#define USE_canopen_STATIC
#include "CANopen.h"
#include "Node.h"
#include "EPOSException.h"
#include "EPOS_defines.h"

#include <string>



namespace maxon {


	/**
	 * @brief maxon EPOS class
	 */
	class EPOS_API EPOS : public canopen::Node
	{
	private:

		uint16_t m_ErrorCode;

	private:
		/**
		 * This function is used to set m_ErrorCode member variable to detect background job's exception.
		 */
		void setErrorCode(uint16_t errorCode);

	public:

		/**
		 *
		 */
		void clearErrorCode() {
			m_ErrorCode = EPOS_ERROR_NOERROR;
		}

		/**
		 *
		 */
		virtual void checkNodeErrorState(void);

	public:
		/**
		 * Constructor
		 */
		EPOS(canopen::CANopen* pCANopen, const uint8_t nodeId);

		/**
		 * Destructor
		 */
		virtual ~EPOS(void);

	public:

	public:

		/**
		 * @brief Set Operation Mode
		 *
		 * @see MODESOFOPERATION
		 */
		void setOperationMode(MODESOFOPERATION mode, const ssr::TimeSpec& timeout = INFINITETIME);

		/**
		 * @brief Get Operation Mode
		 *
		 * @see MODESOFOPERATION
		 */
		MODESOFOPERATION getOperationMode(void);



	public:

		bool isHomed(const ssr::TimeSpec& timeout = INFINITETIME);
		/**
		 * EPOS�̎��s��Ԃ��m�F����
		 * @param �^�C���A�E�g�D
		 * @return EPOS_STATE�^�̎��s��ԕϐ�
		 * @throws TimeoutException
		 */
		EPOS_STATE getState(const ssr::TimeSpec& timeout = INFINITETIME);

		EPOS_STATE getStateFromStatusWord(const uint16_t statusWord);

		/**
		* EPOS�̎��s��Ԃ��m�F���C������ŏo�͂���
		* @param �^�C���A�E�g�D
		* @return EPOS_STATE�ɑΉ����镶����
		* @throws TimeoutException
		*/
		std::string getStateStr(const ssr::TimeSpec& timeout = INFINITETIME) {
			return getStateStrFromState(getState(timeout));
		}

		/**
		* EPOS�̎��s��Ԃ��當����𐶐�����D�ʐM�͍s�킸�ɒ����ɕς���
		* @param state EPOS_STATE�^�̏��
		* @return EPOS_STATE�ɑΉ����镶����
		* @throws TimeoutException
		*/
		std::string getStateStrFromState(const EPOS_STATE& state);


		/**
		 * This function will be overridden to detect errors.
		 */
		virtual void onEmergency(const canopen::CANMSG& msg) throw();

		/**
		* �R���g���[�����[�h����������
		* @param controlWord �R���g���[�����[�h�l�D
		* @param timeout �^�C���A�E�g����
		* @throws TimeoutException 
		*/
		void setControlWord(const uint16_t controlWord, const ssr::TimeSpec& timeout = INFINITETIME);

		void quickStop(const ssr::TimeSpec& timeout = INFINITETIME);

		uint16_t getStatusWord(const ssr::TimeSpec& timeout = INFINITETIME);

		void faultReset(const ssr::TimeSpec& timeout = INFINITETIME);

		void shutDown(const ssr::TimeSpec& timeout = INFINITETIME);

		void switchOn(const ssr::TimeSpec& timeout = INFINITETIME);

		void disableVoltage(const ssr::TimeSpec& timeout = INFINITETIME);

		void switchOff(const ssr::TimeSpec& timeout = INFINITETIME);

		void enableOperation(const ssr::TimeSpec& timeout = INFINITETIME);

		void disableOperation(const ssr::TimeSpec& timeout = INFINITETIME);

		void waitStateChanged(const EPOS_STATE targetState, const uint32_t try_count=10, const ssr::TimeSpec& sleepTime = ssr::TimeSpec(500*1000), const ssr::TimeSpec& timeout = INFINITETIME);


		void switchOnAndEnableOperation(const ssr::TimeSpec& timeOut = INFINITETIME);


		void waitTargetReached(const ssr::TimeSpec timeOut = ZEROTIME);

		void halt(const ssr::TimeSpec& timeout = INFINITETIME);
	public:

		/**
		 *
		 */
		void setTargetPosition(const int32_t position, const ssr::TimeSpec& timeout=INFINITETIME);

		/**
		*
		*/
		void setMinimumPositionLimit(const int32_t position);

		/**
		*
		*/
		void setMaximumPositionLimit(const int32_t position);

		/**
		*
		*/
		int32_t getMinimumPositionLimit(void);

		/**
		*
		*/
		int32_t getMaximumPositionLimit(void);

		/**
		*
		*/
		int32_t getActualPosition(const ssr::TimeSpec& timeout = INFINITETIME);

	public:
		/**
		*
		*/
		void setTargetVelocity(const int32_t rpm, const ssr::TimeSpec& timeout = INFINITETIME);

		/**
		 * 
		 */
		uint32_t getMaximumProfileVelocity(void);

		/**
		 * Set Max Velocity. [rpm]
		 */
		void setMaximumProfileVelocity(const uint32_t rpm);

		/**
		 *
		 */
		void setProfileVelocity(const uint32_t rpm, const ssr::TimeSpec& timeout = INFINITETIME);

		/**
		* @brief Get Current Velocity
		*/
		int32_t getActualVelocity(const ssr::TimeSpec& timeOut = INFINITETIME);


	public:

		void setAnalogCurrentSetpointScaling(const int16_t scaling, const ssr::TimeSpec& timeout);

		void setTargetCurrent(const int16_t milliAmp, const ssr::TimeSpec& timeout = INFINITETIME);

		int16_t getActualCurrent(const ssr::TimeSpec& timeout = INFINITETIME);

	public:

		void homing(const bool block = true);

		void setHomePosition(const int32_t homePosition, const ssr::TimeSpec& timeout = INFINITETIME);

		void setHomeOffset(const int32_t homeOffset, const ssr::TimeSpec& timeout = INFINITETIME);

		void setHomingSpeed(const uint32_t speedSwitchSearch, const uint32_t speedZeroSearch, const ssr::TimeSpec& timeout = INFINITETIME);

		uint32_t getHomingSpeed(const ssr::TimeSpec& timeout = INFINITETIME);

		void setHomingAcceleration(const uint32_t accel, const ssr::TimeSpec& timeout = INFINITETIME);

		void setHomingMethod(const HOMINGMETHOD homingMethod, const ssr::TimeSpec& timeout = INFINITETIME);

		void setCurrentThoresholdForHomingMode(const uint16_t milliAmp, const ssr::TimeSpec& timeout);

		void waitHomingAttained(const ssr::TimeSpec timeOut = INFINITETIME);

	public:

		/**
		* �f�W�^���o�͂�ݒ�
		* @param status �f�W�^���o�̓s���̏�ԁD���f���ɂ���ĈقȂ�̂Œ���
		*/
		void setDigitalOutput(const uint16_t status, const ssr::TimeSpec& timeout = INFINITETIME);

		void setDigitalOutputMask(const uint16_t mask, const ssr::TimeSpec& timeout);

		void setDigitalOutputPolarity(const uint16_t polarity, const ssr::TimeSpec& timeout);
		/**
		* �f�W�^�����̓s���̏�Ԋm�F
		* @return �f�W�^�����̓s���̏�ԁD���f���ɂ���ĈقȂ�̂Œ���
		*/
		uint16_t getDigitalInput(const ssr::TimeSpec& timeout = INFINITETIME);

		/*
		* �A�i���O����1 [mV]
		*/
		int16_t getAnalogInput1(const ssr::TimeSpec& timeout = INFINITETIME);

		/*
		* �A�i���O����2 [mV]
		*/
		int16_t getAnalogInput2(const ssr::TimeSpec& timeout = INFINITETIME);

		void setProfileAcceleration(const uint32_t rpmPerSec, const ssr::TimeSpec& timeout = INFINITETIME);

		void setProfileDeceleration(const uint32_t rpmPerSec, const ssr::TimeSpec& timeout = INFINITETIME);

		void setMaximalFollowingError(const uint32_t error, const ssr::TimeSpec& timeout = INFINITETIME);

		FACTORGROUP_DIMENSION getVelocityDimensionIndex(const ssr::TimeSpec& timeout = INFINITETIME);
		FACTORGROUP_NOTATION getVelocityNotationIndex(const ssr::TimeSpec& timeout = INFINITETIME);
	};



	/**
	 * EPOSFactory class.
	 */
	class EPOSFactory : public canopen::INodeFactory {
	public:
		EPOSFactory() {}
		virtual ~EPOSFactory() {}

	public:
		canopen::Node* createNode(canopen::CANopen* pCANopen, const uint8_t nodeId) {
			return new EPOS(pCANopen, nodeId);
		}

		uint32_t getVendorId() const {
			return VENDOR_ID_MAXON;
		}

		uint32_t getProductCode() const {
			return PRODUCT_CODE_EPOS2;
		}

		uint32_t getProductCodeMask() const {
			return PRODUCT_CODE_MASK;
		}
	};
}
