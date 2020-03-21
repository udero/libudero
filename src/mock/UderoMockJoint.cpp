
#include <iostream>

#include "udero/UderoLogger.h"
#include "Thread.h"
#include "UderoMockJoint.h"
using namespace reharo;

UderoMockJoint::UderoMockJoint(const uint32_t& id, double gearRatio, double EncoderRes, const UderoREAL min, const UderoREAL max):
  m_CANID(id), m_gearRatio(gearRatio), m_encoderRes(EncoderRes), m_Position(0.0),
  m_MinAngle(min), m_MaxAngle(max), m_homed(false)
  
{
  UTRACE("UderoMockJoint::UderoMockJoint(id=%d, gear=%f, encoder=%f, min=%f, max=%f)",
	 id, gearRatio, EncoderRes, min, max);
  m_polar = 1;
  accelBuffer = 0;
  velocityBuffer = 0;
  setMode(MODE_INACTIVE);
}


UderoMockJoint::~UderoMockJoint() {}

void UderoMockJoint::setHomeOffset(const int32_t offsetCount) {
  UTRACE("UderoMockJoint::setHomeOffset(%d)", offsetCount);
}

void UderoMockJoint::reset() {
  UTRACE("UderoMockJoint::reset()");
}

void UderoMockJoint::init() {
  UTRACE("UderoMockJoint::init()");
}


UderoREAL UderoMockJoint::getPosition(bool PDO) {
  UVERBOSE("UderoMockJoint(%d)::getPosition() : returns %f", m_CANID, m_Position);
  return m_Position;
}

void UderoMockJoint::goHome() {
  UTRACE("UderoMockJoint::goHome()");
  m_homed = true;
}

void UderoMockJoint::waitHoming() {
  UTRACE("UderoMockJoint::waitHoming()");
  ssr::Thread::Sleep(500);
  UTRACE("UderoMockJoint::homingOkay()");
}

UderoREAL UderoMockJoint::getHomingSpeed() {
	UTRACE("UderoMockJoint::getHomingSpeed() returns 0.0");
	return 0.0;
}

UderoREAL UderoMockJoint::getActualVelocity() {
	UTRACE("UderoMockJoint::getActualVelocity() returns 0.0");
	return 0.0;
}


bool UderoMockJoint::isHomed() {
  UTRACE("UderoMockJoint::isHomed()");
  return m_homed;
}

void UderoMockJoint::waitStopped(int32_t timeoutMillis) {
  UTRACE("UderoMockJoint::waitStopped(timeout=%d [ms])", timeoutMillis);
  //ssr::Thread::Sleep(100);
}

void UderoMockJoint::quickStop() {
  UTRACE("UderoMockJoint::quickStop()");
}

void UderoMockJoint::enableOperation() {
  UTRACE("UderoMockJoint::enableOperation()");
}

void UderoMockJoint::setMode(const UderoJointMode& mode) {
  UTRACE("UderoMockJoint::setMode(%d)", mode);
}

void UderoMockJoint::setAcceleration(const UderoREAL accel) {
  UTRACE("UderoMockJoint::setAcceleration(%f)", accel);
}


void UderoMockJoint::movePositionWithVelocity(const UderoREAL targetPosition_, const UderoREAL targetVelocity, const bool minMaxOff) {
  UTRACE("UderoMockJoint::movePositionWithVelocity(%f, %f)", targetPosition_, targetVelocity);  
  if (targetPosition_ > m_MaxAngle || targetPosition_ < m_MinAngle) {
    //	  std::cout << "hoge" << std::endl;
  }
  UderoREAL targetPosition = targetPosition_ > m_MaxAngle ? m_MaxAngle : (targetPosition_ < m_MinAngle ? m_MinAngle : targetPosition_);

  if (targetPosition != targetPosition_) {
    UTRACE(" - Change Target (%f -> %f)", targetPosition_, targetPosition);
  }
  velocityBuffer = targetVelocity;
  m_Position = targetPosition;
}

void UderoMockJoint::moveVelocity(const UderoREAL targetVelocity) {
}
