#include "../h/syscall_cpp.hpp"
#include "../h/printing.hpp"

Semaphore* sem;

bool volatile finishedA = false;
bool volatile finishedB = false;
bool volatile finishedC = false;
bool volatile finishedD = false;

class WorkerA2: public Thread {
    void workerBodyA2(void* arg);
public:
    WorkerA2():Thread() {}

    void run() override {
        workerBodyA2(nullptr);
    }
};

class WorkerB2: public Thread {
    void workerBodyB2(void* arg);
public:
    WorkerB2():Thread() {}

    void run() override {
        workerBodyB2(nullptr);
    }
};

class WorkerC2: public Thread {
    void workerBodyC2(void* arg);
public:
    WorkerC2():Thread() {}

    void run() override {
        workerBodyC2(nullptr);
    }
};

class WorkerD2: public Thread {
    void workerBodyD2(void* arg);
public:
    WorkerD2():Thread() {}

    void run() override {
        workerBodyD2(nullptr);
    }
};

void WorkerA2::workerBodyA2(void *arg) {
    Thread::barrier();
    printString("A barrier finished\n");
    //sem->signal();
    finishedA = true;
}

void WorkerB2::workerBodyB2(void *arg) {
    Thread::barrier();
    printString("B barrier finished\n");
    //sem->signal();
    finishedB = true;
}

void WorkerC2::workerBodyC2(void *arg) {
    Thread::barrier();
    printString("C barrier finished\n");
    //sem->signal();
    finishedC = true;
}

void WorkerD2::workerBodyD2(void* arg) {
    Thread::barrier();
    printString("D barrier finished\n");
    //sem->signal();
    finishedD = true;
}

void modifikacija(){
    Thread* threads[4];
    sem = new Semaphore(0);

    threads[0] = new WorkerA2();
    printString("ThreadA created\n");

    threads[1] = new WorkerB2();
    printString("ThreadB created\n");

    threads[2] = new WorkerC2();
    printString("ThreadC created\n");

    threads[3] = new WorkerD2();
    printString("ThreadD created\n");

    for(int i=0; i<4; i++) {
        threads[i]->start();
    }

    Thread::barrier();

    printString("UserMain barrier finished!\n");

    while(!finishedA || !finishedB || !finishedC || !finishedD) Thread::dispatch();

    //for(int i=0; i<4; i++) sem->wait();

    for (auto thread: threads) { delete thread; }
}