#ifndef SYSCALL_CPP_H
#define SYSCALL_CPP_H

#include "syscall_c.hpp"

void* operator new (size_t); // Alokacija memorije
void operator delete (void*) noexcept; // Dealokacija memorije
void* operator new[] (size_t); // Alokacija memorije (niz)
void operator delete[] (void*) noexcept; // Dealokacija memorije (niz)

class Thread {
public:
  Thread(void (*body)(void*), void* arg);
  virtual ~Thread();

  int start();

  static void dispatch();
  static int sleep(time_t);

protected:
  Thread();
  virtual void run() {};

private:
  thread_t myHandle;
  void (*body)(void*); void* arg;

  static void runWrapper(void* thread) {
    if(thread != nullptr) ((Thread*)thread)->run();
  }
};

class Semaphore {
public:

  Semaphore(unsigned init = 1);
  virtual ~Semaphore();

  int wait();
  int signal();
  int timedWait(time_t);
  int tryWait();

private:
  sem_t myHandle;
};

class PeriodicThread : public Thread{
public:
  void terminate();

protected:
  PeriodicThread(time_t period);
  virtual void periodicActivation();

private:
  time_t period;
};

class Console {
public:
  static char getc();
  static void putc(char);
};

#endif