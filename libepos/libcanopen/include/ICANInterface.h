/**
 * @file ICANInterface.h
 * @copyright RT Corp. 2012 All rights reserved.
 */
#ifndef INTERFACE_CAN_INTERFACE_HEADER_INCLUDED
#define INTERFACE_CAN_INTERFACE_HEADER_INCLUDED


#include "CANMSG.h"
#include "CANException.h"

/**
 * @if jp
 * @brief CANのボーレート
 * @elif
 * @endif
 */
enum ICAN_BAUD {
	ICAN_BAUD_1M,
	ICAN_BAUD_500K,
	ICAN_BAUD_250K,
	ICAN_BAUD_125K,
	ICAN_BAUD_100K,
	ICAN_BAUD_50K,
	ICAN_BAUD_10K,
	ICAN_BAUD_5K,
};

/**
 */
enum CAN_HRESULT {
	CAN_E_FAIL = -1,
	CAN_S_OK = 0,
};



/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {

	/**
	 * @brief CANインターフェースを利用するための標準インターフェース
	 *
	 */
	class ICANInterface
	{

	public:
		ICANInterface() {}

		virtual ~ICANInterface() {}

		/**
		 * @brief 初期化関数
		 * @param ボーレート
		 * @see ICAN_BAUD
		 */
		virtual void initialize(const ICAN_BAUD baudrate) throw(CANException) = 0;


		/**
		 * @brief 終了処理関数
		 */
		virtual void uninitialize() = 0;

		/**
		 * @if jp
		 * @brief CANメッセージのバッファからの取り出し
		 * @param msg [OUT] メッセージを格納するCANMSG構造体
		 * @return データ受信成功ならばtrue．失敗ならばfalse
		 * @elif
		 * @brief CAN message receive from Buffer
		 * 
		 * @param msg [OUT] CANMSG class object 
		 * @return True if data is successfully read.
		 * @endif
		 */
		virtual bool receive(CANMSG* msg) = 0;

		/**
		 * @if jp
		 * @brief CANメッセージの送信
		 * @param msg [IN] メッセージを格納するCANMSG構造体
		 * @elif
		 * @endif
		 */
		virtual void send(const CANMSG& msg) = 0;

	};

}


#endif