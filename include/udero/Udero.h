/**
 * Udero.h
 * 
 * libuderoをつかうためのメインインターフェースとヘルパー関数
 *
 * createUdero関数を使ってIUderoインターフェースのオブジェクトを生成して使う。詳しくはappsのサンプルを参照すること。
 *
 * @author Yuki Suga
 * @copyright Technotools, 2015
 */

#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "defines.h"
#include "kinematics.h"
#include "UderoException.h"

#define UDERO_MAX_JOINT 8



namespace technotools {

  /**
   * エラー状態変数
   */
  enum UDERO_ERROR_ENUM {
    E_TARGETOUTOFRANGE = 1,
    O_OK = 0,
  };

  /**
   * ホーミングモード
   */
  enum HomingMode {
    ABSOLUTE_POSITION = 1,
  };

  /**
   * 関節状態
   */
  enum JOINT_STATE {
    ENABLED, // サーボON
    DISABLED, // サーボOFFで待機状態
    FAULT, // FAULT状態。エラーの状態でリセットを待っている。そのままサーボONにはなれない
  };
  
  /**
   * Uderoの通信方法を定める構造体 (SOCKET関連は将来的なアップデートのための予約)
   *
   */
  struct UderoConnectionProfile {
    std::string connectionType; //< 通信形態．[CAN | SOCKET | MOCK]
    std::string ipAddress; //< SOCKET通信時の宛先IPアドレス
    int32_t port; //< SOCKET通信時の宛先ホストのポート番号
    std::string canFilename; //< CAN接続時の接続先デバイスファイル名
    int32_t canBaudrate; //< CAN接続時のボーレート (1M固定)
    int32_t canDeviceID; //< CAN接続時のCANデバイスのID番号
    int32_t withInit; //< 
    std::string settingFilename; //< Uderoの設定ファイルのファイル名
    std::string logFileName; //< ログのファイル名
    int32_t logLevel; //< ログレベル [1, 5]
    FILE* logFile;

    std::vector<std::string> unknown_args; // 上記のオプションに捕まらなかったコマンドライン引数。順序は保存される。
  };

  /**
   * Uderoクラスインターフェース
   */ 
  class IUdero;

  /**
   * 実行時引数から接続プロファイルを生成するヘルパー関数
   * 実行時引数は以下の通り：(SOCKET関連は将来的なアップデートのための予約)
   * -c, --connectionType 接続タイプ [CAN|SOCKET|MOCK]
   * -i, --ipAddress SOCKET接続した場合の宛先ホストのIPアドレス
   * -p, --port SOCKET接続した場合の宛先ホストのポート番号
   * -f, --filename CAN接続した場合のデバイス名
   * -l, --logFile LOGファイル名
   * -e, --logLevel LOGレベル [1, 5]
   *
   * @param argc コマンドライン引数の数
   * @param argv コマンドライン引数の配列
   * @return UderoConnectionProfile構造体
   */
  LIBUDERO_API UderoConnectionProfile parseArgs(const int argc, const char* argv[]);

  /**
   * Uderoクラスインターフェースを実装したオブジェクトを生成する
   *
   * @param prof 接続プロファイル
   * @return Uderoクラスインターフェースを実装したクラスオブジェクトへのポインタ
   */
  LIBUDERO_API IUdero* createUdero(const UderoConnectionProfile& prof);

  /**
   * Uderoクラスインターフェースを実装したオブジェクトの削除関数
   */
  LIBUDERO_API void deleteUdero(IUdero* udero);

  /**
   * 関節の制御モードの設定
   */
  enum UderoJointMode {
    MODE_INACTIVE, //< サーボオフ状態
    MODE_POSITION, //< 位置速度制御状態
    MODE_VELOCITY, //< 速度制御状態
  };

  /**
   * Uderoクラスインターフェース。このインターフェースでUderoを動かす。
   */
  class IUdero {
  public:
    IUdero() {}
    virtual ~IUdero() {}


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

