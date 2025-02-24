use crate::{
    c_str,
    kernel_apps::kernel_app::{KernelApp, KernelAppConfig},
    peripherals::uart::uart_tx::send_string,
};

pub struct UartTerminal {
    app_config: KernelAppConfig,
}

impl KernelApp for UartTerminal {
    fn set_app_config(&mut self, config: KernelAppConfig) {
        self.app_config = config;
    }

    fn event_system_start(&mut self) {
        send_string(c_str!("UartTerminal event_system_start\r\n"));
    }

    fn event_start(&mut self) {
        send_string(c_str!("UartTerminal event_start\r\n"));
    }

    fn event_loop(&mut self) {}
}
impl UartTerminal {
    pub fn new() -> Self {
        UartTerminal {
            app_config: KernelAppConfig::new(),
        }
    }
}
