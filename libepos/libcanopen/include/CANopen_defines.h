/**
 * @file CANopen_defines.h
 * @copyright RT Corp. 2012 All rights reserved.
 */
#ifndef CANOPEN_DEFINES_HEADER_INCLUDED
#define CANOPEN_DEFINES_HEADER_INCLUDED


#define CAN_VERSION_STRING "0.0.1"
/**
 * @brief DLLエクスポートの設定
 */
#ifdef WIN32                          // Windows用の設定
#ifdef canopen_EXPORTS                        // DLL側コンパイルの場合
#define CANOPEN_API __declspec(dllexport)
#else                                 // ユーザ側コンパイルの設定
#ifdef EXPORT_canopen_STATIC               
#define CANOPEN_API   // スタティックライブラリ用                  
#else
#ifdef USE_canopen_STATIC
#define CANOPEN_API
#else
#define CANOPEN_API __declspec(dllimport) // ダイナミックリンク用
#endif
#endif 
#endif
#else 
#define CANOPEN_API                       // Linux用
#endif



#include "type.h"
#include "DefaultObjectDictionary_defines.h"

// Definitions for SDO control communication

enum SDO_FUNCTION_CODE {
	SDO_FUNC_UPLOAD = 0x40,
	SDO_FUNC_DOWNLOAD = 0x20,
	SDO_FUNC_ABORTED = 0x80,
};

enum SDO_TRANSFER_TYPE {
	SDO_EXPEDITED_TRANSFER = 0x02,
	SDO_NORMAL_TRANSFER = 0x00,
};

enum SDO_SIZE_INDICATOR {
	SDO_SIZE_INDICATED = 0x01,
	SDO_SIZE_NOT_INDICATED = 0x00,
};

inline uint8_t SDO_SIZE(uint8_t size) {
	return (4-size) << 2;
}

// Definitions for PDO control communication
// PDO通信に関する定義

/**
 * PDO通信に関するCOBIDのオフセット値
 */
enum COBID_OFFSET {
	COBID_TXPDO1_OFFSET = 0x180,
	COBID_TXPDO2_OFFSET = 0x280,
	COBID_TXPDO3_OFFSET = 0x380,
	COBID_TXPDO4_OFFSET = 0x480,
	COBID_RXPDO1_OFFSET = 0x200,
	COBID_RXPDO2_OFFSET = 0x300,
	COBID_RXPDO3_OFFSET = 0x400,
	COBID_RXPDO4_OFFSET = 0x500,
	COBID_TXSDO_OFFSET  = 0x600,
	COBID_RXSDO_OFFSET  = 0x580,
	COBID_HEARTBEAT_OFFSET = 0x700,
};

	
enum PDO_INDEX {
	PDO1,
	PDO2,
	PDO3,
	PDO4,
};

/**
 * PDO通信に関わる状態遷移メッセージ
 */
enum NMT_MESSAGE { 
	ENTER_PREOPERATIONAL = 0x80,
	RESET_COMMUNICATION = 0x82,
	RESET_NODE = 0x81,
	START_REMOTE_NODE = 0x01,
	STOP_REMOTE_NODE = 0x02,
};

/**
 * ハートビートに関わる定義
 */
enum NMT_STATE {
	NMT_BOOTUP  = 0x00,
	NMT_STOPPED = 0x04,
	NMT_OPERATIONAL = 0x05,
	NMT_PREOPERATIONAL = 0x7F,
};

/*
enum NMT_SLAVE_STATE {
	Operational = 0x01,
	Stopped = 0x02,
	PreOperational = 0x80,
	ResetNode = 0x81,
	ResetCommunication = 0x82,
};
*/

enum SDO_ABORT_CODE {
	SDO_ABORT_NOCOMMUNICATION           = 0x00000000,
	SDO_ABORT_TOGGLE                    = 0x05030000,
	SDO_ABORT_TIMEOUT                   = 0x05040000,
	SDO_ABORT_CLIENTSERVERSPECIFIER     = 0x05040001,
	SDO_ABORT_OUTOFMEMORY               = 0x05040005,
	SDO_ABORT_ACCESS                    = 0x06010000,
	SDO_ABORT_WRITEONLY                 = 0x06010001,
	SDO_ABORT_READONLY                  = 0x06010002,
	SDO_ABORT_OBJECTDOENNOTEXIST        = 0x06020000,
	SDO_ABORT_PDOMAPPING                = 0x06040041,
	SDO_ABORT_PDOLENGTH                 = 0x06040042,
	SDO_ABORT_GENERALPARAMETER          = 0x06040043,
	SDO_ABORT_GENERALINTERNALINCOMPATIBILITY = 0x06040047,
	SDO_ABORT_HARDWARE                  = 0x06060000,
	SDO_ABORT_SERVICEPARAMETER          = 0x06070010,
	SDO_ABORT_SERVICEPARAMETERTOOLONG   = 0x06070012,
	SDO_ABORT_SERVICEPARAMETERTOOSHORT  = 0x06070013,
	SDO_ABORT_OBJECTSUBINDEX            = 0x06090011,
	SDO_ABORT_VALUERANGE                = 0x06090030,
	SDO_ABORT_VALUETOOHIGH              = 0x06090031,
	SDO_ABORT_VALUETOOLOW               = 0x06090032,
	SDO_ABORT_MAXIMUMLESSMINIMUM        = 0x06090036,
	SDO_ABORT_GENERAL                   = 0x08000000,
	SDO_ABORT_TRANSFERORSTOREERROR      = 0x08000020,
	SDO_ABORT_LOCALCONTROL              = 0x08000021,
	SDO_ABORT_WRONGDEVICESTATE          = 0x08000022,
	SDO_ABORT_WRONGNMTSTATE             = 0x0F00FFC0,
	SDO_ABORT_ILLEGALCOMMAND            = 0x0F00FFBF,
	SDO_ABORT_PASSWORD                  = 0x0F00FFBE,
	SDO_ABORT_ERRORSERVICEMODE          = 0x0F00FFBC,
	SDO_ABORT_ERRORCANID                = 0x0F00FFB9,
};


#endif