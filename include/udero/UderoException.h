#pragma once


namespace technotools {

  class UderoException : public std::exception {
  private:
    std::string msg;
  public:
  UderoException() : msg("UderoException") {}
  UderoException(const std::string& msg) : msg(msg) {}
  UderoException(const char* msg): msg(msg) {}

    virtual ~UderoException() {}

  public:
    const char* what() const throw() {
      return msg.c_str();
    }
  };

#define DEFINE_EXCEPTION(name) class name ## Exception : public UderoException { \
  public: name ## Exception() : UderoException(" ## name"){} };


  DEFINE_EXCEPTION(JointIndexOutOfRange);
  DEFINE_EXCEPTION(InvalidArgument);
  DEFINE_EXCEPTION(InvalidCommandLineArgument);
  DEFINE_EXCEPTION(InvalidConnectionProfile);
  /*
  class JointIndexOutOfRangeException : public UderoException {
  public:
  JointIndexOutOfRangeException() : UderoException("JointIndexOutOfRage");
  };

  class InvalidArgumentException : public UderoException {
  public:
  InvalidArgumentException() : UderoException("InvalidArgument") {}
  };

  class InvalidConnectionProfileException: public UderoException {
  public:
  InvalidConnectionProfileException() : UderoException("InvalidConnectionProfile") {}
  };
  */
  class JointRangeException : public std::exception {
  public:
    JointRangeException() {}
    virtual ~JointRangeException() throw() {}

    const char* what() const throw() {
      return "JointRangeException";
    }
  };

  class HomingFailedException : public std::exception {
  public:
    HomingFailedException() {}
    virtual ~HomingFailedException() throw() {}

    const char* what() const throw() {
      return "HomingFailedException";
    }
  };

  class HomingErrorException : public std::exception {
  public:
    HomingErrorException() {}
    virtual ~HomingErrorException() throw() {}

    const char* what() const throw() {
      return "HomingErrorException";
    }
  };


};
