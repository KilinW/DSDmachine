#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "Gcode_sender.h"
#include "pwm_unit.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"

//Include adc header
#include "hardware/adc.h"
#include "hardware/dma.h"

//Include multicore
#include "pico/multicore.h"

//Include selfmade header
#include "Gcode_sender.h"

#define PC_UART_ID uart0                // Computer <=> Pico
#define GRBL_UART_ID uart1              // Grbl <=> Pico
#define BAUD_RATE 115200                // Both pc and grbl baudrate are 115200

// We are using pins 0 and 1, but see the GPIO function select table in the
// datasheet for information on which other pins can be used.
#define PC_UART_TX_PIN 0
#define PC_UART_RX_PIN 1
#define GRBL_UART_TX_PIN 4
#define GRBL_UART_RX_PIN 5
#define HALL_X_AXIS 10
#define HALL_Y_AXIS 11
#define LED_PIN 25
#define PUMP_FREQ_PIN 6
#define PUMP_VOLT_PIN 7
#define SIGNAL_SIMUL_PIN 8


//Oscilloscope input channel
#define ADC_INPUT_CHANNEL 0                  // It's channel 26+"0", which is the first adc channel
#define ADC_BUFFER_DEPTH 1

uint dma_chan;

char pc_buffer[1024];
char grbl_buffer[1024];
uint8_t adc_buffer[ADC_BUFFER_DEPTH];

// TODO: Pull GPIO23 high to reduce the noise of adc

Gcode_sender grbl_sender(GRBL_UART_ID);
Pwm_unit signal_simulate(SIGNAL_SIMUL_PIN);
Pwm_unit drop_gen_freq(PUMP_FREQ_PIN);
Pwm_unit drop_gen_volt(PUMP_VOLT_PIN);

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
            drop_gen_freq.set_clkdiv(atof(&command[1]));
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
    pc_print("Magnent approached");
    if(gpio==10 && events==GPIO_IRQ_EDGE_FALL){
        uart_putc(PC_UART_ID, (char)0x85);
    }
};

void pc_uart_init(){
    // Set up UART port for  Pico <=> PC and Pico <=> GRBL. 
    uart_init(PC_UART_ID, BAUD_RATE);               // Initialize UART
    uart_init(GRBL_UART_ID, BAUD_RATE);             

    // Set gpio function on specific pin
    gpio_set_function(PC_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(PC_UART_RX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GRBL_UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(GRBL_UART_RX_PIN, GPIO_FUNC_UART);

    // Set up interrupt function and enable it
    irq_set_exclusive_handler(UART0_IRQ, on_pc_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    irq_set_exclusive_handler(UART1_IRQ, on_grbl_uart_rx);
    irq_set_enabled(UART1_IRQ, true);

    // Enable interrupt when input and disable when output
    uart_set_irq_enables(PC_UART_ID, true, false);
    uart_set_irq_enables(GRBL_UART_ID, true, false);
};

void hall_sensor_init(){
    // Limit switch sensor init
    gpio_init(HALL_X_AXIS);
    gpio_init(HALL_Y_AXIS);

    // Set hall sensor to pull up
    gpio_pull_up(HALL_X_AXIS);
    gpio_pull_up(HALL_Y_AXIS);

    // Set hall sensor interrupt callback
    gpio_set_irq_enabled_with_callback(HALL_X_AXIS, GPIO_IRQ_EDGE_FALL, true, on_xy_reach_limit);
    gpio_set_irq_enabled_with_callback(HALL_Y_AXIS, GPIO_IRQ_EDGE_FALL, true, on_xy_reach_limit);
};

void on_pizo_dma_finished(){
    signal_simulate.set_duty(adc_buffer[0]);
    dma_hw->ints0 = 1u << dma_chan;
    dma_channel_set_write_addr(dma_chan, &adc_buffer[0], true);
};

void signal_simulate_init(){
    signal_simulate.set_warp(255);
    signal_simulate.set_duty(128);
    signal_simulate.set_clkdiv(1.0);
    signal_simulate.enable();
};

void piezo_adc_init(){
    // ADC input init
    adc_init();
    adc_gpio_init(26 + ADC_INPUT_CHANNEL);
    adc_select_input(ADC_INPUT_CHANNEL);
    adc_fifo_setup(
        true,
        true,
        1,
        false,
        true
    );
    adc_set_clkdiv(0);

    // Setting up DMA
    dma_chan = dma_claim_unused_channel(true);
    dma_channel_config dma_cfg = dma_channel_get_default_config(dma_chan);

    // Reset something in dma_channel_get_default_config.
    channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&dma_cfg, false);
    channel_config_set_write_increment(&dma_cfg, false);

    // Reset the source of data transfer request
    channel_config_set_dreq(&dma_cfg, DREQ_ADC);

    dma_channel_configure(dma_chan, &dma_cfg,
        adc_buffer,             // destination of data
        &adc_hw->fifo,          // source of data
        ADC_BUFFER_DEPTH,       // transfer count
        false                   // don't start immediately
    );

    //DAM will raise DMA_IRQ_0 when it finished a block
    dma_channel_set_irq0_enabled(dma_chan, true);

    //Attach a handler we made to DMA_IRQ_0
    irq_set_exclusive_handler(DMA_IRQ_0, on_pizo_dma_finished);
    irq_set_enabled(DMA_IRQ_0, true);

    adc_run(true);
    on_pizo_dma_finished();

    // Once DMA finishes, stop any new conversions from starting, and clean up
    // the FIFO in case the ADC was still mid-conversion.
    /*
    dma_channel_wait_for_finish_blocking(dma_chan);
    grbl_print("Capture finished\n");
    adc_run(false);
    adc_fifo_drain();
    */

};

void io_init(){

    pc_uart_init();
    hall_sensor_init();
    piezo_adc_init();
    signal_simulate_init();
    drop_gen_freq.enable();
    signal_simulate.enable();
};

int main(){

    io_init();

    //Main loop
    while (true) {
        tight_loop_contents();
    }
}
