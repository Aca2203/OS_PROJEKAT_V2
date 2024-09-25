#include "../h/syscall_cpp.hpp"
#include "../h/printing.hpp"

#include "../lib/hw.h"
#include "../h/tcb.hpp"
#include "../h/workers.hpp"
#include "../h/printing.hpp"
#include "../h/riscv.hpp"
#include "../lib/console.h"
#include "../h/syscall_cpp.hpp"

Semaphore* semaphore;

class Worker: public Thread {
    void workerBody(void* arg);
public:
    Worker():Thread() {}

    void run() override {
        workerBody(nullptr);
    }
};

void Worker::workerBody(void *arg) {
    for(int i = 0; i < 5; i++) {
        int id = this->getId();
        printString("Hello! ");
        printInt(id);
        printString("\n");
        for(int i = 0; i < 10000 * id; i++);
    }
    semaphore->signal();
}

const int numThreads = 20;
Thread* threads[numThreads];

extern void userMain();

int modifikacija() {
    Thread::SetMaximumThreads(3);

    semaphore = new Semaphore(0);

    for(int i = 0; i < numThreads; i++) {
        threads[i] = new Worker();
    }

    for(int i = 0; i < numThreads; i++){
        threads[i]->start();
    }

    for(int i = 0; i < numThreads; i++) semaphore->wait();

    for(int i = 0; i < numThreads; i++) delete threads[i];
    delete semaphore;

    return 0;
}