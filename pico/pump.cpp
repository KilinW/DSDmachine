#include "pump.h"

void Pump::setfreq(float freq){
    pwm_set_clkdiv(slice_num, 2500/freq);
    return;
};

void Pump::enable(){
    pwm_set_enabled(this->slice_num, true);
    return;
}