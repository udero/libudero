/**
 * @file EPOS.cpp
 */

#include "EPOS.h"
#include <sstream>
#include <iostream>
using namespace canopen;
using namespace ssr;
using namespace maxon;


std::string EPOS_stateToString(const EPOS_STATE& state) {
	switch (state) {
	case STATE_START:
		return "START_STATE";
	case STATE_NOTREADYTOSWITCHON:
		return "STATE_NOTREADYTOSWITCHON";
	case STATE_SWITCHONDISABLE:
		return "STATE_SWITCHONDISABLE";
	case STATE_READYTOSWITCHON:
		return "STATE_READYTOSWITCHON";
	case STATE_SWITCHON:
		return "STATE_SWITCHON";
	case STATE_REFRESH:
		return "STATE_REFRESH";
	case STATE_MEASUREINIT:
		return "STATE_MEASUREINIT";
	case STATE_OPERATIONENABLE:
		return "STATE_OPERATIONENABLE";
	case STATE_QUICKSTOPACTIVE:
		return "STATE_QUICKSTOPACTIVE";
	case STATE_FAULTREACTIONACTIVE_DISABLED:
		return "STATE_FAULTREACTIONACTIVE_DISABLED";
	case STATE_FAULTREACTIONACTIVE_ENABLED:
		return "STATE_FAULTREACTIONACTIVE_ENABLED";
	case STATE_FAULT:
		return "STATE_FAULT";
	default:
	{
		std::ostringstream ss;
		ss << "Unknown State : " << state;
		return ss.str();
	}
	}
	std::ostringstream ss;
	ss << "Unknown State : " << state;
	return ss.str();
}



EPOS::EPOS(CANopen* pCANopen, const uint8_t nodeId) :
Node(pCANopen, nodeId), m_ErrorCode(EPOS_ERROR_NOERROR)
{
}

EPOS::~EPOS(void)
{
}


std::string EPOS::getStateStrFromState(const EPOS_STATE& state) {
	return EPOS_stateToString(state);
}

MODESOFOPERATION EPOS::getOperationMode(void)
{
	return (MODESOFOPERATION)m_SDO.read<int8_t>(OD_INDEX_MODESOFOPERATIONDISPLAY, OD_SUBINDEX_MODESOFOPERATIONDISPLAY);
}

void EPOS::setOperationMode(MODESOFOPERATION mode, const ssr::TimeSpec& timeout)
{
	m_SDO.write<int8_t>(OD_INDEX_MODESOFOPERATION, OD_SUBINDEX_MODESOFOPERATION, mode, timeout);
}

void EPOS::setTargetPosition(const int32_t position, const ssr::TimeSpec& timeout) {
	m_SDO.write<int32_t>(OD_INDEX_TARGETPOSITION, OD_SUBINDEX_TARGETPOSITION, position, timeout);
}

void EPOS::setTargetVelocity(const int32_t incPerMilliSec, const ssr::TimeSpec& timeout) {
	m_SDO.write<int32_t>(OD_INDEX_TARGETVELOCITY, OD_SUBINDEX_TARGETVELOCITY, incPerMilliSec, timeout);
}

void EPOS::setProfileVelocity(const uint32_t rpm, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint32_t>(OD_INDEX_PROFILEVELOCITY, OD_SUBINDEX_PROFILEVELOCITY, rpm, timeout);
}

void EPOS::setControlWord(const uint16_t controlWord, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD, controlWord, timeout);
}

EPOS_STATE EPOS::getState(const ssr::TimeSpec& timeout) {
	return (EPOS_STATE)(getStatusWord(timeout) & EPOS_STATEMASK);
}

bool EPOS::isHomed(const ssr::TimeSpec& timeout) {
	
	uint16_t statusWord = getStatusWord(timeout);
	if (statusWord & STATUSWORD_POSITIONREFERENCEDTOHOMEPOSITION) {
		return true;
	}
	return false;
}

EPOS_STATE EPOS::getStateFromStatusWord(const uint16_t statusWord) {
	return (EPOS_STATE)(statusWord & EPOS_STATEMASK);
}

/**
	* @brief Get Current Velocity
	*/
int32_t EPOS::getActualVelocity(const ssr::TimeSpec& timeout)
{
	return m_SDO.read<int32_t>(OD_INDEX_VELOCITYACTUALVALUE, OD_SUBINDEX_VELOCITYACTUALVALUE, timeout);
}

