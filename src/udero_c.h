
/**
* Udero.h
*
* @author Yuki Suga
* @copyright Techno Tool, 2015
*/

#pragma once

#include "libudero.h"

#ifdef __cplusplus
extern "C" {
#endif


#define UDERO_NUM_JOINT 8

	/**
	 * Uderoの初期化
	 * 
	 * @param can_net ESDのCAN-USBのドライバにおけるnet番号．通常は0
	 * @param baudrate ボーレート．1000000 (1M) が通常の値．
	 * @param handle Udero_t型変数へのポインタ．Udero_t型変数を使って関節制御などを行うので保存しておくこと
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_init(const uint32_t can_net, const uint32_t baudrate, Udero_t* handle);

	/**
	 * Uderoの終了
	 * 
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_cleanup(const Udero_t handle);

	/**
	 * Uderoの各関節の目標角度設定
     *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @param id 関節のID番号 [0, 6]
	 * @param position 目標関節角度 [rad]
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_setTargetJointPosition(const Udero_t handle, const UderoJointID id, const UderoREAL position);

	/** 
	 * Uderoの各関節の角度取得
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @param id 関節のID番号 [0, 6]
	 * @param position 間接角度を格納するためのUderoREAL型変数へのポインタ．単位rad．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_getActualJointPosition(const Udero_t handle, const UderoJointID id, UderoREAL* position);

	/**
	 * Uderoの状態取得
	 * 
	 * この関数を定期的に呼ばなければいけない．この関数を呼ばれるとPCがUderoと通信し，間接角度等の情報を最新情報に更新する．
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_update(const Udero_t handle);

	/**
	 * UderoをInactive状態に変更
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_deactivate(const Udero_t handle);

	/**
	 * UderoをPWM状態に変更
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_activatePWM(const Udero_t handle);

	/**
	 * Uderoを電流制御モードに変更 (現在は使えない) 
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_activateCurrent(const Udero_t handle);

	/**
	 * Uderoを速度制御モードに変更 (現在は使えない) 
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_activateVelocity(const Udero_t handle);

	/**
 	 * Uderoを位置制御モードに変更
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_activatePosition(const Udero_t handle);

	/**
	 * Uderoのハンド閉じ指令
	 * 
	 * ハンド閉じ動作を開始する．Udero_handStop関数を呼ばなければ閉じ動作が停止しないので注意して使用すること
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_handClose(const Udero_t handle);

	/**
	 * Uderoのハンド開き動作
	 *
	 * ハンドの開き動作を開始する．Udero_handStop関数を呼ばなければ開き動作が停止しないので注意して使用すること．
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_handOpen(const Udero_t handle);

	/**
	 * Uderoのハンド開き・閉じ動作の停止
	 *
	 * @param handle Udero_t型変数．Udero_init関数で得られた値を使うこと．
	 * @return UderoResult_t型変数．Udero_OKならば成功．
	 */
	LIBUDERO_API UderoResult_t Udero_handStop(const Udero_t handle);

#ifdef __cplusplus
}
#endif
