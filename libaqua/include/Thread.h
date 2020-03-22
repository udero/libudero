/********************************************************
 * SerialPort.h
 *
 * Portable Thread Class Library for Windows and Unix.
 * @author ysuga@ysuga.net
 * @date 2010/11/02
 ********************************************************/
#pragma once

#include "aqua.h"
#include <exception>

#ifdef WIN32
#include <windows.h>
#define THREAD_ROUTINE DWORD WINAPI
#else
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#define THREAD_ROUTINE void*
#endif


#include "TimeSpec.h"

namespace ssr {

	class MutexTimeoutException : public std::exception {
	public:
		MutexTimeoutException() {}

		virtual ~MutexTimeoutException() throw() {}

	public:
		const char* what() const throw() { return "Mutex Lock Timeout."; }
	};

  /**
   *
   */
  class Mutex {
  private:
#ifdef WIN32
    HANDLE m_Handle;
#else
    pthread_mutex_t m_Handle;
#endif
    
    
  public:
    Mutex() {
#ifdef WIN32
      m_Handle = ::CreateMutex(NULL, 0, NULL);
#else
      pthread_mutex_init(&m_Handle, NULL);
#endif
    }
    
    virtual ~Mutex() {
#ifdef WIN32
      ::CloseHandle(m_Handle);
#else
      pthread_mutex_destroy(&m_Handle);
#endif
    }
    
  public:
    bool Lock(const ssr::TimeSpec& timeout=INFINITETIME) {
#ifdef WIN32
		if (timeout == INFINITETIME) {
			::WaitForSingleObject(m_Handle, INFINITE);
			return true;
		}
		else {
			DWORD ret = WaitForSingleObject(m_Handle, timeout.sec * 1000 + timeout.usec / 1000);
			if (ret == WAIT_TIMEOUT) {
				return false;
			}
		}

#else
      pthread_mutex_lock(&m_Handle);
      return true;
#endif
    }
    
    void Unlock() {
#ifdef WIN32
      ::ReleaseMutex(m_Handle);
#else
      pthread_mutex_unlock(&m_Handle);
#endif
    }
  };

  /**
   *
   */
  class MutexBinder {
  private:
    Mutex& m_mutex;
  public:
  MutexBinder(Mutex& mutex, const ssr::TimeSpec& timeout=INFINITETIME) : m_mutex(mutex) {
	  if (!m_mutex.Lock(timeout)) {
		  throw MutexTimeoutException();
	  }
  }
    ~MutexBinder() {m_mutex.Unlock();}
  };

  static THREAD_ROUTINE StartRoutine(void* arg);

  
  /**
   *
   */
  class Thread {
  private:
#ifdef WIN32
    HANDLE m_Handle;
    DWORD m_ThreadId;
#else
    pthread_t m_Handle;
#endif
  public:

    /**
     *
     */
    Thread(void) {
    }

    virtual ~Thread(void) {
    }
    
  public:
    void Start() {
#ifdef WIN32
	m_Handle = CreateThread(NULL, 0, StartRoutine, (LPVOID)this, 0, &m_ThreadId);
#else
	int ret = pthread_create(&m_Handle, NULL, StartRoutine, (void*)this);
	if(ret != 0) {
	  perror("pthread_create");
	}
#endif
    }

    
    virtual void Run() = 0;
    
    void Join() {
#ifdef WIN32
      WaitForSingleObject(m_Handle, INFINITE);
#else
      void* retval;
      pthread_join(m_Handle, &retval);
#endif
    }
    
    static void Sleep(unsigned long milliSeconds) {
#ifdef WIN32
      ::Sleep(milliSeconds);
#else
      struct timespec interval;
      interval.tv_sec = milliSeconds / 1000;
      interval.tv_nsec = (milliSeconds % 1000) * 1000000;
      nanosleep(&interval, NULL);
#endif
    }
    
  public:

    void Exit(unsigned long exitCode) {
#ifdef WIN32
	ExitThread(exitCode);
#else
	pthread_exit(0);
#endif
    }
  };


  static THREAD_ROUTINE StartRoutine(void* arg)
  {
    Thread* threadObject = (Thread*)arg;
    threadObject->Run();
    threadObject->Exit(0);
#ifdef WIN32
    ExitThread(0);
#else 
    pthread_exit(0);
#endif
    return 0;
  }

};



/*******************************************************
 * Copyright  2010, ysuga.net all rights reserved.
 *******************************************************/