void EPOS::faultReset(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_QUICKSTOP | CONTROLWORD_ENABLEVOLTAGE, timeout);
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_QUICKSTOP | CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_FAULTRESET, timeout);
	clearErrorCode();
}

void EPOS::halt(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEOPERATION | CONTROLWORD_HALT, timeout);
	clearErrorCode();
}


void EPOS::shutDown(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD, 
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP,
		timeout);
}

void EPOS::switchOn(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD, 
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON, timeout);
}

void EPOS::switchOff(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_HALT, timeout);
}

void EPOS::switchOnAndEnableOperation(const ssr::TimeSpec& timeOut)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD, 
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEOPERATION | CONTROLWORD_HALT, timeOut);
}

void EPOS::quickStop(const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEOPERATION , timeout);
}

void EPOS::disableVoltage(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD, 
		CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEOPERATION, timeout);
}

uint16_t EPOS::getStatusWord(const ssr::TimeSpec& timeout)
{
	return m_SDO.read<uint16_t>(OD_INDEX_STATUSWORD, OD_SUBINDEX_STATUSWORD, timeout);
}

void EPOS::enableOperation(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON |
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_ENABLEOPERATION, timeout);
}

void EPOS::disableOperation(const ssr::TimeSpec& timeout)
{
	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON |
		CONTROLWORD_ENABLEVOLTAGE , timeout);
}


void EPOS::waitStateChanged(const EPOS_STATE targetState, const uint32_t try_count, const ssr::TimeSpec& sleepTime, const ssr::TimeSpec& timeout)
{
	EPOS_STATE s;
	for (uint32_t i = 0; i < try_count; i++) {
		ssr::Thread::Sleep(sleepTime.sec * 1000 + sleepTime.usec / 1000);
		s = getState(timeout);
		if (targetState == s) {
			return;
		}
	}
	std::stringstream ss;
	ss << "maxon::EPOS(nodeId=" << (uint32_t)getNodeId() <<")::waitStateChanged Timeout. Waiting (now:" << getStateStrFromState(s) << " -> " << getStateStrFromState(targetState);
	throw canopen::TimeOutException(ss.str().c_str());
}


void EPOS::homing(const bool block /* = true */)
{
  //	MODESOFOPERATION opmode = getOperationMode();
	setOperationMode(MODESOFOPERATION_HOMINGMODE);

	m_SDO.write<uint16_t>(OD_INDEX_CONTROLWORD, OD_SUBINDEX_CONTROLWORD,
		CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP | CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEOPERATION | CONTROLWORD_HOMINGOPERATIONSTART);
	while(block) {
		uint16_t statusWord = getStatusWord();
		if(statusWord & STATUSWORD_HOMINGERROR) {
			throw HomingException();
		}

		if( (statusWord & STATUSWORD_HOMINGATTAINED) && (statusWord & STATUSWORD_TARGETREACHED) ) {
			break;
		}
	}
}

void EPOS::setHomingMethod(const HOMINGMETHOD homingMethod, const ssr::TimeSpec& timeout) {
	m_SDO.write<int8_t>(OD_INDEX_HOMINGMETHOD, OD_SUBINDEX_HOMINGMETHOD, homingMethod, timeout);
}

void EPOS::setHomingSpeed(const uint32_t speedSwitchSearch, const uint32_t speedZeroSearch, const ssr::TimeSpec& timeout /* = INFINITETIME */)
{
	m_SDO.write<uint32_t>(OD_INDEX_HOMINGSPEEDFORSWITCHSEARCH, OD_SUBINDEX_HOMINGSPEEDFORSWITCHSEARCH, speedSwitchSearch, timeout);
	m_SDO.write<uint32_t>(OD_INDEX_HOMINGSPEEDFORZEROSEARCH, OD_SUBINDEX_HOMINGSPEEDFORZEROSEARCH, speedZeroSearch, timeout);
}

uint32_t EPOS::getHomingSpeed(const ssr::TimeSpec& timeout /* = INFINITETIME */)
{
	return m_SDO.read<uint32_t>(OD_INDEX_HOMINGSPEEDFORSWITCHSEARCH, OD_SUBINDEX_HOMINGSPEEDFORSWITCHSEARCH, timeout);
}


