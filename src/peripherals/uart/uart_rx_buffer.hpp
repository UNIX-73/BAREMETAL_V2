#pragma once

#include <common.h>
#include "peripherals/uart.hpp"

class uart_rx_buffer
{
public:
    uart_rx_buffer() = delete;

private:
    static bool buffer_overwritten;
    static u32 rx_head;

    static const u32 BUFFER_SIZE = 2048;
    static u8 rx_buffer[BUFFER_SIZE];

    static const u32 EVENT_NOTIFIERS_SIZE = 8;
    static void (*event_notifiers[EVENT_NOTIFIERS_SIZE])();

    static void push_char(const char character);

public:
    static const u8 *read_buffer();
    static u32 capacity();
    static bool subscribe(void (*callback)(void));
    static bool unsubscribe(void (*callback)(void));
    static void handle_interrupt(const char c);
    static u32 get_head();
    static void copy_data(u8 *p_array, u32 length);
};