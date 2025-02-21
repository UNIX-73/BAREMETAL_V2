#include <common.h>
#include <peripherals/gpio.hpp>
#include "utils/utils.h"
#include "peripherals/uart.hpp"
#include "arm/interrupts/irq.h"

void UIntToStr(u32 num, char *str)
{
    int i = 0;
    do
    {
        str[i++] = '0' + (num % 10); // Obtener el último dígito
        num /= 10;                   // Reducir el número
    } while (num > 0);

    str[i] = '\0'; // Terminar la cadena con un carácter nulo

    // Invertir la cadena
    for (int j = 0; j < i / 2; j++)
    {
        char temp = str[j];
        str[j] = str[i - j - 1];
        str[i - j - 1] = temp;
    }
}

extern "C" void rust_init();

extern "C" void kernel_main()
{

    GPIO::SetFunctionSelect(21, GPIO::FUNCTION_SELECT_OPTIONS::OUTPUT);

    irq_init_vectors();
    enable_interrupt_controller();
    irq_enable();

    uart::uart_init();

    delay(2000);

    rust_init();

    char buffer[12];
    u32 el = get_el();
    UIntToStr(el, buffer);
    uart::send_string("EL level: \r\n");
    uart::send_string(buffer);
    uart::send_string("\n\r");

    while (1)
    {
        uart::send_string("Code arrived to cpp loop \n\r");
        delay(1000000);
    }
}
