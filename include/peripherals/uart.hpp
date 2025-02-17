#include <common.h>
#include <peripherals/gpio.hpp>

// Estructura que mapea los registros del UART0
struct uart_regs
{
    reg32 DR;            // 0x00: Data Register
    reg32 RSRECR;        // 0x04: Receive Status / Error Clear Register
    reg32 reserved0[4];  // 0x08 - 0x14: Reservados
    reg32 FR;            // 0x18: Flag Register
    reg32 reserved1;     // 0x1C: Reservado
    reg32 ILPR;          // 0x20: (No se usa)
    reg32 IBRD;          // 0x24: Integer Baud rate divisor
    reg32 FBRD;          // 0x28: Fractional Baud rate divisor
    reg32 LCRH;          // 0x2C: Line Control Register
    reg32 CR;            // 0x30: Control Register
    reg32 IFLS;          // 0x34: Interrupt FIFO Level Select Register
    reg32 IMSC;          // 0x38: Interrupt Mask Set Clear Register
    reg32 RIS;           // 0x3C: Raw Interrupt Status Register
    reg32 MIS;           // 0x40: Masked Interrupt Status Register
    reg32 ICR;           // 0x44: Interrupt Clear Register
    reg32 DMACR;         // 0x48: DMA Control Register
    reg32 reserved2[13]; // 0x4C - 0x7C: Reservados
    reg32 ITCR;          // 0x80: Test Control Register
    reg32 ITIP;          // 0x84: Integration Test Input Register
    reg32 ITOP;          // 0x88: Integration Test Output Register
    reg32 TDR;           // 0x8C: Test Data Register
};

#define PERIPHERAL_BASE 0x3F000000
#define UART0_BASE (PERIPHERAL_BASE + 0x201000)
#define REGS_UART0 ((uart_regs *)(UART0_BASE))

// Para la habilitación de la interrupción del UART0 en el controlador
#define UART0_IRQ 57
#define IRQ_ENABLE_2 ((reg32 *)(PERIPHERAL_BASE + 0xB214))

namespace UART
{

    inline void send_string(const char *str);

    inline void uart_init()
    {
        // 1. Deshabilitar UART antes de configurarlo
        REGS_UART0->CR = 0;

        // 2. Configurar los pines GPIO para UART0
        GPIO::SetFunctionSelect(14, GPIO::FUNCTION_SELECT_OPTIONS::ALT0); // TX
        GPIO::SetFunctionSelect(15, GPIO::FUNCTION_SELECT_OPTIONS::ALT0); // RX

        GPIO::SetPullUpDown(14, 0); // Sin pull-up/down
        GPIO::SetPullUpDown(15, 0); // Sin pull-up/down

        // 3. Limpiar interrupciones pendientes
        REGS_UART0->ICR = 0x7FF;

        // 4. Configurar la tasa de baudios
        REGS_UART0->IBRD = 26; // Parte entera
        REGS_UART0->FBRD = 3;  // Parte fraccionaria

        // 5. Configurar el formato del frame: 8 bits, sin paridad, 1 bit de stop
        REGS_UART0->LCRH = (0b11 << 5) | (1 << 4); // FIFO enable y 8 bits

        // 6. Habilitar UART, TX y RX
        REGS_UART0->CR = (1 << 0) | (1 << 8) | (1 << 9);

        // 7. Habilitar interrupciones en UART
        REGS_UART0->IFLS &= ~(0b111 << 3);
        REGS_UART0->IFLS |= (0b000 << 3); // Se pone la interrupción para que salte cuando llegue algo siempre, aunque sea un byte

        REGS_UART0->IMSC = (1 << 4); // Habilitar la interrupción por recepción (RXIM)
        *IRQ_ENABLE_2 |= (1 << (UART0_IRQ - 32));

        UART::send_string("UART INITIALIZED\r\n");
    }

    inline void send(char c)
    {
        // Esperar mientras el FIFO de transmisión esté lleno
        while (REGS_UART0->FR & (1 << 5))
        {
            // Espera
        }
        REGS_UART0->DR = c;
    }

    inline void send_string(const char *str)
    {
        while (*str)
        {
            send(*str++);
        }
    }

    inline char receive()
    {
        // Esperar mientras el FIFO de recepción esté vacío
        while (REGS_UART0->FR & (1 << 4))
        {
            // Espera
        }
        return REGS_UART0->DR & 0xFF; // Se retorna solo el byte recibido
    }

    inline void receive_string(char *buffer, int max_length)
    {
        int i = 0;
        while (i < max_length - 1)
        {
            char c = receive();
            if (c == '\r' || c == '\n')
            {
                break;
            }
            buffer[i++] = c;
        }
        buffer[i] = '\0';
    }

    namespace IRQ_MANAGER
    {
        inline void handle_irq_mis()
        {
            //  RX
            if (REGS_UART0->MIS & (0b1 << 4))
            {
                REGS_UART0->ICR = (0b1 << 4);

                while (!(REGS_UART0->FR & (0b1 << 4)))
                {
                    volatile char c = REGS_UART0->DR;
                    UART::send('\n');
                    UART::send('\r');

                    UART::send(c); // No funciona creo
                }

                UART::send_string("received text\n\r"); // TODO: leer el texto que se recibe
            }
        }
    }
}
