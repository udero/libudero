
/**
 * UderoBase.h
 * 
 * libuderoで台座をつかうためのメインインターフェースとヘルパー関数
 *
 * @author Yuki Suga
 * @copyright Techno Tool, 2015
 */

#pragma once

#include "Udero.h"

#define UDERO_BASE_ID 8


namespace reharo {


  /**
   * Uderoクラスインターフェース
   */ 
  class IUderoBase;


  /**
   * Uderoクラスインターフェースを実装したオブジェクトを生成する
   *
   * @param prof 接続プロファイル
   * @return Uderoクラスインターフェースを実装したクラスオブジェクトへのポインタ
   */
  LIBUDERO_API IUderoBase* createUderoBase(IUdero* pUdero);

  /**
   * Uderoクラスインターフェースを実装したオブジェクトの削除関数
   */
  LIBUDERO_API void deleteUderoBase(IUderoBase* udero);


  /**
   * Uderoクラスインターフェース
   */
  class IUderoBase {
  public:
    IUderoBase() {}
    virtual ~IUderoBase() {}

    virtual std::string getClassName() const = 0;

    /**
     * 関節の加速度を設定
     * @param index 関節のインデックス
     * @param accel 加速度 [rad/s^2]
     */
    virtual void setJointAcceleration(const int32_t index, const UderoREAL accel) = 0;

    /**
     * 関節のホーミング時のオフセットを設定
     * @param index 関節のインデックス
     * @param offset オフセット量 [cnt]
     */
    virtual void setJointHomeOffset(const int32_t index, const int32_t offset) = 0;

    /**
     * 関節のホーミング時のオフセットを設定
     * @param index 関節のインデックス
     * @param offset オフセット量 [rad]
     */
    virtual void setJointMode(const int32_t index, const UderoJointMode& mode) = 0;

    /**
     * 関節角度の取得
     * @param index 関節のインデックス
     * @return 関節の角度 [rad]
     */
    virtual UderoREAL getJointPosition(const int32_t index) = 0;

    virtual void setJointHomingMode(const int32_t index, const HomingMode mode) = 0;

    virtual void goHomeJoint(const int32_t index) = 0;

    /**
     * 各変数の初期化
     */
    virtual void init() = 0;
    
    /**
     * 各変数の終了処理
     */
    virtual void fini() = 0;

    /**
     * 状態のアップデート処理．この関数は周期的に呼ぶ必要がある
     */
    virtual void spin() = 0;

    /**
     * 通信エラーなどが起こった場合にリセットを行う関数
     */
    virtual void reset() = 0;

    /**
     * 関節を一つずつ動かすときに使う関数．手首がうまく動作しないので注意
     */
    virtual void moveJoint(const int32_t index, const UderoREAL pos, const UderoREAL vel, const bool minMaxOff=false) = 0;


    /**
     * 動作終了までブロックする関数
     *
     * @param blocking trueならば停止状態まで処理をブロックする。falseならばisStoppedメソッドで停止状態になったか否かを判定できるようになる。停止状態になったらwaitReadyメソッドでready状態に変更することができる。
     */
    virtual void waitStopped(bool blocking=true) = 0;

    /**
     * Ready状態になるまでwait
     */
    virtual void waitReady() = 0;

    /**
     * stopped状態か否かを判定
     */
    virtual bool isStopped() = 0;
    /**
     * 急停止
     */
    virtual void quickStop() = 0;

    /**
     * ホーミングを行う
     *
     * @param blocking ホーミングが終了するまで処理をブロックするならtrueを渡すこと
     * @param force ホーミング完了している状態の関節ではホーミング処理をスキップするならfalseを渡す
     */
    virtual void goHome(bool blocking = true, bool force = false) = 0;

    /**
     * ホーミングが終了するまで処理をブロックする関数
     */
    virtual void waitHoming() = 0;

    /**
     * ホーミング中か否かの判定関数
     *
     * @return ホーミング中ならばtrue
     */
    virtual bool isHoming() = 0;

    /**
     * 関節一つだけ動かします
     *
     * @param index
     * @param velocity 速度 [rad/sec]
     */
    virtual void moveJointVelocity(const int32_t index, const double velocity) = 0;

    /**
     * ジョイントの角速度取得
     * @param index
     * @return 速度 [rad/sec]
     */
    virtual double getJointActualVelocity(const int32_t index) = 0;

    /**
     * ジョイントホーミングが完了しているか
     * @param index
     * @return 完了していればtrue
     */
    virtual bool isJointHomed(const int32_t index) = 0;

    /**
     * クイックストップします
     * @param index
     */
    virtual void quickStopJoint(const int32_t index) = 0;
    
    /**
     */
    virtual uint16_t getJointDigitalInput(const int32_t index) = 0;


    /**
     */
    virtual JOINT_STATE getJointState(const int32_t index) = 0;

    /**
     */
    virtual void faultResetJoint(const int32_t index) = 0;
  };

};
