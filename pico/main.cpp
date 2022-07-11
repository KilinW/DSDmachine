#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "Gcode_sender.h"

int main(){

    const int led_pin = 25;

    // Initialise UART 0
    gpio_init(led_pin);
    gpio_set_dir(led_pin, GPIO_OUT);
    stdio_init_all();
    char a[10];
    
    while (true) {
        
        // Blink LED
        //printf("Blinking!\r\n");
        printf("Hello!");
        gpio_put(led_pin, true);
        sleep_ms(10);
        gpio_put(led_pin, false);
        sleep_ms(100);
    }
}
