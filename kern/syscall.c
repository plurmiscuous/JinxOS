#include <lib.h>
#include <syscall.h>
#include <int.h>
#include <thread.h>

int
syscall(uint32_t num, uint32_t a1, uint32_t a2, uint32_t a3) {
    (void) a3;

    int ret = 0;

    switch (num) {
        case syscall_putc:
            putc((char) a1);
            break;
        case syscall_exit:
            thread_exit((int) a1);
            break;
        case syscall_thread_yield:
            thread_switch(S_READY, NULL, NULL);
            break;
        case syscall_thread_wait:
            assert(a1 != 0);
            assert(a2 != 0);
            thread_switch(S_SLEEP, (struct wchan*) a1, (struct spinlock*) a2);
            break;
        default:
            panic("unhandled syscall no: %u", num);
    }

    return ret;
}

static inline
int
_syscall(int num, bool check, uint32_t a1, uint32_t a2, uint32_t a3) {
    // Generic system call: pass system call number in AX,
    // up to five parameters in DX, CX, BX, DI, SI.
    // Interrupt kernel with ISR_SYSCALL.
    //
    // The "volatile" tells the assembler not to optimize
    // this instruction away just because we don't use the
    // return value.
    //
    // The last clause tells the assembler that this can
    // potentially change the condition codes and arbitrary
    // memory locations.

    int ret;

    asm volatile(
        "int %1\n"
        : "=a" (ret)
        : "i" (ISR_SYSCALL), "a" (num), "d" (a1), "c" (a2), "b" (a3)
        : "cc", "memory");

    if(check && ret > 0)
        panic("syscall %d returned %d (> 0)", num, ret);

    return ret;
}

void
thread_yield(void) {
    _syscall(syscall_thread_yield, false, 0, 0, 0);
}

void
thread_wait(struct wchan* wc, struct spinlock* lk) {
    _syscall(syscall_thread_wait, false, (uint32_t) wc, (uint32_t) lk, 0);
}
