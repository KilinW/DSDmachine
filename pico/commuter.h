#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"

#define PC_UART_ID uart0                // Computer <=> Pico
#define GRBL_UART_ID uart1              // Grbl <=> Pico
#define BAUD_RATE 115200                // Both pc and grbl baudrate are 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define PC_UART_TX_PIN 0
#define PC_UART_RX_PIN 1
#define GRBL_UART_TX_PIN 6
#define GRBL_UART_RX_PIN 7

class Commuter{
    public:
        Commuter();

        void on_pc_uart_rx();
        void on_grbl_uart_rx();

        begin(){
            uart_init(PC_UART_ID, BAUD_RATE);
            uart_init(GRBL_UART_ID, BAUD_RATE);

            gpio_set_function(PC_UART_TX_PIN, GPIO_FUNC_UART);
            gpio_set_function(PC_UART_RX_PIN, GPIO_FUNC_UART);
            gpio_set_function(GRBL_UART_RX_PIN, GPIO_FUNC_UART);
            gpio_set_function(GRBL_UART_TX_PIN, GPIO_FUNC_UART);         

            irq_set_exclusive_handler(UART0_IRQ, on_pc_uart_rx);
            irq_set_enable(UART0_IRQ, true);
            
            irq_set_exclusive_handler(UART1_IRQ, on_grbl_uart_rx);
            irq_set_enable(UART1_IRQ, true);

            uart_set_irq_enables(PC_UART_ID, true, false);
            uart_set_irq_enables(GRBL_UART_ID, true, false);
        };
}