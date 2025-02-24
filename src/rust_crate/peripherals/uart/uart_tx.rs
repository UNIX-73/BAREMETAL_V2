use crate::extern_fn::uart::{uart_send, uart_send_string};

/// Macro que permite definir cadenas literales terminadas en `\0`
#[macro_export]
macro_rules! c_str {
    ($s:expr) => {
        concat!($s, "\0") // Funciona solo con literales de cadena
    };
}

pub fn send_char(c: u8) {
    unsafe {
        uart_send(c);
    }
}

/// Envía un `&str`, asegurando que termine en `\0`
pub fn send_string(s: &str) {
    if s.as_bytes().last() == Some(&0) {
        // Si la cadena ya termina en `\0`, la enviamos directamente
        unsafe {
            uart_send_string(s.as_ptr());
        }
    } else {
        // Buffer local en la pila
        const MAX_LEN: usize = 1024;
        let mut buf = [0u8; MAX_LEN];
        let len = s.len();

        if len + 1 >= MAX_LEN {
            return; // Evita desbordamientos
        }

        buf[..len].copy_from_slice(s.as_bytes());
        buf[len] = 0; // Añade terminador `\0`

        unsafe {
            uart_send_string(buf.as_ptr());
        }
    }
}
