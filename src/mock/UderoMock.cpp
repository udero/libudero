
#include <ctime>
#include <stdarg.h>

#include "CANopen.h"

#include "UderoImpl.h"
#include "UderoMockJoint.h"

#include "global_parameter.h"
#include "can_param.h"
#include "udero/UderoLogger.h"

using namespace reharo;

class UderoMock : public UderoImpl {
public:
  UderoMock(const std::string& filename) : UderoImpl(filename) {
    UTRACE("UderoMock::UderoMock() called");
    numJoints = 7;
    for (int i = 0; i < numJoints; i++) {
		jointPos_.push_back(0);
		ppJoints[i] = new UderoMockJoint(i+1, g_gear_ratio[i], encoder_res[i], angle_min_max[i][0], angle_min_max[i][1]);
    }
    UTRACE("UderoMock::UderoMock() ended");
  }

  virtual ~UderoMock() {}
};


IUdero* createMockUdero(const reharo::UderoConnectionProfile& profile) {
  UINFO("createMockUdero() called");
  UTRACE("reharo::UderoMock creating");
  IUdero* udero = new UderoMock(profile.settingFilename);
  UTRACE("reharo::UderoMock created");
  return udero;
}

