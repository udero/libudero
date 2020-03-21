/**
 * @file CANMSGBuffer.h
 * @copyright RT Corp. 2012. Allrights Reserved.
 */


#ifndef CANMSG_BUFFER_HEADER_INCLUDED
#define CANMSG_BUFFER_HEADER_INCLUDED

//#include "Mutex.h"
#include "Timer.h"
#include "CANMSG.h"

#include <string.h>

/**
 * @if jp
 * @brief CANopenライブラリ用の名前空間
 * @else
 * @brief Namespace for CANopen Library
 * @endif
 */
namespace canopen {

	/**
	 * @class CANMSGBuffer
	 * @brief CANMSG buffer class.
	 */
	class CANMSGBuffer {
	private:
		CANMSG m_Msg;
		bool updated;
		ssr::Mutex m_Mutex;
		ssr::Timer m_Timer;

	public:

		/**
		 * @brief Constructor
		 */
		CANMSGBuffer() {
			updated = false;
			memset(&m_Msg, 0, sizeof(m_Msg));
		}

		/**
		 * @brief Destructor
		 */
		virtual ~CANMSGBuffer() {
		}

	public:

		/** 
		 * @brief Push CANMSG object to buffer. This function set "updated" flag.
		 * @param msg CANMSG to be pushed.
		 */
		void pushMessage(const CANMSG& msg) {
			ssr::MutexBinder binder(m_Mutex);
			updated = true;
			m_Msg = msg;
		}

		/**
		 * @brief Pop CANMSG object from buffer. This function clear "updated" flag.
		 * This function also check the updated flag. 
		 * If updated flag is not set, control will stop until other thread call pushMessage function
		 * @param msg [OUT] CANMSG object
		 * @param timeOut timeout object.
		 */
		bool popMessage(CANMSG* msg, const ssr::TimeSpec& timeOut = INFINITETIME) {
			m_Timer.tick();
			ssr::TimeSpec currentTime;
			while(true) {
				ssr::MutexBinder binder(m_Mutex);
				//m_Mutex.Lock();
				if(updated) {
					//m_Mutex.Unlock();
					break;
				}
				//m_Mutex.Unlock();
				m_Timer.tack(&currentTime);
				if(timeOut < currentTime) {
					return false;
				}
				//ssr::Thread::yield();
				ssr::Thread::Sleep(0);
			}

			{
				ssr::MutexBinder binder(m_Mutex);
				updated = false;
				*msg = m_Msg;
				return true;
			}
		}

		/**
		 * @brief Just get CANMSG from Buffer.
		 * This function does not check "updated" flag.
		 */
		CANMSG getCurrentMessage() {
			return m_Msg;
		}
	};

}

#endif // #ifndef CANMSG_BUFFER_HEADER_INCLUDED
