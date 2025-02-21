use crate::{extern_fn::rust::cpp::uart_send_string, kernel_apps::kernel_apps_manager::KernelApp};

pub struct UartConsole {
    test: bool,
}

impl KernelApp for UartConsole {
    fn event_system_start(&self) {
        if self.test {
            unsafe {
                uart_send_string("Hello from rust!\n\r".as_bytes().as_ptr());
            }
        }
    }

    fn event_start(&self) {}

    fn event_loop(&self) {}
}

impl UartConsole {
    pub const fn new() -> Self {
        UartConsole { test: true }
    }
}
