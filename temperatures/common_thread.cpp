
#include "common_thread.h"
#include <iostream>
#include <limits.h>

void* Thread::runner(void *data) {
  Thread* self = (Thread*) data;
  self->run();
  return NULL;
}

void Thread::start() {
  pthread_attr_t tattr;
  pthread_attr_init(&tattr);
  size_t size = PTHREAD_STACK_MIN + 50 * 1024;
  pthread_attr_setstacksize(&tattr, size);
  pthread_create(&thread, &tattr, Thread::runner, this);
}

void Thread::join() {
  pthread_join(thread, NULL);
}


Mutex::Mutex() {
  pthread_mutex_init(&this->mutex, NULL);
}

void Mutex::lock() {
  pthread_mutex_lock(&this->mutex);
}

void Mutex::unlock() {
  pthread_mutex_unlock(&this->mutex);
}

Mutex::~Mutex() {
  pthread_mutex_destroy(&this->mutex);
}


Lock::Lock(Mutex &m) : m(m) {
  m.lock();
}

Lock::~Lock() {
  m.unlock();
}