void EPOS::setHomingAcceleration(const uint32_t accel, const ssr::TimeSpec& timeout /* = INFINITETIME */)
{
	m_SDO.write<uint32_t>(OD_INDEX_HOMINGACCELERATION, OD_SUBINDEX_HOMINGACCELERATION, accel, timeout);
}

void EPOS::setCurrentThoresholdForHomingMode(const uint16_t milliAmp, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_CURRENTTHRESHOLDFORHOMINGMODE, OD_SUBINDEX_CURRENTTHRESHOLDFORHOMINGMODE, milliAmp, timeout);
}

void EPOS::setHomePosition(const int32_t homePosition, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint32_t>(OD_INDEX_HOMEPOSITION, OD_SUBINDEX_HOMEPOSITION, homePosition, timeout);
}

void EPOS::setHomeOffset(const int32_t homeOffset, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint32_t>(OD_INDEX_HOMEOFFSET, OD_SUBINDEX_HOMEOFFSET, homeOffset, timeout);
}

void EPOS::waitTargetReached(const TimeSpec timeOut)
{
	TimeSpec currentTime;
	m_Timer.tick();
	while(true) {
		if(timeOut != ZEROTIME) {
			m_Timer.tack(&currentTime);
			if(currentTime > timeOut) {
				throw TimeOutException("EPOS::waitTargetReached");
			}
		}
		TimeSpec statusWordTimeOut(0, 100 * 1000);
		uint16_t statusWord = getStatusWord(statusWordTimeOut);
		if(statusWord & STATUSWORD_TARGETREACHED) {
			return;
		}
		if(statusWord & STATUSWORD_FAULT) {
			throw FaultException();
		}
	}
}

void EPOS::waitHomingAttained(const TimeSpec timeOut)
{
	TimeSpec currentTime;
	m_Timer.tick();
	while(true) {
		if(timeOut != ZEROTIME) {
			m_Timer.tack(&currentTime);
			if(currentTime > timeOut) {
				throw TimeOutException("EPOS::waitHomingAttained");
			}
		}
		uint16_t statusWord = getStatusWord(timeOut);
		if(statusWord & STATUSWORD_HOMINGATTAINED) {
			return;
		}

		if(statusWord & STATUSWORD_FAULT) {
			throw FaultException();
		}
	}
}

void EPOS::onEmergency(const CANMSG& msg) throw ()
{
	this->m_ErrorCode = ((uint16_t)msg.data[0] << 8) & ((uint16_t)msg.data[1]);
}

uint32_t EPOS::getMaximumProfileVelocity(void)
{
	return m_SDO.read<uint32_t>(OD_INDEX_MAXIMUMPROFILEVELOCITY, OD_SUBINDEX_MAXIMUMPROFILEVELOCITY);
}

void EPOS::setMaximumProfileVelocity(const uint32_t rpm)
{
	return m_SDO.write<uint32_t>(OD_INDEX_MAXIMUMPROFILEVELOCITY, OD_SUBINDEX_MAXIMUMPROFILEVELOCITY, rpm);
}

int32_t EPOS::getMinimumPositionLimit(void)
{
	return m_SDO.read<int32_t>(OD_INDEX_MINIMUMPOSITIONLIMIT, OD_SUBINDEX_MINIMUMPOSITIONLIMIT);
}

int32_t EPOS::getMaximumPositionLimit(void)
{
	return m_SDO.read<int32_t>(OD_INDEX_MAXIMUMPOSITIONLIMIT, OD_SUBINDEX_MAXIMUMPOSITIONLIMIT);
}

void EPOS::setMinimumPositionLimit(const int32_t position)
{
	m_SDO.write<int32_t>(OD_INDEX_MINIMUMPOSITIONLIMIT, OD_SUBINDEX_MINIMUMPOSITIONLIMIT,
		position);
}

void EPOS::setMaximumPositionLimit(const int32_t position)
{
	m_SDO.write<int32_t>(OD_INDEX_MAXIMUMPOSITIONLIMIT, OD_SUBINDEX_MAXIMUMPOSITIONLIMIT,
		position);
}

