
#include <ctime>
#include <stdarg.h>

#include "CANopen.h"
#include "udero/UderoLogger.h"
#include "UderoImpl.h"
#include "UderoCANJoint.h"

#ifdef WIN32
#include "PCANUSB.h"
#else
#include "SocketCAN.h"
#endif

#include "global_parameter.h"
#include "can_param.h"


using namespace reharo;

class UderoCAN : public UderoImpl {
private:
  canopen::ICANInterface *m_pCAN;
  canopen::CANopen* m_pCANopen;


public:
  canopen::ICANInterface* getCAN() { return m_pCAN; }
  canopen::CANopen* getCANopen() { return m_pCANopen; }
public:
  UderoCAN(canopen::ICANInterface* pCAN, const std::string& settingFilename) : UderoImpl(settingFilename), m_pCAN(pCAN) {
    try {
      UTRACE("UderoCAN::UderoCAN(");
      m_pCANopen = new canopen::CANopen(m_pCAN);
      m_pCANopen->registerNodeFactory(new maxon::EPOSFactory());
      numJoints = 7;
      for (int i = 0; i < numJoints; i++) {
	UVERBOSE("initializing Joint[%d]", i);
	ppJoints[i] = new UderoCANJoint(i + 1, 
					m_pCANopen,
					g_gear_ratio[i], encoder_res[i], 
					angle_min_max[i][0], angle_min_max[i][1]);
      }
      ppJoints[0]->setPolar(-1);
      ppJoints[1]->setPolar(-1);
      ppJoints[3]->setPolar(-1);
      ppJoints[5]->setPolar(-1);
      for (int i = 0;i < 6;i++) {
	jointPos_.push_back(0);
      }
    } catch (std::exception& ex) {
      std::cerr << "Exception when UderoCAN::UderoCAN()" << std::endl;
      std::cerr << ex.what() << std::endl;
      throw ex;
    }
  }

  virtual ~UderoCAN() {}

  virtual std::string getClassName() const {
    return "UderoCAN";
  }

  virtual void reset() {
    UTRACE("UderoCAN::reset()");
    m_pCAN->uninitialize();
    m_pCAN->initialize(ICAN_BAUD_1M);
    init();
    UTRACE("UderoCAN::reset() ends");
  }
};


IUdero* createCANUdero(const reharo::UderoConnectionProfile& profile) {
  UINFO("createCANUdero called");
  UINFO(" - canFilename    = %s", profile.canFilename.c_str());
  UINFO(" - canDeviceID    = %x", profile.canDeviceID);
  UINFO(" - settingFilename= %s", profile.settingFilename.c_str());

#ifdef WIN32
  UTRACE("peaksystem::CANUSB creating...");
  peaksystem::PCANUSB* pCAN = new peaksystem::PCANUSB(profile.canDeviceID);
  UTRACE("peaksystem::CANUSB created");
#else 
  UTRACE("ssr::SocketCAN creating with(%s)...", profile.canFilename.c_str());
  ssr::SocketCAN* pCAN = new ssr::SocketCAN(profile.canFilename.c_str());
  UTRACE("ssr::SocketCAN created");
#endif  

  UTRACE("ICANInterface initializing...");
  pCAN->initialize(ICAN_BAUD_1M);
  UTRACE("ICANInterface initialized");
  UTRACE("reharo::UderoCAN creating");
  UderoCAN* udero = new UderoCAN(pCAN, profile.settingFilename);
  UTRACE("reharo::UderoCAN created");
  return udero;
}



canopen::ICANInterface* getCAN(reharo::IUdero* udero) {
  UderoCAN* uderoCAN = dynamic_cast<UderoCAN*>(udero);
  if (!uderoCAN) return NULL;
  return uderoCAN->getCAN();
}

canopen::CANopen* getCANopen(reharo::IUdero* udero) {
  UderoCAN* uderoCAN = dynamic_cast<UderoCAN*>(udero);
  if (!uderoCAN) return NULL;
  return uderoCAN->getCANopen();
}