    /**
     * ホーミングモードの設定
     * @param index
     * @param mode
     */
    virtual void setJointHomingMode(const int32_t index, const HomingMode mode) = 0;

    /**
     * ホーミング指令
     * @param index
     */
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
     * ハンドの開閉動作。停止までまち、Fault状態になったら自動でリセットする。
     * @param gap ハンドの開いている量 [m]
     * @param vel
     */
    virtual void moveHandSync(const UderoREAL gap, const UderoREAL vel) = 0;

    /**
     * ハンドの開閉動作 
     * @param gap ハンドの開いている量 [m]
     * @param vel
     */
    virtual void moveHand(const UderoREAL gap, const UderoREAL vel) = 0;

    /**
     * ハンドの開閉動作．ただし現在の位置からの差分で動かす．
     */
    virtual void moveHandRel(const UderoREAL gap, const UderoREAL vel) = 0;

    /**
     * ハンドの開閉動作．ただし現在の位置からの差分で動かす．停止までまち、Fault状態になったら自動で戻る
     */
    virtual void moveHandRelSync(const UderoREAL gap, const UderoREAL vel) = 0;

    /**
     * 関節の動作．全関節を同時に動かす
     *
     * @param pos 6つの実数要素を持つvector．各関節の目標位置 [rad]
     * @param vel 6つの実数要素を持つvector．各関節の目標速度 [rad/sec]
     */
    virtual void moveJoints(const std::vector<UderoREAL>& pos, const std::vector<UderoREAL>& vel) = 0;

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
     * 収納状態への移動
     */
    virtual void foldIn(bool blocking = true) = 0;

    /**
     * 展開状態への移動
     */
    virtual void foldOut(bool blocking = true) = 0;

    /**
     * 収納状態への移動が終了するまでブロックする関数
     */
    virtual void waitFoldIn() = 0;

    /**
     * 展開状態への移動が終了するまでブロックする関数
     */
    virtual void waitFoldOut() = 0;

    /**
     * 収納状態への移動中か否かを判定する関数
     *
     * @return 収納状態へ移動中であればtrue
     */
    virtual bool isFoldingIn() = 0;

    /**
     * 展開状態へ移動中か否かを確認する関数
     *
     * @return 展開状態へ移動中ならばtrue
     */
    virtual bool isFoldingOut() = 0;

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
     * デカルト座標系での移動．手先中心で移動する
     *
     * @param mat 4x4のマトリックス．目的の位置・姿勢を表現する変換行列
     * @param step_time 目的の位置姿勢までの目標移動時間
     */
    virtual int moveCartesian(const Matrix44& mat, const UderoREAL step_time) = 0;

    /**
     * デカルト座標系での移動．手首中心で移動する
     *
     * @param mat 4x4のマトリックス．目的の位置・姿勢を表現する変換行列
     * @param step_time 目的の位置姿勢までの目標移動時間
     */
    virtual int moveCartesianWrist(const Matrix44& mat, const UderoREAL step_time) = 0;

    /**
     * 手先の位置を得る関数．
     * ただしこの計算値はspin関数でアップデートされる．
     */
    virtual Matrix44 forwardKinematics() = 0;

    /**
     * 手首の位置姿勢を計算して返す
     * ただしこの計算値はspin関数でアップデートされる．
     */
    virtual Matrix44 forwardKinematicsWrist() = 0;

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
     * 指定した関節をクイックストップします
     * @param index
     */
    virtual void quickStopJoint(const int32_t index) = 0;
    
    /**
     * 関節を制御中のモーターコントローラのDigitalInを取得します
     * @param index
     * @return DigitalInの状態
     */
    virtual uint16_t getJointDigitalInput(const int32_t index) = 0;


    /**
     * 関節の状態を取得します
     * @param index
     * @return 関節状態
     */
    virtual JOINT_STATE getJointState(const int32_t index) = 0;

    /**
     * 関節コントローラがFault状態の場合に、それをリセットします。
     * @param index
     */
    virtual void faultResetJoint(const int32_t index) = 0;
  };

};
