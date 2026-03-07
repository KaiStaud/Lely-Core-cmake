#pragma once
/*
26-10-2025
Bits,States and Commands for running the cia402 statemachine.
*/
#include <stdint.h>
#include <stdbool.h>
#include "homing.h"

int add(int a, int b);

typedef enum{
    no_fault_detected,
    fault_input_left_switch_active,
    fault_input_right_switch_active,
    fault_input_hardstop_active
}fault_inputs;

typedef enum 
{
    drive_state_not_ready_to_switch_on,
    drive_state_switch_on_disabled,
    drive_state_ready_to_switch_on,
    drive_state_switched_on,
    drive_state_operation_enabled,
    drive_state_quickstop_enabled,
    drive_state_fault_reaction_active,
    drive_state_fault
}drive_state;

typedef enum
{
    switched_on = 0,
    enable_voltage = 1,
    quickstop = 2,
    enable_operation = 3,
    operation_mode_specific_1 = 4,
    operation_mode_specific_2 = 5,
    operation_mode_specific_3 = 6,
    fault_reset = 7,
    halt = 8,
}control_bits;

#define _SET_BIT(bit) 1<<bit
#define _RESET_BIT(bit) 0<<bit

typedef enum
{
    cmd_shutdown = _SET_BIT(enable_voltage) | _SET_BIT(quickstop),
    cmd_switch_on = _SET_BIT(switched_on) | _SET_BIT(enable_voltage) | _SET_BIT(quickstop),
    cmd_enable_operation =  _SET_BIT(enable_operation)| _SET_BIT(switched_on) | _SET_BIT(enable_voltage) | _SET_BIT(quickstop),
    cmd_disable_operation = _RESET_BIT(enable_voltage) | _SET_BIT(fault_reset),
    cmd_reset_fault = _SET_BIT(fault_reset)
}control_commands;


bool validate_command(uint16_t control_word, control_commands mask);
/*
Receive a controlword and find the next state.
*/
int run_transition(uint16_t controlword);
drive_state set_state(control_commands cmd);
drive_state get_state();
fault_inputs get_fault_inputs(gpio_state* gpio);
uint8_t get_statusword_lowbyte(drive_state state);
