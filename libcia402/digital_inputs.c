#include "digital_inputs.h"
uint16_t entry=0;
uint32_t read_inputs(digital_inputs value){
    entry = value.negative_limit_switch |
            (value.positive_limit_switch << 1) |
            (value.home_switch << 2) |
            (value.drive_interlocked <<3) |
            value.digital_inputs <<16;
    return entry;
}
