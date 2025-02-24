#pragma once
#include "common.h"

extern "C"
{
    void rust_init();

    namespace rs_uart_irq
    {
        void handle_irq_uart_rx_buffer(const u8 c);
        void handle_irq_uart_rx();
    }
}