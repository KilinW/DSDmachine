#include "pwm_unit.h"

void Pwm_unit::set_warp(uint16_t warp){
    pwm_set_wrap(this->slice_num, warp);
    return;
};

void Pwm_unit::set_duty(uint16_t level){
    pwm_set_chan_level(this->slice_num, PWM_CHAN_A, level);
    return;
};

void Pwm_unit::set_clkdiv(float divider){
    pwm_set_clkdiv(this->slice_num, divider);
    return;
};

void Pwm_unit::enable(){
    pwm_set_enabled(this->slice_num, true);
    return;
}