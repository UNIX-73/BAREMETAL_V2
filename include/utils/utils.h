#pragma once
#ifndef __ASSEMBLER__
#include <common.h>

#ifdef __cplusplus
extern "C"
{
#endif

    void delay(u64 ticks);
    void put32(u64 address, u32 value);
    u32 get32(u64 address);
    u32 get_el();

#ifdef __cplusplus
}
#endif
#endif // __ASSEMBLER__



