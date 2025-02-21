#include "peripherals/uart.hpp"

extern "C"
{
    void uart_send(const char c)
    {
        uart::send(c);
    }

    void uart_send_string(const char *str)
    {
        uart::send_string(str);
    }
}