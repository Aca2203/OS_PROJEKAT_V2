#include "../h/printing.hpp"
#include "../h/syscall_cpp.hpp"

Semaphore* waitForAll;
Semaphore* mutex;

class Worker: public Thread {
    void workerBody(void* arg);
public:
    Worker():Thread() {}

    void run() override {
        workerBody(nullptr);
    }
};

void Worker::workerBody(void *arg) {
    for(int i = 0; i < 3; i++) {
        mutex->wait();
        printString("Nit ");
        printInt(this->getId());
        printString(" je usla u kriticnu sekciju.\n");
        for(int i = 0; i < 10000 * this->getId(); i++);
        Thread::dispatch();
        mutex->signal();
        printString("Nit ");
        printInt(this->getId());
        printString(" je izasla iz kriticne sekcije.\n");
    }

    waitForAll->signal();
}

const int THREAD_COUNT = 50;

void modifikacija() {
    Thread* threads[THREAD_COUNT];
    waitForAll = new Semaphore(0);
    mutex = new Semaphore(5);

    for(int i = 0; i < THREAD_COUNT; i++) {
        threads[i] = new Worker();
    }

    for(int i = THREAD_COUNT - 1; i >= 0; i--) {
        threads[i]->start();
    }

    for(int i = 0; i < THREAD_COUNT; i++) waitForAll->wait();

    for (auto thread: threads) { delete thread; }
}