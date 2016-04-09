#ifndef __THREADS_H__
#define __THREADS_H__

#include <pthread.h>


class Thread {
private:
    pthread_t thread;

    static void* runner(void *data);

public:
    Thread() {}

    void start();
    void join();

    virtual void run() = 0;
    virtual ~Thread() {}

private:
    Thread(const Thread&);
    Thread& operator=(const Thread&);
};


#endif
