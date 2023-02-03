/** 
 * defines.h
 *
 * ライブラリエクスポートなどの設定や基本的な変数の定義
 *
 * @author Yuki Suga
 * @copyright Technotools, 2015
 */

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


  // Windows用の関数エクスポート宣言
#ifndef LIBUDEROI_API

#ifdef WIN32
#ifdef _USE_UDERO_STATIC
#define LIBUDERO_API
#else
#ifdef _WINDLL
#define LIBUDERO_API __declspec(dllexport)
#else // _WINDLL
#ifdef EXPORT_udero2_STATIC
#define LIBUDERO_API
#else
#define LIBUDERO_API __declspec(dllimport)
#endif EXPORT_udero2_STATIC

#endif // _WINDLL
#endif // _USE_UDERO_STATIC
#else // WIN32
#define LIBUDERO_API 
#endif // ifdef WIN32

#endif //LIBUDERO_API


  typedef int32_t UderoResult_t;

  /**
   * UderoResult_t 型で使う値
   */
  enum {
    Udero_OK = 0,
    Udero_FAILED = -1,
    Udero_INVALID_OBJ = -2,
    Udero_TIMEOUT = -3,
  };

  /**
   * 
   */
  typedef struct UderoSetting_type {
    uint32_t baudrate;
    uint32_t net;
  } UderoSetting;

  /** 
   * C言語バインディング用のUdero制御ハンドル
   */
  typedef int32_t Udero_t;

  /**
   * C言語バインディング用のUderoJointの制御ハンドル
   */
  typedef uint8_t UderoJointID;

  /** 
   * libuderoで使う実数型の定義
   */
  typedef double UderoREAL;


#ifdef __cplusplus
}
#endif

