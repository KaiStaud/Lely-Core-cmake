/*
2025-11-29
Mirror Inputs into object dictionary
*/
#ifndef DIGITAL_INPUTS_H
#define DIGITAL_INPUTS_H
#include <stdint.h>
#include <stdbool.h>

typedef struct{
    bool positive_limit_switch;
    bool negative_limit_switch;
    bool home_switch;
    bool drive_interlocked;
    unsigned int reseverd: 11;
    unsigned int digital_inputs: 15;
} digital_inputs;

uint32_t read_inputs(digital_inputs value);
#endif