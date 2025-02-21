use crate::kernel_apps::kernel_apps_manager::{KERNEL_APPS_LENGTH, KernelAppsManager};
use crate::kernel_apps::uart_console::uart_console::UartConsole;

pub extern "C" fn rust_init() {
    let mut kernel_apps_manager: KernelAppsManager<KERNEL_APPS_LENGTH> = KernelAppsManager::new();

    let uart_console: UartConsole = UartConsole::new();

    kernel_apps_manager.subscribe(&uart_console);

    kernel_apps_manager.init();
    kernel_apps_manager.handle_event_system_start();

    while kernel_apps_manager.is_init() {
        kernel_apps_manager.handle_event_start();
        kernel_apps_manager.handle_event_loop();
    }
}
