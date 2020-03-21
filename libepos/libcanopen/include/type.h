/**
 * @file type.h
 * @mail staff@rt-net.jp
 * @url http://rt-net.jp
 * @copyright RT Corp. 2012. Allrights Reserved.
 */

#ifndef TYPE_HEADER_INCLUDED
#define TYPE_HEADER_INCLUDED


/**
 * @brief DLLエクスポートの設定
 */
#ifdef WIN32                          // Windows用の設定
#ifdef _USRDLL                        // DLL側コンパイルの場合
#define DLL_API __declspec(dllexport)
#else                                 // ユーザ側コンパイルの設定
#ifdef _STATIC_EXPORTS               
#define DLL_API                       // スタティックライブラリ用                  
#else
#define DLL_API __declspec(dllimport) // ダイナミックリンク用
#endif 
#endif
#else 
#define DLL_API                       // Linux用
#endif


/**
 * setting for stdint.h
 */
#if defined (_MSC_VER) && (_MSC_VER == 1500) // VC2008用
#ifndef _MSC_STDINT_H_
//#error "Defined!"
typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;
typedef int int32_t;
typedef unsigned int uint32_t;
typedef long int64_t;
typedef unsigned long uint64_t;
#endif
#else
#include <stdint.h> // VC2010もしくはLinux用
#endif



#endif // #ifndef TYPE_HEADER_INCLUDED

