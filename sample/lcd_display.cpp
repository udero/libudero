#include "lcd_display.h"

ssr::SerialPort* initLCD(ParamLoader* loader) {
  try {
    std::string portName = loader->get("LCD_PORT", "COM1");
    UTRACE("initLCD called");
    pSerial = new ssr::SerialPort(portName.c_str(), 57600);
    updateLCD(pSerial);
  }
  catch (std::exception& ex) {
    UERROR("LCD COM Port (%s) can not open.", portName.c_str());
    pSerial = NULL;
    UERROR("LCD will be ignored.");
  }
  return pSerial;
  UTRACE("initLCD exit");
}

static char modeMsg[4] = { 'M', ':', '0', 0x08 };
static char spdMsg[4] = { 'S', ':', '0', 0x08 };
static CONTROL_MODE oldControlMode = CONTROL_UNKNOWN;

void updateLCD(ssr::SerialPort* pSerial, CONTROL_MODE controlMode) {
  UTRACE("updateLCD called");
  bool sent = false;
  if (pSerial) {
    if (controlMode != oldControlMode) {
      if (controlMode == CONTROL_START) {
	modeMsg[2] = '0';
      }
      else if (controlMode == CONTROL_CARTESIAN) {
	modeMsg[2] = '1';
      }
      else {
	modeMsg[2] = '2';
      }
      pSerial->Write(modeMsg, 4);
      oldControlMode = controlMode;
      sent = true;
    }
  }
  UTRACE("updateLCD exit");
}
