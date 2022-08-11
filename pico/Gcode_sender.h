#ifndef gcodesender
#define gcodesender

#include <string.h>
#include "stdlib.h"
#include "cstdio"
#include "pico/stdlib.h"
#include "hardware/uart.h"

class Gcode_sender{
    public:
        Gcode_sender(uart_inst_t* uart){
            grbl_uart_port = uart;
        };

        uart_inst_t* grbl_uart_port;
        char grbl_send_buffer[1024];

        void send_gcode(char* gcode);
        void moveTo(char* code_buffer, float x, float y);
        void moveTo(char* code_buffer, float x, float y, float speed);
        void operate_pc_command(char* pcbuffer);
        float pwm_scale(char* pc_buffer);
};


#endif