#ifndef gcodesender
#define gcodesender

#include <string.h>
#include "pico/stdlib.h"

class Gcode_sender{
    public:
        Gcode_sender(int tx, int rx);
        void send_gcode(char* gcode);
        void moveTo(char* code_buffer, float x, float y);
        void moveTo(char* code_buffer, float x, float y, float speed);
    
};


#endif