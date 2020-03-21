/**
 * @file UderoImpl.h
 * @author Yuki Suga (ysuga@ysuga.net)
 */


#pragma once
#include <iostream>
#include "udero/UderoLogger.h"
#include "udero/Udero.h"

namespace reharo {
  
  /**
   * 関節レベルでのサービスを抽象化するインターフェース
   */
  class UderoJoint {
  public:
    /**
     *
     */
    UderoJoint() {}

    /**
     *
     */
    virtual ~UderoJoint() {}

  public:
    /**
     * 初期化
     */
    virtual void init() = 0;

    /**
     * 回転方向の設定
     * @param polarity [-1|1] 
     */
    virtual void setPolar(int polarity) = 0;

    /**
     * ホーミングを開始する。ホーミング開始後にすぐに処理は帰ってくる
     */
    virtual void goHome() = 0;

    /**
     * ホーミングが終了するまで処理をブロックする
     */
    virtual void waitHoming() = 0; 

	/*
	 *
	 */
	virtual UderoREAL getHomingSpeed() = 0;


    /**
     * リセットする
     */
    virtual void reset() = 0;

    /**
     * ホーミング処理を行ったか否か
     */
    virtual bool isHomed() = 0;

    /**
     * ホーミングのオフセットを設定する
     */
    virtual void setHomeOffset(const int32_t offsetCount) = 0;

    virtual void setHomingMode(const HomingMode mode) = 0;

    /**
     * 停止状態まで処理を止めます。
     */
    virtual void waitStopped(int32_t timeoutMillis = 1000) = 0;
    virtual UderoREAL getPosition(bool PDO=true) = 0;
    virtual void setMode(const UderoJointMode& mode) = 0;
    virtual void setAcceleration(const UderoREAL accel) = 0;
    virtual void movePositionWithVelocity(const UderoREAL targetPosition, const UderoREAL targetVelocity, const bool minMaxOff=false) = 0;
    virtual void moveVelocity(const UderoREAL targetVelocity) = 0;
	virtual UderoREAL getActualVelocity() = 0;
    virtual void quickStop() = 0;
    virtual void enableOperation() = 0;
    virtual void setMinMax(const UderoREAL min, const UderoREAL max) = 0;

	virtual uint16_t getDigitalInput() = 0;

	virtual JOINT_STATE getJointState() = 0;

  };


  class UderoImpl : public IUdero, public ssr::Thread {
  public:
    UderoJoint* ppJoints[UDERO_MAX_JOINT];
    uint32_t numJoints;
  private:
    void _checkJointIndex(const int index) {
      if (index < 0 || index >= numJoints) {
	UERROR("JointIndexOutOfRange");
	throw reharo::JointIndexOutOfRangeException();
      }
    }
    
    // if true, OK, not invalid targetJoint position.
    bool checkTarget(const std::vector<double>& target, const Matrix44& mat);
    bool checkTargetWrist(const std::vector<double>& target, const Matrix44& mat);
  protected:
    std::string task_;
    bool homingForce;
    double foldingSpeed;

    std::vector<UderoREAL> jointPos_;

    Matrix44 forwardKinematics_;
    Matrix44 forwardKinematicsWrist_;
  private:
    std::vector<double> foldingVelocity;
  public:
    UderoImpl();
    UderoImpl(const std::string& settingFilename);
    virtual ~UderoImpl();
    
    virtual void waitReady();
    //void logprint(int level, const char* format, ...);
  private:
    void _goPose0();
    void _goPoseF();
    void _goPoseW();
    void _goPoseI();

  public:
    virtual void init();
    virtual void fini() {};
    virtual void spin();
    virtual void reset() {};
    
    virtual void setJointAcceleration(const int32_t index, const UderoREAL accel) {
      UTRACE("UderoImpl::setJointAcceleration(%d, %f)", index, accel);
      _checkJointIndex(index);
      ppJoints[index]->setAcceleration(accel);
    }

    virtual void setJointHomingMode(const int32_t index, const HomingMode mode) {
      UTRACE("UderoImpl::setJointHomingMode(%d, %d)", index, mode);
      _checkJointIndex(index);
      ppJoints[index]->setHomingMode(mode);
    }

    virtual void setJointHomeOffset(const int32_t index, const int32_t offset) {
      UTRACE("UderoImpl::setJointHomeOffset(%d, %d)", index, offset);
      _checkJointIndex(index);
      ppJoints[index]->setHomeOffset(offset);
    }

    virtual void setJointMode(const int32_t index, const UderoJointMode& mode) {
      UTRACE("UderoImpl::setJointMode(%d, %d)", index, mode);
      _checkJointIndex(index);
      ppJoints[index]->setMode(mode);
    }

    virtual UderoREAL getJointPosition(const int32_t index) {
      _checkJointIndex(index);
      return getJointPos(index);
    }

    virtual void goHome(bool blocking=true, bool force = false);
    void goHomeImpl(bool force = false);
    void goHomeInner(bool force = false);
    virtual void waitHoming();
    virtual bool isHoming();

    virtual void foldIn(bool blocking=true);
    void foldInImpl();
    virtual void waitFoldIn();
    virtual bool isFoldingIn();

    virtual void foldOut(bool blocking=true);
    void foldOutImpl();
    virtual void waitFoldOut();
    virtual bool isFoldingOut();
    virtual void moveHand(const UderoREAL gap, const UderoREAL vel);
    virtual void moveHandRel(const UderoREAL gap, const UderoREAL vel);
    virtual void moveHandSync(const UderoREAL gap, const UderoREAL vel);
    virtual void moveHandRelSync(const UderoREAL gap, const UderoREAL vel);

    virtual void moveJoint(const int32_t index, const UderoREAL pos, const UderoREAL vel, const bool minMaxOff=false);
		
    double getJointPos(const int index, const bool PDO=true);

    virtual void moveJoints(const std::vector<UderoREAL>& pos, const std::vector<UderoREAL>& vel);

    virtual void waitStopped(bool blocking);
    void waitStoppedImpl(int32_t timeout=1000);
    virtual bool isStopped();

    virtual void quickStop();

    virtual int moveCartesian(const Matrix44& mat, const UderoREAL step_time);
    virtual int moveCartesianWrist(const Matrix44& mat, const UderoREAL step_time);

    virtual Matrix44 forwardKinematics();

    virtual Matrix44 forwardKinematicsWrist();


	virtual void goHomeJoint(const int32_t index) {
		this->ppJoints[index]->goHome();
	}

	virtual void moveJointVelocity(const int32_t index, const double velocity) {
		this->ppJoints[index]->moveVelocity(velocity);
	}

	virtual double getJointActualVelocity(const int32_t index) {
		return this->ppJoints[index]->getActualVelocity();
	}

	virtual bool isJointHomed(const int32_t index) {
		return this->ppJoints[index]->isHomed();
	}

	virtual void quickStopJoint(const int32_t index) {
		 this->ppJoints[index]->quickStop();
	}

	virtual uint16_t getJointDigitalInput(const int32_t index) {
		return this->ppJoints[index]->getDigitalInput();
	}

	virtual JOINT_STATE getJointState(const int32_t index) {
	  return this->ppJoints[index]->getJointState();
	}

	virtual void faultResetJoint(const int32_t index) {
	  return this->ppJoints[index]->init();
	}

  public:
    virtual void Run();
  };

};
