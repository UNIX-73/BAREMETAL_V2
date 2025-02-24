unsafe extern "C" {
    pub fn uart_send(c: u8);
    pub fn uart_send_string(str: *const u8);
}
