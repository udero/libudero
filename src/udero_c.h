
/**
* Udero.h
*
* @author Yuki Suga
* @copyright Techno Tool, 2015
*/

#pragma once

#include "libudero.h"

#ifdef __cplusplus
extern "C" {
#endif


#define UDERO_NUM_JOINT 8

	/**
	 * Udero�̏�����
	 * 
	 * @param can_net ESD��CAN-USB�̃h���C�o�ɂ�����net�ԍ��D�ʏ��0
	 * @param baudrate �{�[���[�g�D1000000 (1M) ���ʏ�̒l�D
	 * @param handle Udero_t�^�ϐ��ւ̃|�C���^�DUdero_t�^�ϐ����g���Ċ֐ߐ���Ȃǂ��s���̂ŕۑ����Ă�������
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_init(const uint32_t can_net, const uint32_t baudrate, Udero_t* handle);

	/**
	 * Udero�̏I��
	 * 
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_cleanup(const Udero_t handle);

	/**
	 * Udero�̊e�֐߂̖ڕW�p�x�ݒ�
     *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @param id �֐߂�ID�ԍ� [0, 6]
	 * @param position �ڕW�֐ߊp�x [rad]
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_setTargetJointPosition(const Udero_t handle, const UderoJointID id, const UderoREAL position);

	/** 
	 * Udero�̊e�֐߂̊p�x�擾
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @param id �֐߂�ID�ԍ� [0, 6]
	 * @param position �Ԑڊp�x���i�[���邽�߂�UderoREAL�^�ϐ��ւ̃|�C���^�D�P��rad�D
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_getActualJointPosition(const Udero_t handle, const UderoJointID id, UderoREAL* position);

	/**
	 * Udero�̏�Ԏ擾
	 * 
	 * ���̊֐������I�ɌĂ΂Ȃ���΂����Ȃ��D���̊֐����Ă΂���PC��Udero�ƒʐM���C�Ԑڊp�x���̏����ŐV���ɍX�V����D
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_update(const Udero_t handle);

	/**
	 * Udero��Inactive��ԂɕύX
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_deactivate(const Udero_t handle);

	/**
	 * Udero��PWM��ԂɕύX
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_activatePWM(const Udero_t handle);

	/**
	 * Udero��d�����䃂�[�h�ɕύX (���݂͎g���Ȃ�) 
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_activateCurrent(const Udero_t handle);

	/**
	 * Udero�𑬓x���䃂�[�h�ɕύX (���݂͎g���Ȃ�) 
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_activateVelocity(const Udero_t handle);

	/**
 	 * Udero���ʒu���䃂�[�h�ɕύX
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_activatePosition(const Udero_t handle);

	/**
	 * Udero�̃n���h���w��
	 * 
	 * �n���h��������J�n����DUdero_handStop�֐����Ă΂Ȃ���Ε����삪��~���Ȃ��̂Œ��ӂ��Ďg�p���邱��
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_handClose(const Udero_t handle);

	/**
	 * Udero�̃n���h�J������
	 *
	 * �n���h�̊J��������J�n����DUdero_handStop�֐����Ă΂Ȃ���ΊJ�����삪��~���Ȃ��̂Œ��ӂ��Ďg�p���邱�ƁD
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_handOpen(const Udero_t handle);

	/**
	 * Udero�̃n���h�J���E������̒�~
	 *
	 * @param handle Udero_t�^�ϐ��DUdero_init�֐��œ���ꂽ�l���g�����ƁD
	 * @return UderoResult_t�^�ϐ��DUdero_OK�Ȃ�ΐ����D
	 */
	LIBUDERO_API UderoResult_t Udero_handStop(const Udero_t handle);

#ifdef __cplusplus
}
#endif
