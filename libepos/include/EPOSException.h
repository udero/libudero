/**
 * @file EPOSException.h
 * @copyright RT Corp. 2012. Allrights Reserved.
 */

#pragma once


#include <string>
#include <exception>
#include "CANException.h"

namespace maxon {

	/**
	 * @brief Exception class for EPOS errors.
	 */
	class EPOSException : public canopen::CANException {
	private:
		const int m_ErrorCode;
	public:
		EPOSException(std::string msg, const int errorCode = -1) : canopen::CANException(msg), m_ErrorCode(errorCode) {
		}

		virtual int getErrorCode() const {return m_ErrorCode;}
		virtual ~EPOSException() throw() { }

		virtual const char* what() const throw() {
			std::string out = canopen::CANException::what();
			out += "(Code = ";
			out += m_ErrorCode;
			out += ")";
			return out.c_str();
		}
	};

	/**
	 * @brief EPOS is in Fault State.
	 * To recover from fault state, call reset function. 
	 */
	class FaultException : public EPOSException {
	public:
		FaultException() : EPOSException("Fault") {}
	};

	/**
	 * @brief EPOS is in Homing Error
	 * To recover from fault state, call reset function. 
	 */
	class HomingException : public EPOSException {
	public:
		HomingException() : EPOSException("Homing Error") {}
	};


}
