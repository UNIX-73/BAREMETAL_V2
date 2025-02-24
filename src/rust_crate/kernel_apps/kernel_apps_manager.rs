const KERNEL_APPS_LENGTH: usize = 256;

use crate::{c_str, peripherals::uart::uart_tx::send_string};

use super::kernel_app::{KernelApp, KernelAppConfig};
pub struct KernelAppsManager<'a> {
    init: bool,
    apps: [Option<&'a mut dyn KernelApp>; KERNEL_APPS_LENGTH],
    system_started: bool,
    app_started: [bool; KERNEL_APPS_LENGTH],
}
impl<'a> KernelAppsManager<'a> {
    pub fn new() -> Self {
        Self {
            init: false,
            apps: [const { None }; KERNEL_APPS_LENGTH],
            system_started: false,
            app_started: [false; KERNEL_APPS_LENGTH],
        }
    }

    pub fn init(&mut self) {
        self.init = true;

        send_string(c_str!("KernelAppManager initialized\r\n"));

        while self.init {
            send_string(c_str!("aaaaaaaaaaaaaaaa\r\n"));

            if !self.system_started {
                send_string(c_str!("bbbb\r\n"));
                self.handle_event_system_start();
                self.system_started = true;
                send_string(c_str!("cccc\r\n"));
            }
            send_string(c_str!("KernelAppManager system_started\r\n"));

            send_string(c_str!("KernelAppManager event_start starting\r\n"));
            self.handle_event_start();
            self.handle_event_loop();
        }
    }

    pub fn subscribe(&mut self, new_app: &'a mut dyn KernelApp) {
        for i in 0..KERNEL_APPS_LENGTH {
            if self.apps[i].is_none() {
                self.apps[i] = Some(new_app);
                self.apps[i]
                    .as_mut()
                    .unwrap()
                    .set_app_config(KernelAppConfig { idx: i });
                self.app_started[i] = false;
                break;
            }
        }
    }

    pub fn unsubscribe(&mut self, app_idx: usize) {
        if app_idx < KERNEL_APPS_LENGTH {
            self.apps[app_idx] = None;
            self.app_started[app_idx] = false;
        }
    }

    pub fn handle_event_system_start(&mut self) {
        if self.init {
            send_string(c_str!("dddd\r\n"));
            for app in self.apps.iter_mut().flatten() {
                send_string(c_str!("eeeef\r\n"));

                app.event_system_start(); //Estoy seguro de que falla aquí
                send_string(c_str!("dddg\r\n"));
            }
        }
        send_string(c_str!("eeee\r\n"));
    }

    pub fn handle_event_start(&mut self) {
        if self.init {
            for i in 0..KERNEL_APPS_LENGTH {
                if self.apps[i].is_some() && !self.app_started[i] {
                    match self.apps[i].as_mut() {
                        Some(app) => {
                            send_string(c_str!("called start\n\r"));
                            app.event_start();
                            self.app_started[i] = true;
                        }
                        None => {}
                    }
                }
            }
            send_string(c_str!("ended start\n\r"));
        }
    }

    pub fn handle_event_loop(&mut self) {
        if self.init {
            for app in self.apps.iter_mut().flatten() {
                app.event_loop();
            }
        }
    }
}
