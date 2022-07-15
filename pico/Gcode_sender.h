#ifndef gcodesender
#define gcodesender

#include <string.h>
#include "stdlib.h"
#include "cstdio"
#include "pico/stdlib.h"

class Gcode_sender{
    public:
        Gcode_sender(uart_inst_t *uart);

        static char grbl_send_buffer[1024];

        void send_gcode(char* gcode);
        void moveTo(char* code_buffer, float x, float y);
        void moveTo(char* code_buffer, float x, float y, float speed);
        void operate_pc_command(char* pcbuffer);
};


#endif