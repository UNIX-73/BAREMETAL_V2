#![no_std]
#![no_main]

use core::panic::PanicInfo;
mod extern_fn;
use kernel_apps::{apps::uart_terminal::UartTerminal, kernel_apps_manager::KernelAppsManager};
use peripherals::uart::uart_tx::send_string;
mod kernel_apps;
mod peripherals;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    send_string(c_str!("Paniqued\r\n"));
    loop {}
}

#[unsafe(no_mangle)]
pub extern "C" fn rust_init() {
    send_string(c_str!("Rust initialized\n\r"));

    let mut kernel_apps_manager = KernelAppsManager::new();

    let mut uart_terminal = UartTerminal::new();

    send_string(c_str!("Calling subscribe\r\n"));
    kernel_apps_manager.subscribe(&mut uart_terminal);
    send_string(c_str!("Called subscribe\r\n"));

    kernel_apps_manager.init();

    loop {}
}