void EPOS::checkNodeErrorState(void)
{
	Node::checkNodeErrorState();

	if(m_ErrorCode != EPOS_ERROR_NOERROR) { 
		std::string errorStr;
		switch(m_ErrorCode) {
		case EPOS_ERROR_GENERIC:
			errorStr = "Generic Error"; break;
		case EPOS_ERROR_OVERCURRENT:
			errorStr = "Over Current"; break;
		case EPOS_ERROR_OVERVOLTAGE:
			errorStr = "Over Voltage"; break;
		case EPOS_ERROR_UNDERVOLTAGE:
			errorStr = "Under Voltage"; break;
		case EPOS_ERROR_OVERTEMPERATURE:
			errorStr = "Over Temperature"; break;
		case EPOS_ERROR_LOGICSUPPLYVOLTAGETOOLOW:
			errorStr = "Logic Supply Voltage Low"; break;
		case EPOS_ERROR_SUPPLYVOLTAGEOUTPUTSTAGETOOLOW:
			errorStr = "Supply Voltage Output State Low"; break;
		case EPOS_ERROR_INTERNALSOFTWARE:
			errorStr = "Internal Software"; break;
		case EPOS_ERROR_SOFTWAREPARAMETER:
			errorStr = "Software Parameter"; break;
		case EPOS_ERROR_POSITIONSENSOR:
			errorStr = "Position Sensor"; break;
		case EPOS_ERROR_CANOVERRRUNOBJECTLOST:
			errorStr = "CAN Overrun Object Lost"; break;
		case EPOS_ERROR_CANOVERRUN:
			errorStr = "CAN Overrun"; break;
		case EPOS_ERROR_CANPASSIVEMODE:
			errorStr = "CAN Passive Mode"; break;
		case EPOS_ERROR_CANLIFEGUARD:
			errorStr = "CAN Life Guard"; break;
		case EPOS_ERROR_CANTRANSMITCOBIDCOLLISION:
			errorStr = "CAN Transmit COB-ID Collision"; break;
		case EPOS_ERROR_CANBUSOFF:
			errorStr = "CAN bus off"; break;
		case EPOS_ERROR_CANRXQUEUEOVERFLOW:
			errorStr = "CAN Rx Queue Over flow"; break;
		case EPOS_ERROR_CANTXQUEUEOVERFLOW:
			errorStr = "CAN Tx Queue Over flow"; break;
		case EPOS_ERROR_CANPDOLENGTH:
			errorStr = "CAN PDO length"; break;
		case EPOS_ERROR_FOLLOWING:
			errorStr = "Following"; break;
		case EPOS_ERROR_HALLSENSOR:
			errorStr = "Hall Sensor"; break;
		case EPOS_ERROR_INDEXPROCESSING:
			errorStr = "Index processing"; break;
		case EPOS_ERROR_ENCODERRESOLUTION:
			errorStr = "Encoder resolution"; break;
		case EPOS_ERROR_HALLSENSORNOTFOUND:
			errorStr = "Hall Sensor Not Found"; break;
		case EPOS_ERROR_NEGATIVELIMITSWITCH:
			errorStr = "Negative Limit Switch"; break;
		case EPOS_ERROR_POSITIVELIMITSWITCH:
			errorStr = "Positive Limit Switch"; break;
		case EPOS_ERROR_HALLANGLEDETECTION:
			errorStr = "Hall Angle Detection"; break;
		case EPOS_ERROR_SOFTWAREPOSITIONLIMIT:
			errorStr = "Software Position Limit"; break;
		case EPOS_ERROR_POSITIONSENSORBREACH:
			errorStr = "Position Sensor Breach"; break;
		case EPOS_ERROR_SYSTEMOVERLOADED:
			errorStr = "System mover Loaded"; break;
		case EPOS_ERROR_INTERPOLATEDPOSITIONMODE:
			errorStr = "Interpolated Position Mode"; break;
		case EPOS_ERROR_AUTOTUNINGINDENTIFICATION:
			errorStr = "Auto Tuning Indentification"; break;
		case EPOS_ERROR_GEARSCALINGFACTOR:
			errorStr = "Gear scaling factor"; break;
		case EPOS_ERROR_CONTROLLERGAIN:
			errorStr = "Controller Gain"; break;
		case EPOS_ERROR_MAINSENSORDIRECTION:
			errorStr = "Main Sensor Direction"; break;
		case EPOS_ERROR_AUXILIARYSENSORDIRECTION:
			errorStr = "Auxiliary Sensor Direction"; break;
		default:
			errorStr = "Unknown Emergency Error"; break;
		}
		throw EPOSException(errorStr, m_ErrorCode);
	}
}


