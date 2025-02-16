#include "arm/interrupts/irq.h"
//D13.2.137
.global irq_init_vectors
irq_init_vectors:
    adr x0, vectors
    msr vbar_el1, x0
    ret

//C5.2.2
.global irq_enable
irq_enable:
    msr daifclr, #2
    ret

.global irq_disable
irq_disable:
    msr daifset, #2
    ret