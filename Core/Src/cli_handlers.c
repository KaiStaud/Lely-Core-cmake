#include "cli_handlers.h"
#include <stdlib.h>  // for strtol
#include <lely/co/co.h>
#include <lely/can/net.h>
#include "FreeRTOS_CLI.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "extern_variables.h"
#include "string.h"
#include <stdio.h>

#define MAX_IN_STR_LEN                          300
#define MAX_OUT_STR_LEN                         600
#define MAX_RX_QUEUE_LEN                        300

static const char *prvpcTaskListHeader = "Task states: Bl = Blocked, Re = Ready, Ru = Running, De = Deleted,  Su = Suspended\r\n"\
                                         "Task name         State  Priority  Stack remaining  CPU usage  Runtime(us)\r\n"\
                                         "================= =====  ========  ===============  =========  ===========\r\n";

BaseType_t WriteObject(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
  
  int index = arg_to_int(pcCommandString,1);
  int subindex = arg_to_int(pcCommandString,2);
  int value = arg_to_int(pcCommandString,3);
                                //if (argc != 4) {
    //LOG(CLI_LOG_CAT1, "write_object [Index] [SubIndex] [value]");
  //}
  co_obj_t* obj = co_dev_find_obj(dev, index);
  co_obj_set_val(obj, subindex, &value, sizeof(value));
  return pdFALSE;
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
    return pdFALSE;
  } else {
    //LOG(CLI_LOG_CAT1, "Failed to write to OD: Error %s", errno2str(get_errc()));
    return pdFALSE;
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
    return pdFALSE;
  } else {
    //LOG(CLI_LOG_CAT1, "Failed to write to OD: Error %s", errno2str(get_errc()));
    return pdFALSE;
  }
    return pdFALSE;
}

BaseType_t GetIO(char *pcWriteBuffer, size_t xWriteBufferLen,
                                 const char *pcCommandString) {
    return pdFALSE;
}

static const char *prvpcMapTaskState(eTaskState eState)
{
    switch (eState)
    {
        case     eReady: return "Re";
        case   eRunning: return "Ru";
        case   eDeleted: return "De";
        case   eBlocked: return "Bl";
        case eSuspended: return "S";
        default: return "??";
    }
}


BaseType_t prvCommandTaskStats(char *pcWriteBuffer,
                                      size_t xWriteBufferLen,
                                      const char *pcCommandString)
{
    UBaseType_t uxArraySize, i;
    uint32_t ulTotalRunTime = 0;
    TaskStatus_t *pxTaskStatusArray;
    int offset = 0;


    size_t xHeapFree;
    size_t xHeapMinMemExisted;

    xHeapFree = xPortGetFreeHeapSize();
    xHeapMinMemExisted = xPortGetMinimumEverFreeHeapSize();
    snprintf(pcWriteBuffer, xWriteBufferLen,
             "Heap size: %3u bytes (%3d KiB)\r\nRemaining: %3u bytes (%3d KiB)\r\nMinimum ever existed : %3u bytes (%3d KiB)\r\n",
             configTOTAL_HEAP_SIZE, configTOTAL_HEAP_SIZE / 1024, xHeapFree, xHeapFree / 1024, xHeapMinMemExisted, xHeapMinMemExisted / 1024);


    /* Anzahl Tasks holen */
    uxArraySize = uxTaskGetNumberOfTasks();

    /* Speicher allozieren */
    pxTaskStatusArray = pvPortMalloc(uxArraySize * sizeof(TaskStatus_t));
    if (pxTaskStatusArray == NULL)
    {
        snprintf(pcWriteBuffer, xWriteBufferLen,
                 "Error: Not enough memory\r\n");
        return pdFALSE;
    }

    /* Task-Infos holen */
    uxArraySize = uxTaskGetSystemState(pxTaskStatusArray,
                                       uxArraySize,
                                       &ulTotalRunTime);

    /* Prozentbasis vorbereiten */
    if (ulTotalRunTime > 0)
    {
        ulTotalRunTime /= 100UL;
    }

    /* Header */
    offset += snprintf(pcWriteBuffer + offset,
                       xWriteBufferLen - offset,
                       "%s",
                       prvpcTaskListHeader);

    /* Alle Tasks durchlaufen */
    for (i = 0; i < uxArraySize; i++)
    {
        TaskStatus_t *pxTask = &pxTaskStatusArray[i];
        uint32_t ulPercentage = 0;

        if (ulTotalRunTime > 0)
        {
            ulPercentage = pxTask->ulRunTimeCounter / ulTotalRunTime;
        }

        offset += snprintf(pcWriteBuffer + offset,
                           xWriteBufferLen - offset,
                           "%-16s  %5s  %8lu  %14uB  %6s  %11lu\r\n",
                           pxTask->pcTaskName,
                           prvpcMapTaskState(pxTask->eCurrentState),
                           (unsigned long)pxTask->uxCurrentPriority,
                           (unsigned int)pxTask->usStackHighWaterMark,
                           (ulPercentage > 0) ? " " : "<1%",
                           (unsigned long)pxTask->ulRunTimeCounter);

        /* Buffer-Schutz */
        if (offset >= xWriteBufferLen)
        {
            break;
        }
    }
    /* Speicher freigeben */
    vPortFree(pxTaskStatusArray);

    return pdFALSE; /* nur ein Aufruf */
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