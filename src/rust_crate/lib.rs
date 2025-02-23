#![no_std]
#![no_main]

use core::panic::PanicInfo;
mod extern_cpp;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn rust_init() -> ! {
    for _ in 0..10 {
        unsafe {
            extern_cpp::uart_send(b'H');
        }
    }

    loop {}
}
