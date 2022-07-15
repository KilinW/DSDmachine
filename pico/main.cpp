#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "Gcode_sender.h"

#define PC_UART_ID uart0                // Computer <=> Pico
#define GRBL_UART_ID uart1              // Grbl <=> Pico
#define BAUD_RATE 115200                // Both pc and grbl baudrate are 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define PC_UART_TX_PIN 0
#define PC_UART_RX_PIN 1
#define GRBL_UART_TX_PIN 6
#define GRBL_UART_RX_PIN 7
#define LED_PIN 25

static char pc_buffer[1024];
static char grbl_buffer[1024];

void on_grbl_uart_rx(){
    uint8_t buffer_index = 0;
    while(uart_is_readable_within_us(GRBL_UART_ID, 100){
        grbl_buffer[buffer_index] = uart_getc(GRBL_UART_ID);
        buffer_index++;
    }
    grbl_buffer[buffer_index] = '\0';
    //

    // Unfinished

    //
};


void on_pc_uart_rx(){
    uint8_t buffer_index = 0;
    while(uart_is_readable_within_us(PC_UART_ID, 100)){
        pc_buffer[buffer_index] = uart_getc(PC_UART_ID);
        buffer_index++;
    }
    pc_buffer[buffer_index] = '\0';
    uart_puts(PC_UART_ID, "Received: ");
    uart_puts(PC_UART_ID, pc_buffer);
    //
    
    // Unfinished

    //
};

int main(){
    scanf();
    // Initialise UART 0
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    uart_init(PC_UART_ID, BAUD_RATE);
    //uart_init(GRBL_UART_ID, BAUD_RATE);

    gpio_set_function(PC_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PC_UART_RX_PIN, GPIO_FUNC_UART);
    //gpio_set_function(GRBL_UART_RX_PIN, GPIO_FUNC_UART);
    //gpio_set_function(GRBL_UART_TX_PIN, GPIO_FUNC_UART);         

    irq_set_exclusive_handler(UART0_IRQ, on_pc_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    
    //irq_set_exclusive_handler(UART1_IRQ, on_grbl_uart_rx);
    //irq_set_enabled(UART1_IRQ, true);

    uart_set_irq_enables(PC_UART_ID, true, false);
    //uart_set_irq_enables(GRBL_UART_ID, true, false);

    while (true) {
        tight_loop_contents();
    }
}
