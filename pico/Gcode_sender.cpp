#include "Gcode_sender.h"

void Gcode_sender::send_gcode(char* gcode){};

void Gcode_sender::moveTo(char* code_buffer, float x, float y){
    sprintf(code_buffer, "G00X%fY%f", x, y); 
    return;
};

void Gcode_sender::moveTo(char* code_buffer, float x, float y, float speed){
    sprintf(code_buffer, "G01X%fY%fF%f", x, y, speed); 
    return;
};

void Gcode_sender::operate_pc_command(char* pcbuffer){
    float x = atof(strtok(&pcbuffer[1], " "));
    float y = atof(strtok(NULL," "));
    this->moveTo(grbl_send_buffer, x, y);
    return;
};