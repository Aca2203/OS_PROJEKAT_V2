#include "../test/modifikacija.hpp"
#include "../h/printing.hpp"
#include "../h/syscall_cpp.hpp"

Thread* threads[3];

Semaphore* sem;

class WorkerBB: public Thread {
    void workerBodyBB(void* arg);
public:
    WorkerBB():Thread() {}

    void run() override {
        workerBodyBB(nullptr);
    }
};

void WorkerBB::workerBodyBB(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        printString("B: i="); printInt(i); printString("\n");
        Thread::dispatch();
    }
    printString("B finished!\n");
    sem->signal();
}

class WorkerCC: public Thread {
    void workerBodyCC(void* arg);
public:
    WorkerCC():Thread() {}

    void run() override {
        workerBodyCC(nullptr);
    }
};

void WorkerCC::workerBodyCC(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        if(i == 5) threads[0]->Thread::pingCPP();
        printString("C: i="); printInt(i); printString("\n");
        Thread::dispatch();
    }
    printString("C finished!\n");
    sem->signal();
}

class WorkerAA: public Thread {
    void workerBodyAA(void* arg);
public:
    WorkerAA():Thread() {}

    void run() override {
        workerBodyAA(nullptr);
    }
};

void WorkerAA::workerBodyAA(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        int* niz = new int[10];
        printString("A: i="); printInt(i); printString("\n");
        Thread::dispatch();
        delete[] niz;
    }
    printString("A finished!\n");
    sem->signal();
}

void modifikacija() {

    sem = new Semaphore(0);

    threads[0] = new WorkerAA();
    printString("ThreadA created\n");

    threads[1] = new WorkerBB();
    printString("ThreadB created\n");

    threads[2] = new WorkerCC();
    printString("ThreadC created\n");

    for(int i=0; i<3; i++) {
        threads[i]->start();
    }

    for(int i = 0; i < 3; i++) {
        sem->wait();
    }

    for (auto thread: threads) {
        delete thread;
    }

    delete sem;
}