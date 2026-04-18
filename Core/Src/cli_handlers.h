#pragma once
#include "FreeRTOS.h"
#include <stdint.h>
#include <lely/co/sdev.h>


enum mode {
no_mode_selected =0,
profile_position_mode = 1,
profile_velocity_mode = 3 ,
profile_torque_mode = 4,
homing = 6,
cyclic_position_mode = 8,
cyclic_velocity_mode = 9,
cyclic_torque_mode = 10

};

BaseType_t WriteObject(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString);
BaseType_t SetTargetPosition(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString);
BaseType_t SetRPM(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString);
BaseType_t GetIO(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString);
                                 
uint8_t set_mode(enum mode p_mode, co_dev_t* dev);
uint32_t get_mode(co_dev_t* dev);
int arg_to_int(const char *pcCommandString,uint8_t arg_number);