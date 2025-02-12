#include <common.h>
#include <peripherals/gpio.hpp>
#include "utils/utils.h"
#include "uart/uart.hpp"

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

extern "C" void kernel_main()
{
    GPIO::SetFunctionSelect(21, GPIO::FUNCTION_SELECT_OPTIONS::OUTPUT);
    UART::UartInit();

    char buffer[12];
    u32 el = get_el();
    UIntToStr(el, buffer);
    UART::SendString("EL level: \r\n");
    UART::SendString(buffer);
    UART::SendString("\n\r");

    while (1)
    {
        GPIO::SetPin(21);
        delay(10000000);
        GPIO::ClearPin(21);
        delay(10000000);
    }
}
