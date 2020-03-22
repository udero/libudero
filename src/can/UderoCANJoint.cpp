
#include "udero/UderoLogger.h"
#include "UderoCANJoint.h"
using namespace technotools;

UderoCANJoint::UderoCANJoint(const uint32_t& id, canopen::CANopen* pCANopen, double gearRatio, double EncoderRes, const UderoREAL min, const UderoREAL max):
  m_CANID(id), m_gearRatio(gearRatio), m_encoderRes(EncoderRes),
  m_MinAngle(min), m_MaxAngle(max)
  
{
  UINFO("UderoCANJoint::UderoCANJoint(%d, gear=%f, enc=%f, min=%f, max=%f)", id,
	gearRatio, EncoderRes, min, max);
  UVERBOSE(" - Creating EPOS Node Object.");
  try {
  m_pEPOS = static_cast<maxon::EPOS*>(pCANopen->initNode(id));

  m_polar = 1;
  accelBuffer = 0;
  velocityBuffer = 0;

  m_pEPOS->start();

  for (int i = 0; i < NUM_HOMING_PARAM; i++) {
    homing_param[i] = default_homing_param[m_CANID - 1][i];
  }
  } catch (std::exception& ex) {
    std::cerr << "Exception when UderoCANJoint::UderoCANJoint()" << std::endl;
    std::cerr << ex.what() << std::endl;
    throw ex;
  }
}


UderoCANJoint::~UderoCANJoint() {}

void UderoCANJoint::setHomeOffset(const int32_t offsetCount) {
  UTRACE("UderoCANJoint::setHomeOffset(%f)", offsetCount);
  homing_param[1] = offsetCount;
}

void UderoCANJoint::setupPDO() {
  m_pEPOS->enterPreOperational();
  setupTXPDO1();
  setupRXPDO4();
  setupRXPDO2();
  setupRXPDO1();
  m_pEPOS->start();
}

void UderoCANJoint::setupTXPDO1(const ssr::TimeSpec& timeout/* = INFINITETIME*/) {
  m_pEPOS->setupTXPDOParameter<PDO1>(true, false);
  m_pEPOS->setupTXPDOTransmissionType<PDO1>(false, false); // asynchronous, no RTR                                                                                                                                               
  m_pEPOS->setupTXPDOTransmissionInhibitTime<PDO1>(20*1000); // 20[ms]                                                                                                                                                         
  m_pEPOS->setupTXPDOMapping<PDO1>(OD_PDOMAPPINGOBJECT_POSITIONACTUALVALUE, OD_PDOMAPPINGOBJECT_STATUSWORD);
}

void UderoCANJoint::setupRXPDO2(const ssr::TimeSpec& timeout/* = INFINITETIME */) {
  m_pEPOS->setupRXPDOParameter<PDO2>(true, false);
  m_pEPOS->setupRXPDOTransmissionType<PDO2>(false, timeout); // asynchronous, no RTR                                                                                                                                             
  m_pEPOS->setupRXPDOMapping<PDO2>(OD_PDOMAPPINGOBJECT_PROFILEVELOCITY);
}

void UderoCANJoint::setupRXPDO1(const ssr::TimeSpec& timeout/* = INFINITETIME*/) {
  m_pEPOS->setupRXPDOParameter<PDO1>(true, false);
  m_pEPOS->setupRXPDOTransmissionType<PDO1>(false, timeout); // asynchronous, no RTR                                                                                                                                             
  m_pEPOS->setupRXPDOMapping<PDO1>(OD_PDOMAPPINGOBJECT_TARGETPOSITION, OD_PDOMAPPINGOBJECT_CONTROLWORD);
}

void UderoCANJoint::setupRXPDO4(const ssr::TimeSpec& timeout/* = INFINITETIME*/) {
  m_pEPOS->setupRXPDOParameter<PDO4>(true, false, timeout);
  m_pEPOS->setupRXPDOTransmissionType<PDO4>(false, timeout); // asynchronous, no RTR                                                                                                                                             
  m_pEPOS->setupRXPDOMapping<PDO4>(OD_PDOMAPPINGOBJECT_PROFILEACCELERATION, OD_PDOMAPPINGOBJECT_PROFILEDECELERATION, timeout);
}

void UderoCANJoint::reset() {
  UTRACE("UderoCANJoint(%d)::reset()", m_CANID);
  m_pEPOS->reset();
}

