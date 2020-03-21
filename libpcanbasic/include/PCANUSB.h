/**
 * @file PCANUSB.h
 */

#ifndef PCANUSB_HEADER_INCLUDED
#define PCANUSB_HEADER_INCLUDED

#include "ICANInterface.h"

#ifdef WIN32
#ifdef pcanusbbasic_EXPORTS
#define LIBPCANUSB_API __declspec(dllexport)
#else
#ifdef EXPORT_pcanusbbasic_STATIC
#define LIBPCANUSB_API
#else

#define LIBPCANUSB_API __declspec(dllimport)
#endif // LIBPCANUSB_STATIC_EXPORTS
#endif

#else // Linux
#define LIBPCANUSB_API 
#endif // ifdef WIN32


#ifdef WIN32

#else // Linux

#include "linux/can.h"
#include "linux/can/raw.h"
#include "linux/can/error.h"
#include <string>

#endif


/**
 * @if jp
 * @brief PEAK Systems用CANバスインターフェースの名前空間
 * 
 * @else
 * @brief
 * @endif
 */
namespace peaksystem {


	/**
	 * @if jp
	 * @brief PEAK SystemsのPCANUSB用CANInterfaceクラスの実装．
	 * @else
	 * @brief CANInterface implementation for PCANUSB from PEAK systems
	 * @endif
	 */
    class LIBPCANUSB_API PCANUSB : public canopen::ICANInterface {
	private:
		int32_t m_deviceID;
		
		bool m_InitFlag;
#ifdef WIN32

#else // For Linux
		int m_Fd;
		std::string m_FileName;
		struct sockaddr_can can_addr;
#endif
	public:

#ifndef WIN32
		/**
		 * @if jp
		 * @brief コンストラクタ
		 * @param fileName CANインターフェースのファイル名．この引数はWindowsでは無視されます．
		 * @else
		 * @brief Constructor
		 * @param fileName File name of CAN interface. This parameter will be ignored in Windows OS.
		 * @endif
		 */
		PCANUSB(const char *fileName);
#endif

		/**
		* @if jp
		* @brief コンストラクタ
		* @param fileName CANインターフェースのファイル名．この引数はWindowsでは無視されます．
		* @else
		* @brief Constructor
		* @param fileName File name of CAN interface. This parameter will be ignored in Windows OS.
		* @endif
		*/
		PCANUSB(const int32_t deviceID);

		/**
		 * @if jp
		 * @brief デストラクタ
		 * @else
		 * @brief Destructor
		 * @endif
		 */
		virtual ~PCANUSB(void);

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

#endif //#ifndef PCANUSB_HEADER_INCLUDED
