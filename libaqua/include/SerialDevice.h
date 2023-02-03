#pragma once



namespace ssr {
  class SerialDevice {
  public:

    virtual ~SerialDevice() {}

  public:

    virtual int GetSizeInRxBuffer() = 0;
    virtual int Write(const void* src, const unsigned int size) = 0;
    virtual int Read(void* dst, const unsigned int size) = 0;

  };


}
