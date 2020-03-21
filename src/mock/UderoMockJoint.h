#pragma once

#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "UderoImpl.h"
#include "UderoMock.h"

namespace reharo {

  class UderoMockJoint : public UderoJoint {
  private:
    int m_polar;
    uint32_t m_CANID;
    double m_jointPosBuffer;
    bool m_homed;
    UderoREAL accelBuffer;
    UderoREAL velocityBuffer;
    UderoREAL m_Position;

  public:

    // do nothing
    virtual void setHomingMode(const HomingMode mode) {}

    virtual void setHomeOffset(const int offsetCount);
  public:
    void setPolar(int polar) {
      if (polar >= 0) m_polar = 1;
      else m_polar = -1;
    }

  private:
    inline int32_t pos_to_inc(double pos) {

      return (int32_t)((pos * m_gearRatio * 4 * m_encoderRes) / (2 * M_PI));
    }

    inline double inc_to_pos(int32_t inc) {
      //return inc;
      return ((inc) * 2 * M_PI) / (m_gearRatio * 4 * m_encoderRes);
    }

    inline int32_t radPerSec_to_incPerMilliSec(double radPerSec) {
      return (int32_t)(((radPerSec / 1000) / (2 * M_PI) * m_gearRatio * 4 * m_encoderRes));
    }

    inline int32_t radPerSec_to_rpm(double radPerSec) {
      return (int32_t)(((radPerSec) / (2 * M_PI) * m_gearRatio * 60));
    }

    inline double incPerMilliSec_to_radPerSec(int32_t incPerMilliSec) {
      return (incPerMilliSec * 1000) / fabs(m_gearRatio) * (2 * M_PI) / (4 * m_encoderRes);
    }

    inline double rpm_to_radPerSec(int32_t rpm) {
      return (rpm / 60.0) / fabs(m_gearRatio) * (2 * M_PI);
    }
  public:
    UderoMockJoint(const uint32_t& id, double gearRatio, double encoderRes, const UderoREAL min, const UderoREAL max);
    virtual ~UderoMockJoint();
  public:

    virtual void setAcceleration(const UderoREAL accel);


    double m_gearRatio;

    double m_encoderRes;

    virtual void reset();

    virtual UderoREAL getPosition(bool PDO = true);
    virtual void init();
    virtual void goHome();
    virtual bool isHomed();
	virtual UderoREAL getHomingSpeed();
	virtual UderoREAL getActualVelocity();
    virtual void waitHoming();
    virtual void waitStopped(int32_t timeoutMillis=1000);
    virtual void setMode(const UderoJointMode& mode);
    virtual void movePositionWithVelocity(const UderoREAL targetPosition, const UderoREAL targetVelocity, bool minMaxOff=false);
    virtual void moveVelocity(const UderoREAL targetVelocity);
    virtual void quickStop();
    virtual void enableOperation();
  private:
    UderoREAL m_MinAngle;
    UderoREAL m_MaxAngle;
    virtual void setMinMax(const UderoREAL min, const UderoREAL max) {
      m_MinAngle = min; m_MaxAngle = max;
    }

	virtual uint16_t getDigitalInput() {
		return 0;
	}

	virtual JOINT_STATE getJointState() {
	  return ENABLED;
	}
  };

};
