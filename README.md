
# libudero 

�e�N�m�c�[���А����{�b�g�A�[������p���C�u����

## �R�[�h����̃r���h���@

CMake���g���܂��BCMake�̓o�[�W����3.10�ȍ~���g���ĉ������B

### Windows 10�ł̒��ӓ_
#### PeakSystem PCAN-USB�h���C�o�̃C���X�g�[��
Udero�ɕt�����Ă���PCAN-USB�̃h���C�o���C���X�g�[�����āAWindows�V�X�e�����ϊ��P�[�u����F�����Ă��邱�Ƃ��m�F���Ă��������B

#### PCAN-BASIC API�̃C���X�g�[��
PCAN-BASIC API�Ɉˑ����Ă��܂��BPCAN BASIC API��PeakSystem�Ђ��񋟂���CAN-USB�ϊ��A�_�v�^�p�̃��C�u�����ł��B
Peak Systems�Ђ̃E�F�u�T�C�g����_�E�����[�h���Ă��������B
https://www.peak-system.com/
�_�E�����[�h����zip��W�J���čD���ȏꏊ�ɔz�u���Ă��������B
���̈ʒu�����CMake�̐ݒ�ɔ��f������K�v������܂��B
�܂��A�W�J�����t�H���_�ɂ���ReadMe.txt��ǂ�ŁADLL�t�@�C����System32�f�B���N�g���Ȃǂɔz�u���Ă��������B

#### CMake�ł̐ݒ�
CMake�Ńv���W�F�N�g�𐶐�����Ƃ��ɏ�L�̂悤��PCAN BASIC API�̃f�B���N�g����ݒ肷��K�v������܂��B
CMake-gui��Configure�����̂��ɁAPCANBASIC_ROOT�̒l���APCAN-BASIC API�t�H���_�ɐݒ肵�Ă���AGenerate���s���Ă��������B


#### Visual Studio�ł̃r���h
CMake�Őݒ肵���o�[�W������Visual Studio�Ńr���h���Ă��������B�J���҂�Visual Studio 2019 (x64) �ł̓���m�F���s���Ă��܂��B

## �e�X�g�p�A�v��
apps�f�B���N�g���ɂ́AUdero�𗘗p���邽�߂̃R���p�N�g�ȃR�}���h�Q���쐬���邽�߂̃\�[�X�R�[�h������܂��B
�����̃R�[�h�́Alibudero���C�u�������g�����߂̃T���v���ɂ��Ȃ��Ă��܂��B�R�[�h���m�F���Ďg���Ă��������B
��Ȃ��͈̂ȉ��̒ʂ�ł��B

- close_gripper �O���b�p�[����܂�
- digital_input �֐߂̃f�W�^�����͂���ʂɕ\��
- foldin �܂肽���ݏ�ԂɈړ�
- foldout �܂肽���ݏ�Ԃ���W�J��ԁA�������͔C�ӂ̎p������W�J�p���Ɉړ�
- free �S�֐߂��T�[�{OFF
- get_angles �֐ߊp�x���擾
- homing �z�[�~���O���Č��_������o��
- is_homed �z�[�~���O���I����Ă��邩���m�F
- joint_get_state ���݂̊֐߂̏�Ԃ��o��
- joint_set_state �֐߂̏�Ԃ�ύX
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

�܂��APCAN-USB�̃h���C�o�A�����PCAN-BASIC��DLL�t�@�C�����܂܂�Ă��܂���B
�e�X���_�E�����[�h���ăC���X�g�[�����s���Ă��������B

## �f�B���N�g���̍\��

- libuder
  - apps �e�X�g�p�̃A�v���Q�̃\�[�X�R�[�h���������f�B���N�g��
  - include udero���C�u�����̃w�b�_�[�t�@�C��
  - libaqua Thread��Sleep�Ȃǂ̃v���b�g�t�H�[�����𖄂߂邽�߂̃��C�u����
  - libepos ���[�^�[�h���C�o���g�����߂̃��C�u����
  - libpcanbasic PCAN-USB�P�[�u�����g�����߂̃��C�u�����BWindows��p
  - libsocketcan PCAN-USB�P�[�u����Linux�Ŏg���ꍇ�̃��C�u�����B
  - src Udero�̃��C�u����
  - thirdpary �T�[�h�p�[�e�B�[���̃��C�u�����̓������f�B���N�g��
  - CMakeLists.txt
  - README.md ���̃t�@�C���ł��B



## Authors

* **Yuki Suga** - *SUGAR SWEET ROBOTICS* -

## License

This project is Proprietary license.
This code is allowed to use with Udero (built by Technotools Corpolation.).
This code is NOT allowed to use not with Udero.

�{�R�[�h�̈ꕔ����т��ׂĂ��e�N�m�c�[���А�Udero�ȊO�̂��̂Ǝg�����Ƃ��ւ���B

## �Ɛӎ���
�{�\�[�X�R�[�h��p�������ʁA���[�U������������Ȃ鑹�Q�ɂ��Ă��A��҂����Technotools�͈�؂̐ӔC�𕉂��܂���B���Ȃ̐ӔC�̏�Ŏg�p���ĉ������B

## ���쌠
�{�\�[�X�R�[�h�̒��쌠�́A��҂ł���Technotools�ɋA�����܂��B