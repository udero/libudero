/** 
 * defines.h
 *
 * ���C�u�����G�N�X�|�[�g�Ȃǂ̐ݒ���{�I�ȕϐ��̒�`
 *
 * @author Yuki Suga
 * @copyright Technotools, 2015
 */

#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>


  // Windows�p�̊֐��G�N�X�|�[�g�錾
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
   * UderoResult_t �^�Ŏg���l
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
   * C����o�C���f�B���O�p��Udero����n���h��
   */
  typedef int32_t Udero_t;

  /**
   * C����o�C���f�B���O�p��UderoJoint�̐���n���h��
   */
  typedef uint8_t UderoJointID;

  /** 
   * libudero�Ŏg�������^�̒�`
   */
  typedef double UderoREAL;


#ifdef __cplusplus
}
#endif

