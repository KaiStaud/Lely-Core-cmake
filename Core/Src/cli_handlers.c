#include "cli_handlers.h"
#include <stdlib.h>  // for strtol
#include <lely/co/co.h>
#include <lely/can/net.h>
#include "FreeRTOS_CLI.h"
#include "extern_variables.h"

BaseType_t WriteObject(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
  //if (argc != 4) {
    //LOG(CLI_LOG_CAT1, "write_object [Index] [SubIndex] [value]");
    return 1;
  //}
  //co_obj_t* obj = co_dev_find_obj(dev, index);
  //co_obj_set_val(obj, subindex, &value, sizeof(value));
  return 1;
}

BaseType_t SetTargetPosition(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
  //LOG(CLI_LOG_CAT1, "Setting new target position");
  set_mode(profile_position_mode, dev);
  int value = arg_to_int(pcCommandString,1);
  co_obj_t* obj = co_dev_find_obj(dev, 0x607A);
  int bytes_written = co_obj_set_val(obj, 00, &value, sizeof(value));
  if (bytes_written) {
    //LOG(CLI_LOG_CAT1, "New target position %i", value);
    return 0;
  } else {
    //LOG(CLI_LOG_CAT1, "Failed to write to OD: Error %s", errno2str(get_errc()));
    return 1;
  }
}

BaseType_t SetRPM(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
  //LOG(CLI_LOG_CAT1, "Setting new target velocity");
  set_mode(profile_velocity_mode, dev);
  int value = arg_to_int(pcCommandString,1);
  co_obj_t* obj = co_dev_find_obj(dev, 0x60FF);
  int bytes_written = co_obj_set_val(obj, 00, &value, sizeof(value));

  if (bytes_written) {
    //LOG(CLI_LOG_CAT1, "New target velocity %i", value);
    return 0;
  } else {
    //LOG(CLI_LOG_CAT1, "Failed to write to OD: Error %s", errno2str(get_errc()));
    return 1;
  }
  return 0;
}

BaseType_t GetIO(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
  return 0;
  ;
}

uint8_t set_mode(enum mode p_mode, co_dev_t* dev) {
  co_obj_t* obj = co_dev_find_obj(dev, 0x6060);
  uint32_t value = (uint32_t)p_mode;
  return co_obj_set_val_u32(obj, 0x00, value);
}

uint32_t get_mode(co_dev_t* dev) {
  co_obj_t* obj = co_dev_find_obj(dev, 0x6060);
  return co_obj_get_val_u32(obj, 0);
}

int arg_to_int(const char *pcCommandString,uint8_t arg_number) {
    BaseType_t xParameterStringLength;
    const char *pcParameter = FreeRTOS_CLIGetParameter
                        (
                          /* The command string itself. */
                          pcCommandString,
                          /* Return the first parameter. */
                          arg_number,
                          /* Store the parameter string length. */
                          &xParameterStringLength
                        );
    return strtol(pcParameter, NULL, 0);
}