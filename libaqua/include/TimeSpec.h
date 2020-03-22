/**
 * @file Timer.h 
 * @author Yuki Suga (ysuga.net), Yuki Nakagawa (RT Corp.), Motomasa Tanaka (Mayekawa MFC Co., Ltd.)
 * @copyright RT Corp. 2012 All rights reserved.
 */

#pragma once
#include "aqua.h"
#include "type.h"

/**
 * @if jp
 * @brief PCWrapperライブラリ用の名前空間
 * @else
 * @brief Namespace for Aqua Library
 * @endif
 */
namespace ssr {

	/**
	 * @if jp
	 * @class TimeSpec
	 * @brief このクラスは時間を表すために使用される
	 * 
	 * @else
	 * @class TimeSpec
	 *
	 * @endif
	 */
	struct TimeSpec {
	private:

	public:
		uint32_t sec; //< Second
		uint32_t usec; //< Micro Second

	public:
		uint32_t getUsec() {
		  return sec * 1000000 + usec;
		}
	public:
		
		/**
		 * @if jp
		 * @brief コンストラクタ
		 * @else
		 * @brief Constructor
		 * @endif
		 */
		TimeSpec() {
			sec = usec = 0;
		}

		/**
		 * @if jp
		 * @brief コンストラクタ
		 * @param Sec 秒
		 * @param Usec マイクロ秒
		 * @else
		 * @brief Constructor
		 * @endif
		 */
		TimeSpec(const uint32_t Sec, const uint32_t Usec) {
			sec = Sec;
			usec = Usec;
		}

		TimeSpec(const uint32_t Usec) {
			sec = Usec / (1000*1000);
			usec = Usec % (1000*1000);
		}

		/**
		 * @if jp
		 * @brief 比較演算子 ==
		 * @else
		 * @brief 
		 * @endif
		 */
		bool operator==(const TimeSpec& timeSpec) const {
			if(this->sec == timeSpec.sec && this->usec == timeSpec.usec) {
				return true;
			}
			return false;
		}

		/**
		 * @if jp
		 * @brief 比較演算子 !=
		 * @else
		 * @brief 
		 * @endif
		 */
		bool operator!=(const TimeSpec& timeSpec) const {
			return !( operator==(timeSpec));
		}


		/**
		 * @if jp
		 * @brief 比較演算子 >
		 * @else
		 * @brief Larger Than Operator
		 * @endif
		 */
		bool operator>(const TimeSpec& timeSpec) const {
			if(this->sec > timeSpec.sec) {
				return true;
			} else if(this->sec == timeSpec.sec) {
				if(this->usec > timeSpec.usec) {
					return true;
				}
			}
			return false;
		}

		/**
		 * @if jp
		 * @brief 比較演算子 <
		 * @else
		 * @brief Less Than Operator
		 * @endif
		 */
		bool operator<(const TimeSpec& timeSpec) const {
			if(this->sec < timeSpec.sec) {
				return true;
			} else if(this->sec == timeSpec.sec) {
				if(this->usec < timeSpec.usec) {
					return true;
				}
			}
			return false;
		}

		/**
		 * @if jp
		 * @brief 比較演算子 >=
		 * @else
		 * @brief Larger Than or Equal To operator
		 * @endif
		 */
		bool operator>=(const TimeSpec& timeSpec) const {
			if(this->operator>(timeSpec)) {
				return true;
			} else if(this->operator==(timeSpec)) {
				return true;
			}
			return false;
		}

		/**
		 * @if jp
		 * @brief 比較演算子 <=
		 * @else
		 * @brief Less Than or Equal To Operator
		 * @endif
		 */
		bool operator<=(const TimeSpec& timeSpec) const {
			if(this->operator<(timeSpec)) {
				return true;
			} else if(this->operator==(timeSpec)) {
				return true;
			}
			return false;
		}
	};


}








/**
 * @brief Global Object for ZERO time.
 */
static const ssr::TimeSpec ZEROTIME(0, 0);


/**
 * @brief Global Object for ZERO time.
 */
static const ssr::TimeSpec INFINITETIME(0xFFFFFFFF, 0xFFFFFFFF);


