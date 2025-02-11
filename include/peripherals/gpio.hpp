#pragma once

#include <stdint.h>

#define GPIO_BASE 0x3F200000
#define GPIO_FSEL_OFFSET 0x00 // 4GPIO fsel de {00}[000][000][000][000][000][000][000][000][000][000] 10 pines
#define GPIO_SET_OFFSET 0x1C
#define GPIO_CLR_OFFSET 0x28
#define GPIO_LEV_OFFSET 0x34
// Para pull-up/down
#define GPIO_PUD_OFFSET 0x94
#define GPIO_PUDCLK_OFFSET 0x98
// Definimos las direcciones específicas para cada registro como macros
#define P_GPIO_BASE ((volatile uint32_t *)GPIO_BASE)
#define P_GPIO_FSEL ((volatile uint32_t *)(GPIO_BASE + GPIO_FSEL_OFFSET))
#define P_GPIO_SET ((volatile uint32_t *)(GPIO_BASE + GPIO_SET_OFFSET))
#define P_GPIO_CLR ((volatile uint32_t *)(GPIO_BASE + GPIO_CLR_OFFSET))
#define P_GPIO_LEV ((volatile uint32_t *)(GPIO_BASE + GPIO_LEV_OFFSET))

#define PIN_COUNT 40

enum FUNCTION_SELECT_OPTIONS
{
    INPUT = 0b000,  // Modo de entrada
    OUTPUT = 0b001, // Modo de salida
    ALT0 = 0b100,   // Función alternativa 0
    ALT1 = 0b101,   // Función alternativa 1
    ALT2 = 0b110,   // Función alternativa 2
    ALT3 = 0b111,   // Función alternativa 3
    ALT4 = 0b011,   // Función alternativa 4
    ALT5 = 0b010    // Función alternativa 5
};

namespace GPIO
{
    enum FUNCTION_SELECT_OPTIONS
    {
        INPUT = 0b000,  // Modo de entrada
        OUTPUT = 0b001, // Modo de salida
        ALT0 = 0b100,   // Función alternativa 0
        ALT1 = 0b101,   // Función alternativa 1
        ALT2 = 0b110,   // Función alternativa 2
        ALT3 = 0b111,   // Función alternativa 3
        ALT4 = 0b011,   // Función alternativa 4
        ALT5 = 0b010    // Función alternativa 5
    };

    inline void SetFunctionSelect(uint32_t pin, FUNCTION_SELECT_OPTIONS function)
    {
        if (pin > 39)
            return; // TODO: Agregar un gestor de logs o manejo de error

        int fselRegister = pin / 10; // FSEL0, FSEL1, FSEL2, etc.
        int shiftAmount = (pin % 10) * 3;

        // Limpia los 3 bits correspondientes al pin
        P_GPIO_BASE[fselRegister] &= ~(0b111 << shiftAmount);
        // Establece la función, asegurando que solo se usan 3 bits
        P_GPIO_BASE[fselRegister] |= ((function & 0b111) << shiftAmount);
    }

    inline void SetPin(uint32_t pin)
    {
        if (pin < 0 || pin > 39)
            return;

        uint32_t setRegister = pin / 32;
        uint32_t bitPosition = pin % 32;

        P_GPIO_SET[setRegister] = (0b1 << bitPosition);
    };

    inline void ClearPin(uint32_t pin)
    {
        if (pin < 0 || pin > 39)
            return;

        uint32_t clrRegister = pin / 32;
        uint32_t bitPosition = pin % 32;

        P_GPIO_CLR[clrRegister] = (0b1 << bitPosition);
    };

    inline uint32_t ReadPin(uint32_t pin)
    {
        if (pin < 0 || pin > 39)
            return 0;

        uint32_t levRegister = pin / 32;
        uint32_t bitPosition = pin % 32;

        return (P_GPIO_LEV[levRegister] >> bitPosition) & 0b1;
    };

    inline void SetPullUpDown(uint32_t pin, uint32_t pud)
    {
        volatile uint32_t *gppud = reinterpret_cast<volatile uint32_t *>(GPIO_BASE + GPIO_PUD_OFFSET);
        volatile uint32_t *gppudclk = reinterpret_cast<volatile uint32_t *>(GPIO_BASE + GPIO_PUDCLK_OFFSET);

        *gppud = pud; // Escribimos la configuración (0 para desactivar)
        // Espera corta para que la señal se estabilice
        for (volatile int i = 0; i < 150; i++)
            ;
        // Aplicamos la configuración solo al pin indicado
        *gppudclk = (1 << pin);
        for (volatile int i = 0; i < 150; i++)
            ;
        // Se limpian los registros para finalizar
        *gppud = 0;
        *gppudclk = 0;
    }
}
