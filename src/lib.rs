#![no_std]
#![no_main]

use core::panic::PanicInfo;
mod extern_fn;
mod kernel_apps;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}
