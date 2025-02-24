use super::uart_tx::send_string;

const BUFFER_LENGTH: usize = 2048;
static mut UART_RX_BUFFER: [u8; BUFFER_LENGTH] = [0u8; BUFFER_LENGTH];
static mut UART_RX_IDX: usize = 0;

#[unsafe(no_mangle)]
pub extern "C" fn handle_irq_uart_rx_buffer(char: u8) {
    if unsafe { UART_RX_IDX } + 1 >= BUFFER_LENGTH {
        unsafe {
            UART_RX_BUFFER = [0u8; BUFFER_LENGTH];
            UART_RX_IDX = 0;
        }
        send_string("Buffer full\r\n\0");
        return;
    }

    unsafe {
        UART_RX_BUFFER[UART_RX_IDX] = char;
        UART_RX_IDX += 1;
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn handle_irq_uart_rx() {}