void UderoCANJoint::init() {
	UTRACE("UderoCANJoint(%d)::init()", m_CANID);
	ssr::TimeSpec sleeptime(0, 10 * 1000);
	ssr::TimeSpec timeout(0, 500 * 1000);

	//m_pEPOS->reset();

	EPOS_STATE state = m_pEPOS->getState(timeout);

	if (state == STATE_FAULT || state == STATE_FAULTREACTIONACTIVE_DISABLED || state == STATE_FAULTREACTIONACTIVE_ENABLED) {
		m_pEPOS->faultReset(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}
	else if (state == STATE_QUICKSTOPACTIVE) {
		m_pEPOS->disableVoltage(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}
	else if (state != STATE_SWITCHONDISABLE) {
		m_pEPOS->disableVoltage(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}

	state = m_pEPOS->getState(timeout);
	if (state == STATE_SWITCHONDISABLE) {
		m_pEPOS->shutDown(timeout);
		m_pEPOS->waitStateChanged(STATE_READYTOSWITCHON, 10, sleeptime, timeout);
	}

	state = m_pEPOS->getState(timeout);
	if (state == STATE_READYTOSWITCHON) {
		m_pEPOS->switchOn(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHON, 10, sleeptime, timeout);
	}
	else {
		EPOS_STATE state = m_pEPOS->getState(timeout);
	}


#if USE_PDO
	setupPDO();
#endif

}

UderoREAL UderoCANJoint::getPosition(bool PDO) {
  UVERBOSE("UderoCANJoint(%d)::getPosition(PDO=%d)", m_CANID, PDO);
#if USE_PDO
  if (PDO) {
    ssr::TimeSpec timeout(0, 100 * 1000);
    int32_t pos;
    uint16_t stat;
    if (m_pEPOS->tryReadByPDO<PDO1, int32_t, uint16_t>(&pos, &stat)) {
      m_jointPosBuffer = m_polar * inc_to_pos(pos);
    }
    return m_jointPosBuffer;
  } else {
    ssr::TimeSpec timeout(0, 100 * 1000);
    m_jointPosBuffer = m_polar * inc_to_pos(m_pEPOS->getActualPosition(timeout));
    return m_jointPosBuffer;
  }
#else
  ssr::TimeSpec timeout(0, 100 * 1000);
  return m_polar * inc_to_pos(m_pEPOS->getActualPosition(timeout));
#endif
}

void UderoCANJoint::setHomingMode(const HomingMode mode) {
  if (mode == ABSOLUTE_POSITION) {
    m_pEPOS->setHomingMethod(HOMINGMETHOD_ACTUALPOSITION);
  }
}


double UderoCANJoint::getHomingSpeed() {
	return inc_to_pos(m_pEPOS->getHomingSpeed());
}

void UderoCANJoint::goHome() {
  UDEBUG("UderoCANJoint(%d)::goHome()", this->m_pEPOS->getNodeId());
  ssr::TimeSpec sleeptime(0, 10 * 1000);
  ssr::TimeSpec timeout(0, 400 * 1000);
  bool homing_setting = false;
  for (int i = 0; i < 5; i++) {
    try {
      UTRACE(" - setting OperationMode to HOMING_MODE");
      m_pEPOS->setOperationMode(MODESOFOPERATION_HOMINGMODE);

      UTRACE(" - enabling Operation");
      m_pEPOS->enableOperation(timeout);

      UTRACE(" - waiting State Changed to STATE_OPERATIONABLE");
      m_pEPOS->waitStateChanged(STATE_OPERATIONENABLE, 10, sleeptime, timeout);

      if (homing_setting) {
	UTRACE(" - setting Homing Method to %d", homing_param[0]);
	m_pEPOS->setHomingMethod((HOMINGMETHOD)homing_param[0]);
	
	UTRACE(" - setting Homing Offset to %d", homing_param[1]);
	m_pEPOS->setHomeOffset(homing_param[1]);
	
	UTRACE(" - setting Homing Speed to %d", homing_param[3]);
	m_pEPOS->setHomingSpeed(homing_param[2], homing_param[3]);
	
	UTRACE(" - setting Homing Accel to %d", homing_param[4]);
	m_pEPOS->setHomingAcceleration(homing_param[4]);
	
	UTRACE(" - setting Homing Position to %d", homing_param[6]);
	m_pEPOS->setHomePosition(homing_param[6]);
      } 
      UTRACE(" - start Homing");
      m_pEPOS->homing(false);
      break;
    }
    catch (std::exception& ex) {
      UERROR("Exception in UderoCANJoint::goHome(nodeID=%d):%s", m_pEPOS->getNodeId(), ex.what());
      try {
	UWARN("Error recovery. UderoCANJoint::init calling...");
	init();
      } catch (std::exception& ex) {
	UERROR("Exception in UderoCANJoint(%d)::init (in recovery behaviour in goHome method.", m_CANID);
	throw ex;
      }
    }
  }

  UDEBUG("UderoCANJoint(%d)::goHome() ends", this->m_pEPOS->getNodeId());
}

void UderoCANJoint::waitHoming() {
  UDEBUG("UderoCANJoint(%d)::waitHoming()", this->m_pEPOS->getNodeId());
  try {
    ssr::TimeSpec timeout(30, 0);
 //   m_pEPOS->waitHomingAttained(timeout);
    m_pEPOS->waitTargetReached(timeout);
  } catch (std::exception& ex) {
    UERROR("UderoCANJoint(%d)::waitHoming():Exception: %s", m_CANID, ex.what());
    throw ex;
  }
  UDEBUG("UderoCANJoint(%d)::waitHoming() ends", this->m_pEPOS->getNodeId());
}

bool UderoCANJoint::isHomed() {
  ssr::TimeSpec timeout(0, 30*1000);
  return m_pEPOS->isHomed(timeout);
}

void UderoCANJoint::waitStopped(int32_t timeoutMillis) {
  UTRACE("UderoCANJoint(%d)::waitStopped(timeout=%d [ms])", m_CANID, timeoutMillis);
  try {
    int32_t sec = timeoutMillis / 1000;
    int32_t usec = (timeoutMillis - sec*1000) * 1000;
    ssr::TimeSpec timeout(sec, usec);
    m_pEPOS->waitTargetReached(timeout);
  } catch (std::exception& ex) {
    UERROR("UderoCANJoint(%d)::waitStopped() Exception: %s", m_CANID, ex.what());
    throw ex;
  }
}

void UderoCANJoint::quickStop() {
  UTRACE("UderoCANJoint(%d)::quickStop()", m_CANID);
  ssr::TimeSpec timeout(1, 100*1000);
  m_pEPOS->quickStop(timeout);
}

void UderoCANJoint::enableOperation() {
  ssr::TimeSpec timeout(0, 50*1000);
  m_pEPOS->enableOperation(timeout);
}

void UderoCANJoint::setMode(const UderoJointMode& mode) {
  UTRACE("UderoCANJoint(%d)::setMode(%d)", m_CANID, mode);
  int err_count = 0;
  while(1) {
    
  try {
    ssr::TimeSpec sleeptime(0, 50 * 1000);
    ssr::TimeSpec timeout(7, 500 * 1000);
    /*
	EPOS_STATE state = m_pEPOS->getState(timeout);

	if (state == STATE_FAULT || state == STATE_FAULTREACTIONACTIVE_DISABLED || state == STATE_FAULTREACTIONACTIVE_ENABLED) {
		m_pEPOS->faultReset(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}
	else if (state == STATE_QUICKSTOPACTIVE) {
		m_pEPOS->disableVoltage(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}
	else if (state != STATE_SWITCHONDISABLE) {
		m_pEPOS->disableVoltage(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHONDISABLE, 10, sleeptime, timeout);
	}

	state = m_pEPOS->getState(timeout);
	if (state == STATE_SWITCHONDISABLE) {
		m_pEPOS->shutDown(timeout);
		m_pEPOS->waitStateChanged(STATE_READYTOSWITCHON, 10, sleeptime, timeout);
	}

	state = m_pEPOS->getState(timeout);
	if (state == STATE_READYTOSWITCHON) {
		m_pEPOS->switchOn(timeout);
		m_pEPOS->waitStateChanged(STATE_SWITCHON, 10, sleeptime, timeout);
	}
    */


    
    if (mode == MODE_POSITION) {
      m_pEPOS->setOperationMode(MODESOFOPERATION_PROFILEPOSITIONMODE, timeout);
      m_pEPOS->enableOperation(timeout);
      UVERBOSE(" - waitStateChange in 50 times");
      m_pEPOS->waitStateChanged(STATE_OPERATIONENABLE, 50, sleeptime, timeout);
    }
    else if (mode == MODE_VELOCITY) {
      m_pEPOS->setOperationMode(MODESOFOPERATION_PROFILEVELOCITYMODE, timeout);
      m_pEPOS->enableOperation(timeout);
      UVERBOSE(" - waitStateChange in 50 times");
      m_pEPOS->waitStateChanged(STATE_OPERATIONENABLE, 50, sleeptime, timeout);
    }
    else if (mode == MODE_INACTIVE) {
      m_pEPOS->disableOperation(timeout);
      UVERBOSE(" - waitStateChange in 50 times");
      m_pEPOS->waitStateChanged(STATE_SWITCHON, 50, sleeptime, timeout);
    }
    
    return;
  } catch (std::exception& ex) {
    UERROR("UderoCANJoint(%d)::setMode():Exception:%s", m_CANID, ex.what());
    if (err_count < 5) {
      err_count ++;
      init();
    } else {
      throw ex;
    }
  }
  }
}

void UderoCANJoint::setAcceleration(const UderoREAL accel) {
  ssr::TimeSpec timeout(0, 300 * 1000);
#if USE_PDO
  if (accelBuffer != accel) {
    m_pEPOS->sendByPDO<PDO4, uint32_t, uint32_t>(radPerSec_to_rpm(accel), radPerSec_to_rpm(accel));
    accelBuffer = accel;
  }
#else
  m_pEPOS->setProfileAcceleration(radPerSec_to_rpm(accel), timeout);
  m_pEPOS->setProfileDeceleration(radPerSec_to_rpm(accel), timeout);
#endif
}


void UderoCANJoint::movePositionWithVelocity(const UderoREAL targetPosition_, const UderoREAL targetVelocity, const bool minMaxOff) {


  UderoREAL targetPosition = targetPosition_ > m_MaxAngle ? m_MaxAngle : (targetPosition_ < m_MinAngle ? m_MinAngle : targetPosition_);
  if (minMaxOff) targetPosition = targetPosition_;

  ssr::TimeSpec timeout(0, 300 * 1000);
#if USE_PDO
  if (velocityBuffer != targetVelocity) {
    m_pEPOS->sendByPDO<PDO2, uint32_t>(radPerSec_to_rpm(targetVelocity));
    velocityBuffer = targetVelocity;
  }
  uint16_t cw = CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP
    | CONTROLWORD_ENABLEOPERATION | CONTROLWORD_NEWSETPOINT | CONTROLWORD_CHANGESETIMMEDIATELY;
  m_pEPOS->sendByPDO<PDO1, int32_t, uint16_t>(m_polar * pos_to_inc(targetPosition), cw);
#else
  //m_pEPOS->setProfileVelocity(radPerSec_to_incPerMilliSec(targetVelocity), timeout);
  m_pEPOS->setProfileVelocity(radPerSec_to_rpm(targetVelocity), timeout);
  m_pEPOS->setTargetPosition(m_polar * pos_to_inc(targetPosition), timeout);
  m_pEPOS->setControlWord(CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP 
			  | CONTROLWORD_ENABLEOPERATION | CONTROLWORD_NEWSETPOINT | CONTROLWORD_CHANGESETIMMEDIATELY, timeout);
#endif
}

void UderoCANJoint::moveVelocity(const UderoREAL targetVelocity) {
	ssr::TimeSpec timeout(0, 300 * 1000);
	m_pEPOS->setTargetVelocity(radPerSec_to_rpm(targetVelocity));
	m_pEPOS->setControlWord(CONTROLWORD_SWITCHON | CONTROLWORD_ENABLEVOLTAGE | CONTROLWORD_QUICKSTOP
		| CONTROLWORD_ENABLEOPERATION | CONTROLWORD_NEWSETPOINT | CONTROLWORD_CHANGESETIMMEDIATELY, timeout);
}

UderoREAL UderoCANJoint::getActualVelocity() {
	UVERBOSE("UderoCANJoint(%d)::getActualVelocity()", m_CANID);
	ssr::TimeSpec timeout(0, 100 * 1000);
	return m_polar * rpm_to_radPerSec(m_pEPOS->getActualVelocity(timeout));
}
 
uint16_t UderoCANJoint::getDigitalInput() {
	ssr::TimeSpec timeout(0, 300 * 1000);
	return m_pEPOS->getDigitalInput(timeout);
}


JOINT_STATE UderoCANJoint::getJointState() {
  ssr::TimeSpec timeout(0, 100 * 1000);
  EPOS_STATE state = m_pEPOS->getState(timeout);
  if (state == STATE_SWITCHON) {
    return DISABLED;
  } else if (state == STATE_OPERATIONENABLE) {
    return ENABLED;
  } else if (state == STATE_QUICKSTOPACTIVE) {
    return ENABLED;
  } else //if (state == STATE_FAULT || state == STATE_FAULTREACTIONACTIVE_DISABLED || state == STATE_FAULTREACTIONACTIVE_ENABLED) {
    {
    return FAULT;
  } 
}
