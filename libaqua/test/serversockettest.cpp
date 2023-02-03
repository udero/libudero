
#include "ServerSocket.h"

#include <stdio.h>
#include <stdlib.h>
#include <exception>

#include <iostream>
#include "Thread.h"
using namespace ssr;

int main(int argc, char* argv[]) {
  std::cout << "Server Test." << std::endl;
  try {
	  SocketInitializer si;
    if (argc <= 1) throw SocketException("Invalid Argument");
    ServerSocket socket;
    socket.Bind(atoi(argv[1]));
    socket.Listen(1);
    
    Socket client = socket.Accept();

    
    while(true) {
      Thread::Sleep(500);
      if (client.GetSizeInRxBuffer() > 0) {
	uint8_t buf;
	if (client.Read(&buf, 1) < 0) {
	  throw SocketException("Read failed.");
	}

	if (buf != 123) {
	  throw SocketException("Wrong Data");
	} else {
	  std::cout << "Received." << std::endl;
	  break;
	}

      } else {
	std::cout << "Waiting Data..." << std::endl;
      }
    }

    client.Close();
  
    std::cout << "Okay" << std::endl;
  } catch (std::exception& ex) {
    std::cout << "exception: " << ex.what() << std::endl;
    perror(NULL);
    return -1;
  }
  return 0;
}
