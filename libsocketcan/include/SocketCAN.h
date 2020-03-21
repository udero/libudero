/**
 * @file SocketCAN.h
 */

#ifndef SOCKETCAN_HEADER_INCLUDED
#define SOCKETCAN_HEADER_INCLUDED

#include "ICANInterface.h"

#define LIBSOCKETCAN_API 

#include "linux/can.h"
#include "linux/can/raw.h"
#include "linux/can/error.h"
#include <string>


/**
 * @if jp
 * @brief SocketCAN 対応CANバスインターフェースの名前空間
 * 
 * @else
 * @brief
 * @endif
 */
namespace ssr {


	/**
	 * @if jp
	 * @brief SocketCAN用CANInterfaceクラスの実装．
	 * @else
	 * @brief CANInterface implementation for SOCKETCAN
	 * @endif
	 */
    class LIBSOCKETCAN_API SocketCAN : public canopen::ICANInterface {
	private:
		int32_t m_deviceID;
		
		bool m_InitFlag;
		int m_Fd;
		std::string m_FileName;
		struct sockaddr_can can_addr;

	public:

		/**
		 * @if jp
		 * @brief コンストラクタ
		 * @param fileName CANインターフェースのファイル名．この引数はWindowsでは無視されます．
		 * @else
		 * @brief Constructor
		 * @param fileName File name of CAN interface. This parameter will be ignored in Windows OS.
		 * @endif
		 */
		SocketCAN(const char *fileName);


		/**
		 * @if jp
		 * @brief デストラクタ
		 * @else
		 * @brief Destructor
		 * @endif
		 */
		virtual ~SocketCAN(void);

	public:

		/**
		 * @brief 初期化関数
		 * @param ボーレート
		 * @see ICAN_BAUD
		 */
		virtual void initialize(const ICAN_BAUD baudrate) throw(canopen::CANException);


		/**
		 * @brief 終了処理関数
		 */
		virtual void uninitialize();

		/**
		 * @if jp
		 * @brief CANメッセージのバッファからの取り出し
		 * @param msg [OUT] メッセージを格納するCANMSG構造体
		 * @return データ受信成功ならばtrue．失敗ならばfalse
		 * @else
		 * @brief CAN message receive from Buffer
		 * 
		 * @param msg [OUT] CANMSG class object 
		 * @return True if data is successfully read.
		 * @endif
		 */
		virtual bool receive(canopen::CANMSG* msg);

		/**
		 * @if jp
		 * @brief CANメッセージの送信
		 * @param msg [IN] メッセージを格納するCANMSG構造体
		 * @else
		 * @endif
		 */
		virtual void send(const canopen::CANMSG& msg);

	 };

}

#endif //#ifndef SOCKETCAN_HEADER_INCLUDED
