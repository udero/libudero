#include <sstream>
//#include "logger.h"
#include "udero/UderoLogger.h"
#include "discrete_key_control.h"

#include "SerialPort.h"
//static const char* default_port_name = "/dev/cu.SLAB_USBtoUART";
#ifdef WIN32
static const char* default_port_name = "COM7";
#else
static const char* default_port_name = "/dev/ttyUSB0";
#endif

static const char* default_baudrate = "115200";
static ssr::SerialPort *pSerial;

bool initKeyInput(ParamLoader* loader) {
  std::string portName = loader->get("KEYPAD_PORT_NAME", default_port_name);
  std::string baudRate = loader->get("KEYPAD_BAUDRATE", default_baudrate);

  try {

    UINFO("Initializing Input with (port=%s, baud=%s)", portName.c_str(), baudRate.c_str());
    pSerial = new ssr::SerialPort(portName.c_str(), atoi(baudRate.c_str()));
    return true;
  } catch (std::exception& ex) {
    UERROR("%s", ex.what());
    UERROR("Can not find SerialPort(%s)", portName.c_str());
    pSerial = NULL;
  }
  return false;
}

void finiKeyInput() {
  delete pSerial;
}

void writeChar(ssr::SerialPort* s, uint8_t c) {
  pSerial->Write(&c, 1);
}

std::string getLine(ssr::SerialPort* s) {
  std::stringstream ss;
  while(true) {
    if (s->GetSizeInRxBuffer() > 0) {
      uint8_t c;
      s->Read(&c, 1);
      if (c == '\n') {
	break;
      }
      ss << c;
    }
  }
  return ss.str();
}

KeyContext checkKeyInput(const KeyContext& ctx) {
  KeyContext newCtx = ctx;
  try {
    writeChar(pSerial, 'R');
    std::string v = getLine(pSerial);
    if (v.length() > 0) {
      int k =  v.c_str()[0];
      UVERBOSE("getch = %c(0x%02X)", (char)k, k);
      newCtx.keyReleasedCount = 0;
      newCtx.old_key = ctx.new_key;
      newCtx.new_key = k;
    } else {
      if (ctx.new_key != 0) {
	newCtx.keyReleasedCount = ctx.keyReleasedCount + 1;
	newCtx.old_key = ctx.new_key;
	newCtx.new_key = ctx.new_key;
	if (newCtx.keyReleasedCount >= keyReleasedCountThreshold) {
	  newCtx.new_key = 0;
	  newCtx.old_key = ctx.new_key;
	}
      } else {
	newCtx.new_key = 0;
	newCtx.old_key = 0;
	newCtx.keyReleasedCount = 0;
      }
    }
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return ctx;
  }
  return newCtx;
}

