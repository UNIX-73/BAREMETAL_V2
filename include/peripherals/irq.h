#pragma once

#include "peripherals/peripheralBase.h"
#include "common.h"

enum vc_irqs {
    AUX_IRQ = (1 << 29)
};

struct arm_irq_regs_2711 {
    reg32 irq0_pending_0;
    reg32 irq0_pending_1;
    reg32 irq0_pending_2;
    reg32 res0;
    reg32 irq0_enable_0;
    reg32 irq0_enable_1;
    reg32 irq0_enable_2;
    reg32 res1;
    reg32 irq0_disable_0;
    reg32 irq0_disable_1;
    reg32 irq0_disable_2;
};

struct arm_irq_regs_2837 {
    reg32 irq0_pending_0;
    reg32 irq0_pending_1;
    reg32 irq0_pending_2;
    reg32 fiq_control;
    reg32 irq0_enable_1;
    reg32 irq0_enable_2;
    reg32 irq0_enable_0;
    reg32 res;
    reg32 irq0_disable_1;
    reg32 irq0_disable_2;
    reg32 irq0_disable_0;
};

typedef struct arm_irq_regs_2837 arm_irq_regs;


#if RPI_VERSION == 4
    typedef struct arm_irq_regs_2711 arm_irq_regs;
#endif

#define REGS_IRQ ((arm_irq_regs *)(PERIPHERAL_BASE + 0x0000B200))