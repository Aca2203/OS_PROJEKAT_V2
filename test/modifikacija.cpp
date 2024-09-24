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

class WorkerBB: public Thread {
    void workerBodyBB(void* arg);
public:
    WorkerBB():Thread() {}

    void run() override {
        workerBodyBB(nullptr);
    }
};

void WorkerAA::workerBodyAA(void *arg) {
    for(int i = 0; i < 3; i++){
        printString("Hello! Thread A here!\n");
        Thread::dispatch();
    }
    printString("A finished!\n");
}

void WorkerBB::workerBodyBB(void *arg) {
    for(int i = 0; i < 3; i++){
        printString("Hello! Thread B here!\n");
        Thread::dispatch();
    }
    printString("B finished!\n");
}

void modifikacija() {
    Thread* threads[2];

    threads[0] = new WorkerAA();
    printString("ThreadA created\n");

    threads[1] = new WorkerBB();
    printString("ThreadB created\n");

    threads[0]->start();
    threads[1]->start();

    threads[0]->join();
    threads[1]->join();

    for (auto thread: threads) { delete thread; }
}