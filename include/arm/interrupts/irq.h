#pragma once
#ifndef __ASSEMBLER__
#include <common.h>

#ifdef __cplusplus
extern "C"
{
#endif
    // DEFINED IN ASM
    void irq_init_vectors();
    void irq_enable();
    void irq_disable();

    // DEFINED IN CPP
    void enable_interrupt_controller();
    void show_invalid_entry_message(u32 type, u64 esr, u64 address);
    void handle_irq();

#ifdef __cplusplus
}
#endif
#endif // __ASSEMBLER__
