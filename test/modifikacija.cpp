#include "../h/printing.hpp"
#include "../h/syscall_cpp.hpp"

class WorkerAA: public Thread {
    void workerBodyAA(void* arg);
public:
    WorkerAA():Thread() {}

    void run() override {
        workerBodyAA(nullptr);
    }
};

void WorkerAA::workerBodyAA(void* arg) {
    for(int i = 0; i < 3; i++) {
        printString("Zdravo A!\n");
        thread_dispatch();
    }
    printString("Nit A zavrsena!\n");
}

class WorkerBB: public Thread {
    void workerBodyBB(void* arg);
public:
    WorkerBB():Thread() {}

    void run() override {
        workerBodyBB(nullptr);
    }
};

void WorkerBB::workerBodyBB(void* arg) {
    for(int i = 0; i < 3; i++) {
        printString("Zdravo B!\n");
        thread_dispatch();
    }
    printString("Nit B zavrsena!\n");
}

void modifikacija(){
    Thread* threads[2];

    threads[0] = new WorkerAA();
    printString("ThreadA created\n");

    threads[1] = new WorkerBB();
    printString("ThreadB created\n");

    for(int i=0; i<2; i++) {
        threads[i]->start();
    }

    Thread::joinAll();

    for (auto thread: threads) { delete thread; }
}