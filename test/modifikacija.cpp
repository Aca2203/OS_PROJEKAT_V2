#include "../h/syscall_cpp.hpp"
#include "../h/printing.hpp"

static volatile bool finishedA = false;
static volatile bool finishedB = false;
static volatile bool finishedC = false;

Thread* threads[3];

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

class WorkerCC: public Thread {
    void workerBodyCC(void* arg);
public:
    WorkerCC():Thread() {}

    void run() override {
        workerBodyCC(nullptr);
    }
};

void WorkerAA::workerBodyAA(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        if(i == 2) {
            threads[1]->sendCPP((char*)"A salje B\n");
            threads[2]->sendCPP((char*)"A salje C\n");
            char* msg = receiveCPP();
            printString("A: "); printString(msg);
        }
        printString("A: i="); printInt(i); printString("\n");
        //for (uint64 j = 0; j < 10000; j++) {}
        thread_dispatch();
    }
    printString("A finished!\n");

    finishedA = true;
}

void WorkerBB::workerBodyBB(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        if(i == 4) threads[2]->sendCPP((char*)"B salje C\n");
        if(i == 7) threads[0]->sendCPP((char*)"B salje A\n");
        if(i == 8) {
            printString("B: "); char* msg = Thread::receiveCPP(); printString(msg);
        }
        printString("B: i="); printInt(i); printString("\n");
        //for (uint64 j = 0; j < 10000; j++) {}
        thread_dispatch();
    }
    printString("B finished!\n");

    finishedB = true;
}

void WorkerCC::workerBodyCC(void *arg) {
    for (uint64 i = 0; i < 10; i++) {
        if(i == 6 || i == 8) {
            char* msg = receiveCPP();
            printString("C: "); printString(msg);
        }
        printString("C: i="); printInt(i); printString("\n");
        //for (uint64 j = 0; j < 10000; j++) {}
        thread_dispatch();
    }
    printString("C finished!\n");

    finishedC = true;
}

void modifikacija() {
    threads[0] = new WorkerAA();
    printString("ThreadA created\n");

    threads[1] = new WorkerBB();
    printString("ThreadB created\n");

    threads[2] = new WorkerCC();
    printString("ThreadC created\n");

    for(int i=0; i<3; i++) {
        threads[i]->start();
    }

    while (!(finishedA && finishedB && finishedC)) {
        Thread::dispatch();
    }

    for (auto thread: threads) { delete thread; }
}