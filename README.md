
# libudero 

�e�N�m�c�[���А����{�b�g�A�[������p���C�u����

## �R�[�h����̃r���h���@

CMake���g���܂��BCMake�̓o�[�W����3.10�ȍ~���g���ĉ������B

### Windows 10�ł̒��ӓ_
#### PCAN-BASIC API�̃C���X�g�[��
PCAN-BASIC API�Ɉˑ����Ă��܂��BPCAN BASIC API��PeakSystem�Ђ��񋟂���CAN-USB�ϊ��A�_�v�^�p�̃��C�u�����ł��B
Peak Systems�Ђ̃E�F�u�T�C�g����_�E�����[�h���Ă��������B
https://www.peak-system.com/
�_�E�����[�h����zip��W�J���čD���ȏꏊ�ɔz�u���Ă��������B
���̈ʒu�����CMake�̐ݒ�ɔ��f������K�v������܂��B
�܂��A�W�J�����t�H���_�ɂ���ReadMe.txt��ǂ�ŁADLL�t�@�C����z�u���Ă��������B

#### CMake�ł̐ݒ�
CMake�Ńv���W�F�N�g�𐶐�����Ƃ��ɏ�L�̂悤��PCAN BASIC API�̃f�B���N�g����ݒ肷��K�v������܂��B
CMake-gui��Configure�����̂��ɁAPCANBASIC_LIBDIR�̒l���APCAN-BASIC API�t�H���_�ɐݒ肵�Ă���A�ēxConfigure/Generate���s���Ă��������B


## �e�X�g�p�A�v��
apps�f�B���N�g���ɂ́AUdero�𗘗p���邽�߂̃R���p�N�g�ȃR�}���h�Q���쐬���邽�߂̃\�[�X�R�[�h������܂��B
�����̃R�[�h�́Alibudero���C�u�������g�����߂̃T���v���ɂ��Ȃ��Ă��܂��B�R�[�h���m�F���Ďg���Ă��������B
��Ȃ��͈̂ȉ��̒ʂ�ł��B

- close_gripper �O���b�p�[����܂�
- digital_input �֐߂̃f�W�^�����͂���ʂɕ\�����܂��B
- foldin �܂肽���ݏ�Ԃɂ��܂�
- foldout �܂肽���ݏ�Ԃ���W�J��ԁA�������͔C�ӂ̎p������W�J�p���Ɉړ����܂��B
- free �S�֐߂��T�[�{OFF���܂�
- get_angles �֐ߊp�x���擾���܂�
- homing �z�[�~���O���Č��_������o���܂��B
- is_homed �z�[�~���O���I����Ă��邩���m�F���܂�
- joint_get_state ���݂̊֐߂̏�Ԃ��o�͂��܂�
- joint_set_state �֐߂̏�Ԃ�ύX���܂��B
- joy_joint_ctrl �Q�[���p�b�h�Ŋe�֐߂𓮂����܂��B
- lock �S�֐߂��T�[�{ON���܂��B
- move_gripper �O���b�p�[��C�ӂ̈ʒu�܂œ������܂�
- open_gripper �O���b�p�[��S�J���܂��B
- set_angles �e�֐߂������̊p�x�܂ňړ������܂��B
- socketserver �\�P�b�g�ʐM�p�̃T�[�o�[�ł��B
- waitstopped ���{�b�g�̊֐߂��ڕW�l�ɓ��B���Ē�~����܂ő҂��܂��B

�����̃A�v����udero_{�f�B���N�g����} �Ƃ���exe�t�@�C���𐶐����܂��B

## �r���h�ς݃p�b�P�[�W
�ȉ��̃A�h���X����Windows�p�̃r���h�ς݃o�C�i��������ł��܂��B
Visual Studio���C���X�g�[�����Ă��Ȃ����œ��삳����ꍇ�́A�K�v�ɉ�����Visual Studio�̍ĔЕz�p���C�u�������C���X�g�[�����Ă��������B

https://github.com/udero/libudero_dist


## Authors

* **Yuki Suga** - *SUGAR SWEET ROBOTICS* -

## License

This project is Proprietary license.
This code is allowed to use with Udero (built by Technotools Corpolation.).
This code is NOT allowed to use not with Udero.

�{�R�[�h�̈ꕔ����т��ׂĂ��e�N�m�c�[���А�Udero�ȊO�̂��̂Ǝg�����Ƃ��ւ���B