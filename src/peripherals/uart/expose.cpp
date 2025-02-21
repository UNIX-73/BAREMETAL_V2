#include "peripherals/uart.hpp"

extern "C"
{
    void uart_send(const char c)
    {
        uart::send(c);
    }
}