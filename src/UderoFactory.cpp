#include <iostream>
#include <stdlib.h>

#include "udero/Udero.h"
#include "udero/UderoBase.h"
#include "udero/UderoLogger.h"
#include "can/UderoCAN.h"
#include "can/UderoBaseCAN.h"
//#include "socket/UderoSocket.h"
#include "mock/UderoMock.h"

//#ifndef DEBUG
// #define DEBUG
//#endif

void INIT_LOG(const char* filename, UDERO_LOGLEVEL level);
void FINI_LOG();

namespace technotools {

  inline std::string trim(const std::string& string, const char* removedChara = " \t\v\r\n") {
    std::string res;
    size_t left = string.find_first_not_of(removedChara);
    
    if (left != std::string::npos) {
      size_t right = string.find_last_not_of(removedChara);
      res = string.substr(left, right - left + 1);
    }
    
    return res;
  }


  typedef void (*CB_TYPE)(UderoConnectionProfile&);
  typedef void (*ARG_CB_TYPE)(UderoConnectionProfile&, std::string&);

  struct ArgOption {
    char s;
    std::string ss;
    std::string type;

    CB_TYPE cb;
    ARG_CB_TYPE argcb;

    ArgOption(char c, std::string cc, CB_TYPE cb): s(c), ss(cc), type("bool"), cb(cb) {}
    ArgOption(char c, std::string cc, std::string type, CB_TYPE cb): s(c), ss(cc), type(type), cb(cb) {}

    ArgOption(char c, std::string cc, ARG_CB_TYPE cb): s(c), ss(cc), type("bool"), argcb(cb) {}
    ArgOption(char c, std::string cc, std::string type, ARG_CB_TYPE cb): s(c), ss(cc), type(type), argcb(cb) {}
    
  };

  void cbConnectionType(UderoConnectionProfile& prof, std::string& argcb) {
    prof.connectionType = argcb;
  }
  
  void cbIp(UderoConnectionProfile& prof, std::string& arg) {
    prof.ipAddress = arg;
  }

  void cbPort(UderoConnectionProfile& prof, std::string& arg) {
    prof.port = atoi(arg.c_str());
  }

  void cbSetting(UderoConnectionProfile& prof, std::string& arg) {
    prof.settingFilename = arg;
  }

  void cbFile(UderoConnectionProfile& prof, std::string& arg) {
    prof.canFilename = arg;
  }

  void cbLog(UderoConnectionProfile& prof, std::string& arg) {
    prof.logFileName = arg;
  }

  void cbLogLevel(UderoConnectionProfile& prof, std::string& arg) {
    if (arg == "SILENT") prof.logLevel = UDERO_SILENT;
    else if (arg == "ERROR") prof.logLevel = UDERO_ERROR;
    else if (arg == "WARN") prof.logLevel = UDERO_WARN;
    else if (arg == "NORMAL") prof.logLevel = UDERO_NORMAL;
    else if (arg == "INFO") prof.logLevel = UDERO_INFO;
    else if (arg == "DEBUG") prof.logLevel = UDERO_DEBUG;
    else if (arg == "TRACE") prof.logLevel = UDERO_TRACE;
    else if (arg == "VERBOSE") prof.logLevel = UDERO_VERBOSE;
    else {
      std::cerr << "Invalid LogLevel Name: " << arg << std::endl;
      std::cerr << " - Ignored." << std::endl;
    }
  }

  void cbWithInit(UderoConnectionProfile& prof, std::string& arg) {
    if (arg == "false" || arg == "False" || arg == "FALSE" || arg == "NO") {
      prof.withInit = 0;
    } else {
      prof.withInit = 1;
    }
  }

  
#define NUM_ARG_OPTIONS 8
  static const ArgOption  options[NUM_ARG_OPTIONS] = {
    ArgOption('c', "--connectionType", "arg", cbConnectionType),
    ArgOption('i', "--ipAddress", "arg", cbIp),
    ArgOption('p', "--port", "arg", cbPort),
    ArgOption('f', "--filename", "arg", cbFile),
    ArgOption('l', "--logfile", "arg", cbLog), 
    ArgOption('e', "--logLevel", "arg", cbLogLevel),
    ArgOption('w', "--init", "arg", cbWithInit),
    ArgOption('s', "--setting", "arg", cbSetting)
  };


