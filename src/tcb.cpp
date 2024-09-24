#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/scheduler.hpp"
#include "../h/syscall_c.hpp"

TCB* TCB::running = nullptr;
uint64 TCB::timeSliceCounter = 0;

TCB* TCB::createThread(Body body, void* arg) {
    TCB* tcb = new TCB(body, arg);
    if(!tcb->isMain()) {
        TCB::startThread(tcb);
    }
    return tcb;
}

TCB* TCB::createThreadWithoutStarting(Body body, void* arg) {
    return new TCB(body, arg);
}

void TCB::startThread(TCB* tcb) {
    Scheduler::put(tcb);
    TCB::running->childrenCount++;
    tcb->parent = TCB::running;
}

void TCB::yield() {
    Riscv::w_a0(0x13);
    __asm__ volatile("ecall");
}

void TCB::dispatch() {
    TCB* old = running;
    if(!old->isFinished() && !old->isBlocked()){ Scheduler::put(old); }
    running = Scheduler::get();

    if(running->isMain()) {
        Riscv::ms_sstatus(Riscv::SSTATUS_SPP);
    } else {
        Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    }

    TCB::contextSwitch(&old->context, &running->context);
}

void TCB::joinAll() {
    while(TCB::running->childrenCount > 0) TCB::dispatch();
}

void TCB::threadWrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);
    running->parent->childrenCount--;
    thread_exit();
}