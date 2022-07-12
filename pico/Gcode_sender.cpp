#include "Gcode_sender.h"

void Gcode_sender::send_gcode(char* gcode){};

void moveTo(char* code_buffer, float x, float y){
    sprintf(code_buffer, "G00X%fY%f", x, y); 
    return;
};

void moveTo(char* code_buffer, float x, float y, float speed){
    sprintf(code_buffer, "G01X%fY%fF%f", x, y ,v); 
    return;
};