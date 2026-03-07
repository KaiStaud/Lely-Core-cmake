#ifndef HOMING_H
#define HOMING_H
// https://doc.synapticon.com/node/sw5.0/motion_control/operation_modes/homing_modes.html

/*
Object ID 	Name 	Description
0x6098 	Homing Method 	Specifies the homing method that shall be used
0x607C 	Home Offset 	Offset from user’s zero position to the home position
0x6099 	Homing Speeds 	Speed with which to move during homing
0x609A 	Homing Acceleration 	Acceleration used during homing
0x6040 	Controlword 	Set bit 8 to 0 and bit 4 to 1 to start homing
0x6041 	Statusword 	Indicates the current state of the homing process
0x60FD 	Digital inputs 	Indicates the current state of the digital inputs
0x2210 	GPIO Pin configuration 	Configures the digital inputs for homing and limit switches
*/
#include <stdbool.h>
#include <stdint.h>

enum homing_direction{
    direction_negative,
    direction_positive,
};

enum homing_switch{
    negative_home_switch,
    positive_home_switch,
};

enum homing_switch_state{
    homing_switch_inactive,
    homing_switch_active
};

enum homing_result{
homing_performed, //Homing is performed
homing_interrupted, //Homing is interrupted or not started
homing_in_progress, //Homing confirmed, but target not yet reached
homing_completed,
homing_error_motor_turning, //Error detected, motor still turning
homing_error_motor_standstill //Error during homing, motor at standstill
};


typedef struct {
    bool state;
}gpio_state;

typedef struct{
    enum homing_direction dir;
}homing_parameters;

bool set_homing_params(homing_parameters parameters);
bool prepare_for_homing(bool enable);
bool enable_disable_output_stage(gpio_state* gpio);
bool get_direction();
enum homing_switch_state get_home_switch(gpio_state* gpio);
// This is the main loop reading inputs controlling the output stage
enum homing_result homing_loop(gpio_state* gpio);
uint8_t get_homing_statusword_highbyte(enum homing_result res);
#endif