#include "Gcode_sender.h"

void Gcode_sender::send_gcode(char* gcode){
    uart_puts(grbl_uart_port, gcode);
    uart_puts(grbl_uart_port, "\r");
    return;
};

void Gcode_sender::moveTo(char* code_buffer, float x, float y){
    sprintf(code_buffer, "G00X%.3fY%.3f", x, y); 
    return;
};

void Gcode_sender::moveTo(char* code_buffer, float x, float y, float speed){
    sprintf(code_buffer, "G01X%.3fY%.3fF%.3f", x, y, speed); 
    return;
};

void Gcode_sender::operate_pc_command(char* pcbuffer){
    float x = atof(strtok(&pcbuffer[1], " "));
    float y = atof(strtok(NULL," "));
    this->moveTo(grbl_send_buffer, x, y);
    this->send_gcode(grbl_send_buffer);
    return;
};