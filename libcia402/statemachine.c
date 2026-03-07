#include "statemachine.h"

int add(int a, int b) {
    return a + b;
}
/* Global variables */
static drive_state internal_state=drive_state_switch_on_disabled;

/* Functions */
drive_state check_fault_conditions(uint16_t controlword)
{
    // Check if there are recessive bits.
    uint8_t fault_conditions=~(controlword);
    if(fault_conditions & 0x6)
    {
        return drive_state_switch_on_disabled;
    }
    else if (fault_conditions & 0x1){
        return drive_state_ready_to_switch_on;        
    }
    // TODO: Handle Fault Reset and Halt bits
    else // No recessive bits
    {
        return internal_state;
    }
}

int run_transition(uint16_t controlword)
{
/* 
    Check if the control word matches with any of the control_commands.
    If multiple matches are available prioritize use the one with most priviledges.
*/
 const drive_state drive_states[4]={
                            drive_state_switch_on_disabled,     // Bit 0: ready to switch on
                            drive_state_ready_to_switch_on,     // Bit 1: enable voltage
                            drive_state_switched_on,            // Bit 2: quick stop / switched on
                            drive_state_operation_enabled,      // Bit 3 : enable operation
                            };
// Find the most significant bit with value=1                            
    for(int i=4;i>=0;i--)
    {
        if((controlword<<1) & (1<<i))
        {
        internal_state=drive_states[i-1];   
        internal_state=check_fault_conditions(controlword);
        return true;   
        }
    }
    if(controlword & (1<<7))
    {
        internal_state=drive_state_switch_on_disabled;
    }
        return false;

}

drive_state get_state()
{
    return internal_state;
}

fault_inputs get_fault_inputs(gpio_state* gpio){
    if (get_home_switch(gpio) == homing_switch_active ){
        internal_state=drive_state_fault_reaction_active;
        return fault_input_right_switch_active;
    }
    return 0;
}

uint8_t get_statusword_lowbyte(drive_state state){
    switch (state)
    {
        case drive_state_not_ready_to_switch_on:
            return 0;
            break;
        case drive_state_ready_to_switch_on:
            return (1<<0)|(1<<5);
            break;    
        case drive_state_switch_on_disabled:
            return (1<<6);
            break;
        case drive_state_switched_on:
            return (0x3) | (1<<5);
            break;
        case drive_state_operation_enabled:
            return (0x7)| (1<<5);
            break;
        case drive_state_quickstop_enabled:
            return 0x7;
                break;
        case drive_state_fault_reaction_active:
            return 0xF;
            break;
        case drive_state_fault: 
            return 0x8;
            break;    
        default:
            break;
    }
    return 0;
}