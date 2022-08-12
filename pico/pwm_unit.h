#ifndef takasago_pump
#define takasago_pump

#include <string.h>
#include "stdlib.h"
#include "cstdio"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

class Pwm_unit{
    public:
        Pwm_unit(uint gpio_pin){
            gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
            slice_num = pwm_gpio_to_slice_num(gpio_pin);
            pwm_set_wrap(slice_num, 50000);         
            pwm_set_chan_level(slice_num, PWM_CHAN_A, 25000);
            pwm_set_clkdiv(slice_num, 60);
        };
        uint slice_num;
        void enable();
        void set_warp(uint16_t);
        void set_duty(uint16_t);
        void set_clkdiv(float);
};


#endif