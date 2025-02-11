#include <common.h>
#include <peripherals/gpio.hpp>

void delay(u64 ticks)
{
    while (ticks > 0)
    {
        asm("nop");
        ticks--;
    }
}

extern "C" void kernel_main()
{
    GPIO::SetFunctionSelect(21, GPIO::FUNCTION_SELECT_OPTIONS::OUTPUT);

    while (1)
    {
        GPIO::SetPin(21);
        delay(500000);
        GPIO::ClearPin(21);
        delay(500000);
    }
}
