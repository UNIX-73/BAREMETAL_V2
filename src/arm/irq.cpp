#include "arm/interrupts/irq.h"

#include "utils/utils.h"
#include "arm/interrupts/ir_entry.h"
#include "peripherals/irq.h"
#include "peripherals/aux.h"
#include "peripherals/uart.hpp"

const char entry_error_messages[16][32] = {
    "SYNC_INVALID_EL1t",
    "IRQ_INVALID_EL1t",
    "FIQ_INVALID_EL1t",
    "ERROR_INVALID_EL1T",

    "SYNC_INVALID_EL1h",
    "IRQ_INVALID_EL1h",
    "FIQ_INVALID_EL1h",
    "ERROR_INVALID_EL1h",

    "SYNC_INVALID_EL0_64",
    "IRQ_INVALID_EL0_64",
    "FIQ_INVALID_EL0_64",
    "ERROR_INVALID_EL0_64",

    "SYNC_INVALID_EL0_32",
    "IRQ_INVALID_EL0_32",
    "FIQ_INVALID_EL0_32",
    "ERROR_INVALID_EL0_32"};

void show_invalid_entry_message(u32 type, u64 esr, u64 address)
{
    UART::SendString("INVALID ENTRY\n\r");
}

void handle_irq()
{
    u32 irq;

    irq = REGS_IRQ->irq0_pending_1;

    UART::SendString("AAAAAsAAAAAA\n\r");

    while (irq)
    {
        if (irq & AUX_IRQ)
        {
            irq &= ~AUX_IRQ;

            while ((REGS_AUX->mu_iir & 4) == 4)
            {
                UART::SendString("AAAAAAAAAAA\n\r");
            }
        }
    }
}
void enable_interrupt_controller()
{
    REGS_IRQ->irq0_enable_1 = AUX_IRQ;
}