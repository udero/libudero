/**
 * @file  CANMSG.h
 * @copyright RT Corp. 2012. Allrights Reserved.
 */
#ifndef CANMSG_HEADER_INCLUDED
#define CANMSG_HEADER_INCLUDED

#include "type.h"

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {

	/**
	 * @if jp
	 * @brief CANメッセージパケット
	 * @else
	 * @brief CAN message packet definition
	 * @endif
	 */
	struct CANMSG {
		uint32_t id;      ///< Node ID
		uint8_t  rtr;     ///< RTR flag
		uint8_t  exaddr;  ///< Not implemented
		uint8_t  length;  ///< Length of Data field
		uint8_t  data[8]; ///< Data field (maximum 8 bytes)
	};
}


#endif // #ifndef CANMSG_HEADER_INCLUDED

