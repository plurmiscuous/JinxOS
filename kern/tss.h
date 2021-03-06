#ifndef _TSS_H_
#define _TSS_H_

struct taskstate {
    uint32_t ts_link;       // Old ts selector
    uint32_t ts_esp0;       // Stack pointers and segment selectors
    uint16_t ts_ss0;        //   after an increase in privilege level
    uint16_t ts_padding1;
    uint32_t ts_esp1;
    uint16_t ts_ss1;
    uint16_t ts_padding2;
    uint32_t ts_esp2;
    uint16_t ts_ss2;
    uint16_t ts_padding3;
    uint32_t ts_cr3;        // Page directory base
    uint32_t ts_eip;        // Saved state from last task switch
    uint32_t ts_eflags;
    uint32_t ts_eax;        // More saved state (registers)
    uint32_t ts_ecx;
    uint32_t ts_edx;
    uint32_t ts_ebx;
    uint32_t ts_esp;
    uint32_t ts_ebp;
    uint32_t ts_esi;
    uint32_t ts_edi;
    uint16_t ts_es;         // Even more saved state (segment selectors)
    uint16_t ts_padding4;
    uint16_t ts_cs;
    uint16_t ts_padding5;
    uint16_t ts_ss;
    uint16_t ts_padding6;
    uint16_t ts_ds;
    uint16_t ts_padding7;
    uint16_t ts_fs;
    uint16_t ts_padding8;
    uint16_t ts_gs;
    uint16_t ts_padding9;
    uint16_t ts_ldt;
    uint16_t ts_padding10;
    uint16_t ts_t;          // Trap on task switch
    uint16_t ts_iomb;       // I/O map base address
};

#endif // _TSS_H_
