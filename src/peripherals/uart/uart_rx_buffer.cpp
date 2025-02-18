#include "uart_rx_buffer.hpp"

u8 uart_rx_buffer::rx_buffer[uart_rx_buffer::BUFFER_SIZE] = {0};
u32 uart_rx_buffer::rx_head = 0;
void (*uart_rx_buffer::event_notifiers[uart_rx_buffer::EVENT_NOTIFIERS_SIZE])(void) = {nullptr}; // Inicializado a nullptr

bool uart_rx_buffer::buffer_overwritten = false;

const u8 *uart_rx_buffer::read_buffer()
{
    return rx_buffer;
}

void uart_rx_buffer::push_char(const char character)
{
    u32 next_head = (rx_head + 1) % BUFFER_SIZE;
    if (next_head == 0) // Cuando rx_head vuelve al principio
    {
        buffer_overwritten = true;
    }
    rx_buffer[rx_head] = character;
    rx_head = next_head;
}

u32 uart_rx_buffer::capacity()
{
    return BUFFER_SIZE;
}

bool uart_rx_buffer::subscribe(void (*callback)(void))
{
    for (u32 i = 0; i < EVENT_NOTIFIERS_SIZE; i++)
    {
        if (event_notifiers[i] == nullptr)
        {
            event_notifiers[i] = callback;
            return true;
        }
    }
    return false;
}

bool uart_rx_buffer::unsubscribe(void (*callback)(void))
{
    for (u32 i = 0; i < EVENT_NOTIFIERS_SIZE; i++)
    {
        if (event_notifiers[i] == callback)
        {
            event_notifiers[i] = nullptr;
            return true;
        }
    }
    return false;
}

void uart_rx_buffer::handle_interrupt(const char c)
{
    push_char(c);

    for (u32 i = 0; i < EVENT_NOTIFIERS_SIZE; i++)
    {
        if (event_notifiers[i] != nullptr)
            event_notifiers[i]();
    }
}

u32 uart_rx_buffer::get_head()
{
    return rx_head;
}

void uart_rx_buffer::copy_data(u8 *p_array, u32 length)
{
    if (length > BUFFER_SIZE)
    {
        uart::send_string("A uart_rx_buffer::copy_data() requested length was bigger than the BUFFER_SIZE");
        return;
    }

    if (!buffer_overwritten)
    {
        for (u32 i = 0; i < length; i++)
        {
            p_array[i] = rx_buffer[i];
        }
    }
    else
    {
        u32 current_idx = rx_head;
        for (u32 i = 0; i < length; i++)
        {
            p_array[i] = rx_buffer[current_idx];

            current_idx = (current_idx + 1) % BUFFER_SIZE;
        }
    }
}
