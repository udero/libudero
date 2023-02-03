
#include "Socket.h"
#include <stdlib.h>
#include <iostream>
#include "Thread.h"

using namespace ssr;
int main(int argc, char* argv[]) {
  try {
    std::cout << "Socket Test." << std::endl;
	SocketInitializer si;

    if (argc <= 2) return -1;
    Socket socket(argv[1], atoi(argv[2]));


    Thread::Sleep(5000);
    
    uint8_t buf = 123;

    if (socket.Write(&buf, 1) < 0) {
      throw SocketException("Write failed.");
    }
    
    if (socket.Close() < 0) {
      std::cout << "Close Failed." << std::endl;
      return -1;
    }

    std::cout << "Okay" << std::endl;
  } catch (std::exception& ex) {
    std::cout << "exception:" << ex.what() << std::endl;
    return -1;
  }
  return 0;
}
