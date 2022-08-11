#ifndef takasago_pump
#define takasago_pump

#include <string.h>
#include "stdlib.h"
#include "cstdio"
#include "pico/stdlib.h"
#include "hardware/pwm.h"

class Pump{
    public:
        Pump(uint frequency_pin, uint voltage_pin){
            gpio_set_function(frequency_pin, GPIO_FUNC_PWM);
            slice_num = pwm_gpio_to_slice_num(frequency_pin);
            pwm_set_wrap(slice_num, 50000);         
            pwm_set_chan_level(slice_num, PWM_CHAN_A, 25000);
            pwm_set_clkdiv(slice_num, 50);
        };
        uint slice_num;
        void enable();
        void setfreq(float freq);
};


#endif