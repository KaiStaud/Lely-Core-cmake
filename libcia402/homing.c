#include "homing.h"

bool homing_active=false;
homing_parameters params;
bool set_homing_params(homing_parameters parameters){
params.dir=parameters.dir;
return true;
}


bool prepare_for_homing(bool enable)
{
    homing_active=enable;
    return true;
}

enum homing_switch_state get_home_switch(gpio_state* gpio){
    #ifndef SOFTWARE_TEST
    // read hardware gpio and set gpio->state
    #endif
    if(gpio->state){
        return homing_switch_active;

    }
    else
    {
        return homing_switch_inactive;
    }
}


bool enable_disable_output_stage(gpio_state* gpio)
{
    if( get_home_switch(gpio) == homing_switch_active)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool get_direction()
{
    return params.dir;
}

// This is the main loop reading inputs controlling the output stage
enum homing_result homing_loop(gpio_state* gpio){
    if( get_home_switch(gpio) == homing_switch_active)
    {
        // Read motor interface: enable and step frequency
        return homing_completed;
    }
    else
    {
        return homing_in_progress;
    }
}

uint8_t get_homing_statusword_highbyte(enum homing_result res)
{
    static uint8_t highbyte=0;
    
    if((res == homing_completed) | (res== homing_performed))
    {
        highbyte = (1<<4);
    }
    else if(( res == homing_error_motor_standstill) | ( res == homing_error_motor_turning))
    {
        highbyte = (1<<5);
    }
    else if (( res == homing_in_progress ) | ( res == homing_interrupted ))
    {
        highbyte = 0;
    }
    else
    {
        
    }
    return highbyte;
}
