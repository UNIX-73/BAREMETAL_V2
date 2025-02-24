pub trait KernelApp {
    fn set_app_config(&mut self, config: KernelAppConfig);

    fn event_system_start(&mut self);

    fn event_start(&mut self);

    fn event_loop(&mut self);
}

pub struct KernelAppConfig {
    pub idx: usize,
}
impl KernelAppConfig {
    pub const fn new() -> Self {
        KernelAppConfig { idx: 0 }
    }
}
