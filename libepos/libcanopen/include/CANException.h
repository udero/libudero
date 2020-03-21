/**
 * @file CANException.h
 * @copyright RT Corp. 2012. Allrights Reserved.
 */
#ifndef CAN_EXCEPTION_HEADER_INCLUDED
#define CAN_EXCEPTION_HEADER_INCLUDED

#include <string>
#include <exception>
//#include <strstream>
#include <sstream>


/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {


	/**
	 * Exception class for CAN Interface errors.
	 */
	class CANException : public std::exception {
	protected: std::string msg;
	public:
		CANException(std::string msg) { this->msg = msg; }
		virtual ~CANException() throw () {} 
	public:
		virtual const char* what() const throw() {return msg.c_str();}
	};

	/**
	 * @brief Time Out Exception
	 */
	class TimeOutException : public CANException {
	public: TimeOutException(const char* string) : CANException(std::string("Time Out:") + string) {}
	};


	/**
	 * @brief SDO Aborted Exception
	 */
	class SDOAbortedException : public CANException {
	private:
		uint32_t m_ErrorCode;

		void addErrorCode(const uint32_t errorCode) {
			m_ErrorCode = errorCode;
			char ch[64];
			std::ostringstream strst;//(ch, 64);
			strst << "(Abort Code = " << std::hex << std::uppercase << m_ErrorCode << ")" << std::ends;
			this->msg = this->msg + std::string(ch);
		}

	public: 
		SDOAbortedException(std::string msg, const uint32_t errorCode) : CANException(msg) {
			addErrorCode(errorCode);
		}

		/**
		 * @brief Constructor
		 */
		SDOAbortedException(const uint32_t errorCode) : CANException("SDO Aborted"){
			addErrorCode(errorCode);
		}

		virtual ~SDOAbortedException() throw() {}

		/**
		 * @brief Get Abort Error Code
		 * @return Abort Error Code defined in CANopen protocol.
		 */
		uint32_t getErrorCode() { return m_ErrorCode; }

	};

	/**
	 * @class RangeException
	 */
	class RangeException : public SDOAbortedException {
	public: RangeException(const uint32_t errorCode) : SDOAbortedException("Range Error", errorCode) {}
	};


}

#endif // #ifndef CAN_EXCEPTION_HEADER_INCLUDED

