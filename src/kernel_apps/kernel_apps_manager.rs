use crate::extern_fn;

pub const KERNEL_APPS_LENGTH: usize = 128;

pub struct KernelAppsManager<'a, const S: usize> {
    init: bool,
    apps: [Option<&'a dyn KernelApp>; S],
    starts: [bool; S],
}
impl<'a, const S: usize> KernelAppsManager<'a, S> {
    pub const fn new() -> Self {
        KernelAppsManager {
            init: false,
            apps: [None; S],
            starts: [false; S],
        }
    }

    pub fn subscribe(&mut self, app: &'a dyn KernelApp) {
        for i in 0..S {
            if self.apps[i].is_none() {
                self.apps[i] = Some(app);
                self.starts[i] = false;
                return;
            }
        }

        unsafe { extern_fn::rust::cpp::uart_send_string("aaa".as_bytes().as_ptr()) }
    }

    pub fn unsubscribe(&mut self, app: &'a dyn KernelApp) {
        for i in 0..S {
            if let Some(app) = self.apps[i] {
                self.apps[i] = None;
                self.starts[i] = false;
                return;
            }
        }

        unsafe { extern_fn::rust::cpp::uart_send_string("aaa".as_bytes().as_ptr()) }
    }

    pub fn init(&mut self) {
        self.init = true;
    }

    pub fn stop(&mut self) {
        self.init = false;
    }

    pub fn is_init(&self) -> bool {
        self.init
    }
    pub fn handle_event_system_start(&mut self) {
        if !self.init {
            return;
        }

        for app in self.apps.iter() {
            if let Some(app) = app {
                app.event_system_start();
            }
        }
    }

    pub fn handle_event_start(&mut self) {
        if !self.init {
            return;
        }

        for i in 0..S {
            if let Some(app) = self.apps[i] {
                if self.starts[i] == false {
                    app.event_start();
                    self.starts[i] = true
                }
            }
        }
    }

    pub fn handle_event_loop(&mut self) {
        if !self.init {
            return;
        }

        for app in self.apps.iter() {
            if let Some(app) = app {
                app.event_loop();
            }
        }
    }
}

pub trait KernelApp {
    fn event_system_start(&self) {}
    fn event_start(&self) {}
    fn event_loop(&self) {}
}
