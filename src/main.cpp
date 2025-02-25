#include "main.hpp"
#include <common.h>
#include <peripherals/gpio.hpp>
#include "utils/utils.h"
#include "peripherals/uart.hpp"
#include "arm/interrupts/irq.h"
#include "external/rust_fn.h"
#include "utils/string.h"

void kernel_main()
{
    GPIO::SetFunctionSelect(21, GPIO::FUNCTION_SELECT_OPTIONS::OUTPUT);

    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();

    uart::uart_init();

    delay(2000);

    char buffer[12];
    u32 el = get_el();
    UIntToStr(el, buffer);
    uart::send_string("EL level: \r\n");
    uart::send_string(buffer);
    uart::send_string("\n\r");

    rust_init();

    uart::send_string("Code broke from rust \n\r");
    while (1)
    {
        uart::send_string("Code arrived to cpp loop \n\r");
        delay(1000000);
    }
}
