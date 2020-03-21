#pragma once

#include <stdint.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include "can_param.h"
#include "UderoImpl.h"
#include "UderoCAN.h"
#include "EPOS.h"


namespace reharo {

  class UderoCANJoint : public UderoJoint {
  private:
    maxon::EPOS* m_pEPOS;
    uint32_t m_CANID;

    int m_polar;

    double m_jointPosBuffer;

    UderoREAL accelBuffer;
    UderoREAL velocityBuffer;

    //FILE* logfile;
    //int loglevel;

    int homing_param[NUM_HOMING_PARAM];

    //void logprint(int level, const char* format, ...);
  public:

    virtual void setHomeOffset(const int offsetCount);

    virtual void setHomingMode(const HomingMode mode);
  public:

    void setupPDO();

    void setupTXPDO1(const ssr::TimeSpec& timeout = INFINITETIME);
    void setupRXPDO1(const ssr::TimeSpec& timeout = INFINITETIME);
    void setupRXPDO2(const ssr::TimeSpec& timeout = INFINITETIME);
    void setupRXPDO4(const ssr::TimeSpec& timeout = INFINITETIME);

    void setPolar(int polar) {
      if (polar >= 0) m_polar = 1;
      else m_polar = -1;
    }

  private:
    inline int32_t pos_to_inc(double pos) {
      //return pos;
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
    UderoCANJoint(const uint32_t& id, canopen::CANopen* pCANopen, double gearRatio, double encoderRes, const UderoREAL min, const UderoREAL max);
    virtual ~UderoCANJoint();
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
    virtual void waitStopped(int32_t timeoutMillis = 1000);
    virtual void setMode(const UderoJointMode& mode);
    virtual void movePositionWithVelocity(const UderoREAL targetPosition, const UderoREAL targetVelocity, const bool minMaxOff=false);
    virtual void moveVelocity(const UderoREAL targetVelocity);
    virtual void quickStop();
    virtual void enableOperation();
  private:
    UderoREAL m_MinAngle;
    UderoREAL m_MaxAngle;
    virtual void setMinMax(const UderoREAL min, const UderoREAL max) {
      m_MinAngle = min; m_MaxAngle = max;
    }

	virtual uint16_t getDigitalInput();

	virtual JOINT_STATE getJointState();
  };

};