  UderoConnectionProfile defaultProf() {
    UderoConnectionProfile prof;
    prof.connectionType = "CAN";
    prof.ipAddress = "127.0.0.1";
    prof.port = 12345;
    prof.canFilename = "can0";
    prof.canBaudrate = 1000000;
    prof.canDeviceID = 0x51;
    prof.withInit = 0;
    prof.logFileName = "udero.log";
    prof.logLevel = UDERO_NORMAL;
    prof.logFile = NULL;
    prof.settingFilename = "udero.conf";
    return prof;
  }

/**
 */
LIBUDERO_API UderoConnectionProfile parseArgs(const int argc, const char* argv[]) {
#ifdef DEBUG
  std::cout << "UderoFactory. parseArgs." << std::endl;
#endif
  UderoConnectionProfile prof = defaultProf();
  prof.unknown_args.push_back("Udero");
  std::string programName = argv[0];
  prof.logFileName = programName + ".log";
  for(int i = 1;i < argc;i++) {
    std::string arg = trim(std::string(argv[i]));
#ifdef DEBUG
    std::cout << " - Arg[" << i << "] : "  << arg << std::endl;
#endif
    int optionType = -1;
    if (arg.c_str()[0] == '-') {
#ifdef DEBUG
      std::cout << " -- Find Option." << std::endl;
#endif
      if (arg.c_str()[1] == '-') { // Long Option
#ifdef DEBUG
	std::cout << " -- Find Long Option." << std::endl;
#endif
	for(int j = 0;j < NUM_ARG_OPTIONS;j++) {
	  if (options[j].ss == arg) {
	    optionType = j;
	  }
	}
      } else { // short option
#ifdef DEBUG
	std::cout << " -- Find Short Option[" << arg.c_str()[1] << "]." << std::endl;
#endif
	for(int j = 0;j < NUM_ARG_OPTIONS;j++) {
	  if (options[j].s == arg.c_str()[1]) {
	    optionType = j;
	  }
	}
      }
      
      if (optionType >= 0) {
	ArgOption opt = options[optionType];
	if (opt.type == "arg") {
	  i++;
	  if (i >= argc) {
	    throw InvalidArgumentException();
	  }

	  std::string arg = trim(std::string(argv[i]));
#ifdef DEBUG
	  std::cout << " --- Argument: " << arg << std::endl;
#endif
	  opt.argcb(prof, arg);// ここでコールバックを読んでプロファイルを修正
	}
      } else { // unknown option
	prof.unknown_args.push_back(arg);
	//	std::cout << "Unknown arg : " << arg << std::endl;
      }
    } else { // if arg.c_str()[0] == '-'
      //std::cout << "Unknown arg : " << arg << std::endl;
      prof.unknown_args.push_back(arg);
    }
  } // for
  return prof;
}


  LIBUDERO_API void initLogger(const int argc, const char* argv[]) {
    UderoConnectionProfile prof = parseArgs(argc, argv);
    std::cout << "LOGFILE:" << prof.logFileName.c_str() << std::endl;
    INIT_LOG(prof.logFileName.c_str(), (UDERO_LOGLEVEL)prof.logLevel);
    UINFO("reharo::createUdero(prof=");
    UINFO(" - connectionType = %s", prof.connectionType.c_str());
    UINFO(" - ipAddress      = %s", prof.ipAddress.c_str());
    UINFO(" - port           = %d", prof.port);
    UINFO(" - canFilename    = %s", prof.canFilename.c_str());
    UINFO(" - canDeviceID    = %x", prof.canDeviceID);
    UINFO(" - logFileName    = %s", prof.logFileName.c_str());
    UINFO(" - settingFile    = %s", prof.settingFilename.c_str());
    UINFO(" - logLevel       = %s)", LOGLEVEL_TO_STRING((UDERO_LOGLEVEL)prof.logLevel));
  }

  LIBUDERO_API void finiLogger() {
    FINI_LOG();
  }
/**
 *
 */
LIBUDERO_API IUdero* createUdero(const UderoConnectionProfile& prof) {

  IUdero* udero = NULL;
  if (prof.connectionType == "CAN") {
#ifdef BUILD_CAN
    udero = createCANUdero(prof);
#else
    throw reharo::InvalidCommandLineArgumentException();
#endif
  } else if (prof.connectionType == "MOCK") {
    udero = createMockUdero(prof);
  } else {
    throw InvalidConnectionProfileException();
  }

  if (udero) {
    if (prof.withInit) {
      udero->init();
    }
  }
  return udero;
}

/**
 *
 */
LIBUDERO_API void deleteUdero(IUdero* udero) {
  UINFO("reharo::deleteUdero()");
  delete udero; 
  UINFO("reharo::deleteUdero() ends");
}

  LIBUDERO_API IUderoBase* createUderoBase(IUdero* pUdero) {
    if (pUdero->getClassName() != "UderoCAN") {
      UWARN("reharo::createUderoBase failed. Currently, UderoBase can be created with UderoCAN.");
      return NULL;

    }
    return createCANUderoBase(pUdero);
  }

  LIBUDERO_API void deleteUderoBase(IUderoBase* udero) {
    UINFO("reharo::deleteUderoBase()");
    delete udero;
    UINFO("reharo::deleteUderoBase() ends");
  }


};
