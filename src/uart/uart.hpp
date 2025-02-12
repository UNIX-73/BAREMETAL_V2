#include <common.h>
#include <peripherals/gpio.hpp>

// Definiciones de direcciones base (ajústalas según tu modelo y configuración)
#define PERIPHERAL_BASE 0x3F000000 // Puede variar en Pi 4 (0xFE000000)
#define UART0_BASE (PERIPHERAL_BASE + 0x201000)

#define UART0_DR_OFFSET 0x00
#define UART0_FR_OFFSET 0x18
#define UART0_IBRD_OFFSET 0x24
#define UART0_FBRD_OFFSET 0x28
#define UART0_LCRH_OFFSET 0x2C
#define UART0_CR_OFFSET 0x30
#define UART0_IMSC_OFFSET 0x38
#define UART0_ICR_OFFSET 0x44

// Macros para acceder a los registros de UART
#define P_UART0_DR ((reg32 *)(UART0_BASE + UART0_DR_OFFSET))     // Data Register
#define P_UART0_FR ((reg32 *)(UART0_BASE + UART0_FR_OFFSET))     // Flag Register
#define P_UART0_IBRD ((reg32 *)(UART0_BASE + UART0_IBRD_OFFSET)) // Integer Baud Rate Divisor
#define P_UART0_FBRD ((reg32 *)(UART0_BASE + UART0_FBRD_OFFSET)) // Fractional Baud Rate Divisor
#define P_UART0_LCRH ((reg32 *)(UART0_BASE + UART0_LCRH_OFFSET)) // Line Control Register
#define P_UART0_CR ((reg32 *)(UART0_BASE + UART0_CR_OFFSET))     // Control Register
#define P_UART0_IMSC ((reg32 *)(UART0_BASE + UART0_IMSC_OFFSET)) // Activar interrupciones
#define P_UART0_ICR ((reg32 *)(UART0_BASE + UART0_ICR_OFFSET))   // Interrupt Clear Register

#define UART0_IRQ 57
#define IRQ_ENABLE_2 ((reg32 *)(PERIPHERAL_BASE + 0xB214))

namespace UART
{
    inline void SendString(const char *str); // Para poder usarla en init predeclaramos

    inline void UartInit()
    {
        // 1. Deshabilitar UART antes de configurarlo
        *P_UART0_CR = 0;

        // 2. Configurar los pines GPIO para UART
        GPIO::SetFunctionSelect(14, GPIO::FUNCTION_SELECT_OPTIONS::ALT0); // TX
        GPIO::SetFunctionSelect(15, GPIO::FUNCTION_SELECT_OPTIONS::ALT0); // RX

        GPIO::SetPullUpDown(14, 0); // Sin pull-up/down
        GPIO::SetPullUpDown(15, 0); // Sin pull-up/down

        // 3. Limpiar interrupciones pendientes
        *P_UART0_ICR = 0x7FF;

        // 4. Configurar la tasa de baudios
        *P_UART0_IBRD = 26; // Parte entera
        *P_UART0_FBRD = 3;  // Parte fraccionaria

        // 5. Configurar el formato del frame: 8 bits, sin paridad, 1 bit de stop
        *P_UART0_LCRH = (1 << 4) | (1 << 5) | (1 << 6); // FEN = 1 (FIFO enable), WLEN = 11 (8 bits)

        // 6. Habilitar UART, TX y RX
        *P_UART0_CR = (1 << 0) | (1 << 8) | (1 << 9); // UART Enable, TX Enable, RX Enable

        /*
            HABILITAR INTERRUPTS
        */
        *P_UART0_IMSC = (1 << 4); // RXIM: habilitar interrupción en recepción
        *IRQ_ENABLE_2 |= (1 << (UART0_IRQ - 32));
        asm("msr daifclr, #2"); // Limpia la máscara de IRQ (bit I)

        UART::SendString("UART INITIALIZED\r\n");
    }

    inline void Send(char c)
    {
        while (*P_UART0_FR & (1 << 5)) // El bit 5 es el que indica si el FIFO está lleno
        {
            // TODO: interrupción creo
        }
        *P_UART0_DR = c;
    }

    inline void SendString(const char *str)
    {
        while (*str)
        {
            Send(*str++); // Envía el carácter actual
        }
    }

    inline char Receive()
    {
        while (*P_UART0_FR & (1 << 4)) // RXFE (bit 4): 1 si FIFO vacío
        {
            // Esperar a que llegue un dato
        }
        return *P_UART0_DR & 0xFF; // Leer el byte recibido
    }

    inline void ReceiveString(char *buffer, int max_length)
    {
        int i = 0;
        while (i < max_length - 1)
        {
            char c = Receive();
            if (c == '\r' || c == '\n') // Detenerse en newline
                break;
            buffer[i++] = c;
        }
        buffer[i] = '\0'; // Terminar string
    }

}
