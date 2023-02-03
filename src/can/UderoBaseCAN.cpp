

#include "UderoCAN.h"
#include "udero/UderoLogger.h"
#include "UderoBaseCAN.h"
#include "UderoCANJoint.h"
#include "../global_parameter.h"



using namespace reharo;



class UderoBaseCAN : public reharo::IUderoBase {
private:
  canopen::ICANInterface* m_pCAN;
  canopen::CANopen* m_pCANopen;

  UderoCANJoint* joint;
public:
  UderoBaseCAN(canopen::ICANInterface* pCAN, canopen::CANopen* pCANopen): 
    m_pCAN(pCAN), m_pCANopen(pCANopen) {
    joint = new UderoCANJoint(8,
			      m_pCANopen,
			      base_gear_ratio,
			      base_encoder_res,
			      base_angle_min,
			      base_angle_max);
  }
  virtual ~UderoBaseCAN() {}

  virtual std::string getClassName() const { return "UderoBaseCAN"; }

    /**
     * 関節の加速度を設定
     * @param index 関節のインデックス
     * @param accel 加速度 [rad/s^2]
     */
    virtual void setJointAcceleration(const int32_t index, const UderoREAL accel) {}

    /**
     * 関節のホーミング時のオフセットを設定
     * @param index 関節のインデックス
     * @param offset オフセット量 [cnt]
     */
    virtual void setJointHomeOffset(const int32_t index, const int32_t offset) {}

    /**
     * 関節のホーミング時のオフセットを設定
     * @param index 関節のインデックス
     * @param offset オフセット量 [rad]
     */
    virtual void setJointMode(const int32_t index, const UderoJointMode& mode) {
      if (index != 0) {
	UWARN("UderoBaseCAN::setJointMode() failed. Invalid index");
	return;
      }
      joint->setMode(mode);
    }

    /**
     * 関節角度の取得
     * @param index 関節のインデックス
     * @return 関節の角度 [rad]
     */
    virtual UderoREAL getJointPosition(const int32_t index) {
      if (index != 0) return 0.0;
      bool PDO = false;
      return joint->getPosition(PDO);
    }

    virtual void setJointHomingMode(const int32_t index, const HomingMode mode) {}

    virtual void goHomeJoint(const int32_t index) {
      if (index != 0) return;
      joint->goHome();
    }

    /**
     * 各変数の初期化
     */
    virtual void init() {
      joint->init();
    }
    
    /**
     * 各変数の終了処理
     */
    virtual void fini() {}

    /**
     * 状態のアップデート処理．この関数は周期的に呼ぶ必要がある
     */
    virtual void spin() {}

    /**
     * 通信エラーなどが起こった場合にリセットを行う関数
     */
    virtual void reset() {
      UTRACE("UderoBaseCAN::reset()");
      init();

    }

    /**
     * 関節を一つずつ動かすときに使う関数．手首がうまく動作しないので注意
     */
    virtual void moveJoint(const int32_t index, const UderoREAL pos, const UderoREAL vel, const bool minMaxOff=false) {
      joint->movePositionWithVelocity(pos, vel);
    }


    /**
     * 動作終了までブロックする関数
     *
     * @param blocking trueならば停止状態まで処理をブロックする。falseならばisStoppedメソッドで停止状態になったか否かを判定できるようになる。停止状態になったらwaitReadyメソッドでready状態に変更することができる。
     */
    virtual void waitStopped(bool blocking=true) {
      double timeout=40.0;
      joint->waitStopped(timeout);
    }

    /**
     * Ready状態になるまでwait
     */
    virtual void waitReady() {}

    /**
     * stopped状態か否かを判定
     */
    virtual bool isStopped() {}
    /**
     * 急停止
     */
    virtual void quickStop() {}

    /**
     * ホーミングを行う
     *
     * @param blocking ホーミングが終了するまで処理をブロックするならtrueを渡すこと
     * @param force ホーミング完了している状態の関節ではホーミング処理をスキップするならfalseを渡す
     */
    virtual void goHome(bool blocking = true, bool force = false) {
      if (force || !joint->isHomed()) {
	joint->goHome();
      }
      
      if(blocking) {
	joint->waitHoming();
      }
    }

    /**
     * ホーミングが終了するまで処理をブロックする関数
     */
    virtual void waitHoming() {
      joint->waitHoming();
    }

    /**
     * ホーミング中か否かの判定関数
     *
     * @return ホーミング中ならばtrue
     */
    virtual bool isHoming() {}

    /**
     * 関節一つだけ動かします
     *
     * @param index
     * @param velocity 速度 [rad/sec]
     */
    virtual void moveJointVelocity(const int32_t index, const double velocity) {}

    /**
     * ジョイントの角速度取得
     * @param index
     * @return 速度 [rad/sec]
     */
    virtual double getJointActualVelocity(const int32_t index) {}

    /**
     * ジョイントホーミングが完了しているか
     * @param index
     * @return 完了していればtrue
     */
    virtual bool isJointHomed(const int32_t index) {}

    /**
     * クイックストップします
     * @param index
     */
    virtual void quickStopJoint(const int32_t index) {}
    
    /**
     */
    virtual uint16_t getJointDigitalInput(const int32_t index) {}


    /**
     */
    virtual JOINT_STATE getJointState(const int32_t index) {}

    /**
     */
    virtual void faultResetJoint(const int32_t index) {}
};


LIBUDERO_API reharo::IUderoBase* createCANUderoBase(reharo::IUdero* udero) {
  canopen::ICANInterface *pCAN = getCAN(udero);
  canopen::CANopen* pCANopen = getCANopen(udero);
  if (pCAN == NULL || pCANopen == NULL) {
    UERROR("CAN NOT GET CAN device information. createCANUderoBase failed.");
    return NULL;
  }

  return new UderoBaseCAN(pCAN, pCANopen);
}