void EPOS::setAnalogCurrentSetpointScaling(const int16_t scaling, const ssr::TimeSpec& timeout)
{
	m_SDO.write<int16_t>(OD_INDEX_ANALOGCURRENTSETPOINTSCALING,
		OD_SUBINDEX_ANALOGCURRENTSETPOINTSCALING,
		scaling, timeout);
}

void EPOS::setTargetCurrent(const int16_t milliAmp, const ssr::TimeSpec& timeout)
{
	m_SDO.write<int16_t>(OD_INDEX_CURRENTMODESETTINGVALUE,
		OD_SUBINDEX_CURRENTMODESETTINGVALUE,
		milliAmp, timeout);
}





int16_t EPOS::getActualCurrent(const ssr::TimeSpec& timeout)
{
	return m_SDO.read<int16_t>(OD_INDEX_CURRENTDEMANDGVALUE,
		OD_SUBINDEX_CURRENTDEMANDVALUE, timeout);
}

void EPOS::setProfileAcceleration(const uint32_t accel, const ssr::TimeSpec& timeout) {
	return m_SDO.write<uint32_t>(OD_INDEX_PROFILEACCELERATION,
		OD_SUBINDEX_PROFILEACCELERATION,
		accel, timeout);
}

void EPOS::setProfileDeceleration(const uint32_t accel, const ssr::TimeSpec& timeout) {
	return m_SDO.write<uint32_t>(OD_INDEX_PROFILEDECELERATION,
		OD_SUBINDEX_PROFILEDECELERATION,
		accel, timeout);
}

int32_t EPOS::getActualPosition(const ssr::TimeSpec& timeout)
{
	
	int32_t i = m_SDO.read<int32_t>(OD_INDEX_POSITIONACTUALVALUE,
		OD_SUBINDEX_POSITIONACTUALVALUE, timeout);
	/*
	if (this->getNodeId() == 3) {
		std::cout << "EPOS::getActualPosition(for node = " << getNodeId() << ") = " << i << std::endl;
	}
	*/

	return i;
}

void EPOS::setDigitalOutputMask(const uint16_t mask, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE, OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESMASK, mask, timeout);
}


void EPOS::setDigitalOutputPolarity(const uint16_t polarity, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE, OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESPOLARITY, polarity, timeout);
}

void  EPOS::setDigitalOutput(const uint16_t status, const ssr::TimeSpec& timeout) {
	m_SDO.write<uint16_t>(OD_INDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE, OD_SUBINDEX_DIGITALOUTPUTFUNCTIONARITIESSTATE, status, timeout);
}

uint16_t EPOS::getDigitalInput(const ssr::TimeSpec& timeout) {
	return 	m_SDO.read<uint16_t>(OD_INDEX_DIGITALINPUTFUNCTIONARITIES, OD_SUBINDEX_DIGITALINPUTFUNCTIONARITIES, timeout);
}

int16_t EPOS::getAnalogInput1(const ssr::TimeSpec& timeout) {
	return m_SDO.read<int16_t>(OD_INDEX_ANALOGINPUT1, OD_SUBINDEX_ANALOGINPUT1, timeout);
}

int16_t EPOS::getAnalogInput2(const ssr::TimeSpec& timeout) {
	return m_SDO.read<int16_t>(OD_INDEX_ANALOGINPUT2, OD_SUBINDEX_ANALOGINPUT2, timeout);
}


void EPOS::setMaximalFollowingError(const uint32_t error, const ssr::TimeSpec& timeout) {

	m_SDO.write<uint32_t>(OD_INDEX_MAXIMALFOLLOWINGERROR, OD_SUBINDEX_MAXIMALFOLLOWINGERROR, error, timeout);
}


FACTORGROUP_DIMENSION EPOS::getVelocityDimensionIndex(const ssr::TimeSpec& timeout) {
	return (FACTORGROUP_DIMENSION)m_SDO.read<uint8_t>(OD_INDEX_VELOCITYDIMENSIONINDEX, OD_SUBINDEX_VELOCITYDIMENSIONINDEX, timeout);
}

FACTORGROUP_NOTATION EPOS::getVelocityNotationIndex(const ssr::TimeSpec& timeout) {
	return (FACTORGROUP_NOTATION)m_SDO.read<int8_t>(OD_INDEX_VELOCITYNOTATIONINDEX, OD_SUBINDEX_VELOCITYNOTATIONINDEX, timeout);
}