#include "Gcode_sender.h"
#include "string"

void Gcode_sender::send_gcode(char* gcode){};

void moveTo(char* code_buffer, float x, float y){
    sprintf(code_buffer, "G00X%fY%f", x, y); 
    return;
};

void moveTo(char* code_buffer, float x, float y, float speed){
    sprintf(code_buffer, "G01X%fY%fF%f", x, y, speed); 
    return;
};

void operate_pc_command(char* pcbuffer){
    char* word = strtok(pcbuffer[1]," ");
    
    char* word = strtok(NULL," ");
    float x = stof(word);

    char* word = strtok(NULL," ");
    float y = stof(word);

    moveTo(pc_buffer, x, y);
    
};