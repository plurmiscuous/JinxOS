#include <lib.h>
#include <spinlock.h>
#include <multiboot.h>
#include <mmu.h>
#include <memlayout.h>
#include <console.h>
#include <x86.h>
#include <init.h>
#include <pmm.h>
#include <pit.h>
#include <kmm.h>
#include <cga.h>

#define demand(exp)                                         \
    do {                                                    \
        if (!(exp))                                         \
            outb(0x92, 0x3);                                \
    } while(0)

__attribute__((aligned(BIT(12))))
pde_t bpgd[NPDENTRIES] = {
    [0] = PTE_P | PTE_W | PTE_PS,
};

static void jinx() {
    static const char* title[] = {
        "\t\t          _/  _/                               _/_/      _/_/_/\n",
        "\t\t         _/      _/_/_/    _/    _/         _/    _/  _/       \n",
        "\t\t        _/  _/  _/    _/    _/_/           _/    _/    _/_/    \n",
        "\t\t _/    _/  _/  _/    _/  _/    _/         _/    _/        _/   \n",
        "\t\t  _/_/    _/  _/    _/  _/    _/           _/_/    _/_/_/      \n",
    };

    print("\n\n");
    for (int i = 0; i < 5; ++i) {
        int j;
        int k = 0;
        for (j = 0; j < 6 - i; ++j, ++k)
            putc(title[i][k]);

        settextcolor(0x0200);
        for (j = 0; j < 32; ++j, ++k)
            putc(title[i][k]);

        settextcolor(0x0800);
        for (j = 0; j < 27; ++j, ++k)
            putc(title[i][k]);
        putc('\n');
    }
    print("\n");
    settextcolor(0x0F00);
}

void kmain(uint32_t eax, size_t ebx) {
    demand(*(uint16_t*) 0x7DFE == 0xAA55);
    demand(eax == 0x2BADB002);

    extern char _bss[], _ebss[];
    memset(_bss, 0, _ebss - _bss);

    init_cga();
    init_serial();

    init_e820(ebx);
    init_gdt();
    init_idt();
    init_isr();
    init_irq();
    init_pit();
    init_kbd();

    init_kmm();
    init_pmm();

    lock_kernel();

    jinx();

    sti();              // enable interrupts
    hlt();              // ensure interrupts are enabled

    for (;;)
        prompt();
}
