#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "Gcode_sender.h"
#include "pump.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

#define PC_UART_ID uart0                // Computer <=> Pico
#define GRBL_UART_ID uart1              // Grbl <=> Pico
#define BAUD_RATE 115200                // Both pc and grbl baudrate are 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define PC_UART_TX_PIN 0
#define PC_UART_RX_PIN 1
#define GRBL_UART_TX_PIN 4
#define GRBL_UART_RX_PIN 5
#define hall_x_axis 10
#define hall_y_axis 11
#define LED_PIN 25
#define PUMP_FREQ_PIN 6
#define PUMP_VOLT_PIN 7


static char pc_buffer[1024];
static char grbl_buffer[1024];

Gcode_sender grbl_sender(GRBL_UART_ID);
Pump drop_gen(PUMP_FREQ_PIN, PUMP_VOLT_PIN);

void pc_print(const char* content){
    uart_puts(PC_UART_ID, content);
    return;
};

void grbl_print(const char* content){
    uart_puts(GRBL_UART_ID, content);
    return;
};

bool pc_command_intepret(char* command){
    switch(command[0]){
        case '#':
            grbl_sender.operate_pc_command(pc_buffer);
            return true;
        case '$':
            grbl_print(&command[1]);
        case '@':
            drop_gen.setfreq(atof(&command[1]));
            return true;
        default:
            return false;
    }
};

void on_grbl_uart_rx(){
    uint8_t buffer_index = 0;
    while(uart_is_readable_within_us(GRBL_UART_ID, 100)){
        grbl_buffer[buffer_index] = uart_getc(GRBL_UART_ID);
        buffer_index++;
    };
    grbl_buffer[buffer_index] = '\0';
    pc_print(grbl_buffer);
    return;
};

void on_pc_uart_rx(){
    uint8_t buffer_index = 0;
    while(uart_is_readable_within_us(PC_UART_ID, 100)){
        pc_buffer[buffer_index] = uart_getc(PC_UART_ID);
        buffer_index++;
    }
    pc_buffer[buffer_index] = '\0';
    pc_print("Command Received: ");
    pc_print(pc_buffer);
    pc_print("\n");

    if(!pc_command_intepret(pc_buffer)){
        pc_print("ERROR: Command Not Recognized\n");
    };
    return;
};

void on_xy_reach_limit(uint gpio, uint32_t events){
    if(gpio==10 && events==GPIO_IRQ_EDGE_FALL){
        uart_putc(PC_UART_ID, (char)0x85);
    }
};



void io_init(){

    //Set up UART port for  Pico <=> PC and Pico <=> GRBL. 
    uart_init(PC_UART_ID, BAUD_RATE);               //Initialize UART
    uart_init(GRBL_UART_ID, BAUD_RATE);             

    //Set gpio function on specific pin
    gpio_set_function(PC_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PC_UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GRBL_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GRBL_UART_RX_PIN, GPIO_FUNC_UART);
    

    //Set up interrupt function and enable it
    irq_set_exclusive_handler(UART0_IRQ, on_pc_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    irq_set_exclusive_handler(UART1_IRQ, on_grbl_uart_rx);
    irq_set_enabled(UART1_IRQ, true);

    //Enable interrupt when input and disable when output
    uart_set_irq_enables(PC_UART_ID, true, false);
    uart_set_irq_enables(GRBL_UART_ID, true, false);

    //Limit switch sensor init
    gpio_init(hall_x_axis);
    gpio_init(hall_y_axis);

    //Set hall sensor to pull up
    gpio_pull_up(hall_x_axis);
    gpio_pull_up(hall_y_axis);

    //Set hall sensor interrupt callback
    gpio_set_irq_enabled_with_callback(hall_x_axis, GPIO_IRQ_EDGE_FALL, true, on_xy_reach_limit);
    gpio_set_irq_enabled_with_callback(hall_y_axis, GPIO_IRQ_EDGE_FALL, true, on_xy_reach_limit);

    drop_gen.enable();
};

int main(){

    io_init();
    //Main loop
    while (true) {
        tight_loop_contents();
    }
}
