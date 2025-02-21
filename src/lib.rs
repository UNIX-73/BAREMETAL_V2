#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

unsafe extern "C" {
    //Para testear que se enlaza

    fn uart_send(char: u8);
}

#[unsafe(no_mangle)]
pub extern "C" fn hello_from_rust() {
    unsafe {
        let char_h: u8 = b'H';
        let char_e: u8 = b'e';
        let char_l: u8 = b'l';
        let char_o: u8 = b'o';
        let endl: u8 = b'\n';

        uart_send(char_h);
        uart_send(char_e);
        uart_send(char_l);
        uart_send(char_l);
        uart_send(char_o);
        uart_send(endl);
    }
}
