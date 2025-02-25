#include "arm/interrupts/irq.h"

#include "utils/utils.h"
#include "arm/interrupts/ir_entry.h"
#include "peripherals/irq.h"
#include "peripherals/aux_.h"
#include "peripherals/uart.hpp"
#include <utils/string.h>
#include "../../main.hpp"

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
    "ERROR_INVALID_EL0_32"
};

void show_invalid_entry_message(u32 type, u64 esr, u64 address)
{
    uart::send_string("INVALID ENTRY\n\r");

    uart::send_string("IR TYPE=");
    if (type <= 32) {
        uart::send_string(entry_error_messages[type]);
        uart::send_string("\r\n");
    }
    else{
        uart::send_string("UNKNOWN\r\n");
    }

    uart::send_string("ESR=");
    char esr_buffer[64];
    UIntToStr(esr, esr_buffer);
    uart::send_string("\r\n");

    uart::send_string("address=");
    char address_buffer[64];
    UIntToStr(address, address_buffer);

    uart::send_string("\r\n\n\n\n");
    uart::send_string("WAITING FOR REBOOT...\r\n");

    delay(5000000);

    uart::send_string("REBOOTING...\r\n");

    kernel_main();
}

void handle_irq()
{
    // u32 irq;

    // irq = REGS_IRQ->irq0_pending_1;

    // TODO: Validar que tipo de pending hay antes
    uart::IRQ_MANAGER::handle_irq_mis();
}

void enable_interrupt_controller()
{
    REGS_IRQ->irq0_enable_1 = AUX_IRQ;
}