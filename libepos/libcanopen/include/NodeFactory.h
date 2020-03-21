/**
 * @file NodeFactory.h
 * @copyright RT Corp. 2012 All rights reserved.
 */

#ifndef NODE_FACTORY_HEADER_INCLUDED
#define NODE_FACTORY_HEADER_INCLUDED

#include "type.h"
#include "Node.h"

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {

	/**
	 * @brief Interface for Node Factory
	 */
	class INodeFactory {

	public:

		/**
		 * @brief Create Node 
		 */
		virtual Node* createNode(CANopen* pCANopen, const uint8_t nodeId) = 0;

		/**
		 * @brief Get Corresponding Vendor ID. This function is used to specify the appropreated factory
		 */
		virtual uint32_t getVendorId() const = 0;

		/**
		 * @brief Get Corresponding Product Code. This function is used to specify the appropreated factory
		 */
		virtual uint32_t getProductCode() const = 0;

		/**
		 * @brief Get Corresponding Product Code Mask. This function is used to specify the appropreated factory
		 */
		virtual uint32_t getProductCodeMask() const = 0;
	};
}

#endif