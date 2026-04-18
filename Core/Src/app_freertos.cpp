/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * File Name          : app_freertos.c
 * Description        : Code for freertos applications
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include <lely/can/net.hpp>

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <errno.h>  // for errno
//#include <lely/can/net.hpp>
#include <lely/co/co.h>
#include <lely/co/nmt.h>
#include <lely/co/rpdo.h>
#include <lely/co/sdev.h>
#include <lely/co/sdo.h>
#include <lely/co/time.h>
#include <lely/co/tpdo.h>
#include <libcia402/digital_inputs.h>
#include <libcia402/homing.h>
#include <libcia402/statemachine.h>
#include <limits.h>  // for INT_MAX, INT_MIN
#include <math.h>
#include <stdint.h>

#include "../bsp/can.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "version.h"
//#include <lely/util/diag.h>
#include "extern_variables.h"
#include "app_cli.h"
}
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

enum homing_progress {
	homing_disabled = 0,
	opmode_configured = 1,
	homing_profile_configured = 2,
	homing_started = 3,
	homing_active = 4,
	homing_done = 5
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define trace(...) NOP
//  diag_at(DIAG_DEBUG, 0, &(struct floc){__FILE__, __LINE__, 0}, __VA_ARGS__)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
co_dev_t* dev;
double rpm = 0, t_a = 0, t_c = 0;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal
};
/* Definitions for enableTask */
osThreadId_t enableTaskHandle;
const osThreadAttr_t enableTask_attributes = {
  .name = "enableTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow
};
/* Definitions for canopenTask */
osThreadId_t canopenTaskHandle;
const osThreadAttr_t canopenTask_attributes = {
  .name = "canopenTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow
};
/* Definitions for cia402Task */
osThreadId_t cia402TaskHandle;
const osThreadAttr_t cia402Task_attributes = {
  .name = "cia402Task",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow
};
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
static void on_time(co_time_t* time, const struct timespec* tp, void* data);
static void on_nmt_cs(co_nmt_t* nmt, co_unsigned8_t cs, void* data);
static int on_can_send(const struct can_msg* msg, void* data);
double gamma_corrected_dutycycle(uint32_t f_max, uint32_t f);
//double run_motion_engine(mode selected_mode, int t,struct trapezoidal_ramp params);
void set_statusword(co_dev_t* dev);
//enum homing_progress try_homing(co_dev_t* dev);
uint32_t co_hal_read_digital_inputs();
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void EnableTask(void *argument);
void CANOpenTask(void *argument);
void Cia402Task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of enableTask */
  //enableTaskHandle = osThreadNew(EnableTask, NULL, &enableTask_attributes);

  /* creation of canopenTask */
  canopenTaskHandle = osThreadNew(CANOpenTask, NULL, &canopenTask_attributes);

  /* creation of cia402Task */
  cia402TaskHandle = osThreadNew(Cia402Task, NULL, &cia402Task_attributes);
  freeRTOSCliInit();
  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
 * @brief  Function implementing the defaultTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
  int cnt = 0;
  /* Infinite loop */
  for (;;) {
    if (cnt > 1000) {
      cnt = 0;
    }
    gamma_corrected_dutycycle(1000, cnt);
    cnt++;
    // Disable Interrupt an poll can rx buffer
    //CLI_RUN();
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_EnableTask */
/**
 * @brief Function implementing the enableTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_EnableTask */
void EnableTask(void *argument)
{
  /* USER CODE BEGIN EnableTask */
  
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END EnableTask */
}

/* USER CODE BEGIN Header_CANOpenTask */
/**
 * @brief Function implementing the canopenTask thread.
 * @param argument: Not used
 * @retval None
 */
volatile bool canopen_initialized = false;
/* USER CODE END Header_CANOpenTask */
void CANOpenTask(void *argument)
{
  /* USER CODE BEGIN CANOpenTask */

  co_nmt_t* nmt;
  can_net_t* net;
  // Generated by `dcf2c --no-strings lpc17xx.dcf lpc17xx_sdev -o src/sdev.c`
  extern const struct co_sdev lpc17xx_sdev;
  struct timespec now = {0, 0};

  can_init(125);
  //trace("SW-Version %s (Commit %s, build on %s)", PROJECT_VERSION, APP_GIT_HASH,BUILD_TIME);
  net = can_net_create();
  assert(net);
  can_net_set_send_func(net, &on_can_send, NULL);
  clock_gettime(1, &now);
  can_net_set_time(net, &now);

  dev = co_dev_create_from_sdev(&lpc17xx_sdev);
  assert(dev);

  nmt = co_nmt_create(net, dev);
  assert(nmt);
  co_nmt_cs_ind(nmt, CO_NMT_CS_RESET_NODE);
  co_nmt_set_cs_ind(nmt, &on_nmt_cs, NULL);
  co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);
  co_tpdo_t* tpdo_1 = co_tpdo_create(net, dev, 1);
  if (tpdo_1 == NULL) {
    //trace("tdpo 1 not created");
  }
  co_rpdo_t* rpdo_1 = co_rpdo_create(net, dev, 1);
  if (rpdo_1 == NULL) {
    //trace("rdpo 1 not created");
  }

  if (co_tpdo_start(tpdo_1) != 0) {
    //trace("could not start tpdo");
  }
  if (co_rpdo_start(rpdo_1) != 0) {
    //trace("could not start rpdo");
  }

  /*
  CLI_INIT(&huart2);	
  CLI_ADD_CMD("move_to", "Rotate n steps", set_target_position);
  CLI_ADD_CMD("set_rpm", "Rotate with constant velocity", set_rpm);
  CLI_ADD_CMD("write_object","Write CANOpen object",write_object);
  */
canopen_initialized = true;
  /* Infinite loop */
  for (;;) {
    clock_gettime(1, &now);
    localtime(&now.tv_sec);

    RTC_DateTypeDef gDate;
    RTC_TimeTypeDef gTime;
    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);
    can_net_set_time(net, &now);
    struct can_msg msg;
    int n_frames = can_recv(&msg, 1);
    if (n_frames != 0) {
      can_net_recv(net, &msg);
    }

    //CLI_RUN();
    osDelay(1);
  }
  /* USER CODE END CANOpenTask */
}

/* USER CODE BEGIN Header_Cia402Task */
/**
 * @brief Function implementing the cia402Task thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_Cia402Task */
void Cia402Task(void *argument)
{
  /* USER CODE BEGIN Cia402Task */
  int t = 0;

  while (!canopen_initialized){};
  /* Infinite loop */
  for (;;) {
    set_statusword(dev);
    /*
    1 revolution (200 Steps) per second
    lead: 150 mm in 10 seconds
    */
    /*
    params.a_max = 0.002;  
    params.v_max = 6.0;  
    t_a = params.t_acc;
    t_c = params.t_const;
    */
    // TODO: Should be never unititialized.
    // Either read from cli or FRAM!
    if (get_state() == drive_state_operation_enabled) {
      t++;
    } else {
      t = 0;
    }
//    mode modes_of_operation = get_mode(dev);
//    rpm = run_motion_engine(modes_of_operation, t, params);
    //gamma_corrected_dutycycle(params.v_max, rpm);

    osDelay(1);
  }
  /* USER CODE END Cia402Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
static int on_can_send(const struct can_msg* msg, void* data) {
  (void)data;
  struct timespec tp;
  clock_gettime(1, &tp);
  return can_send(msg, 1) == 1 ? 0 : -1;
}

static void on_nmt_cs(co_nmt_t* nmt, co_unsigned8_t cs, void* data) {
  (void)data;

  switch (cs) {
    case CO_NMT_CS_START:
      // Reset the TIME indication function, since the service may
      // have been restarted.
      co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);
      break;
    case CO_NMT_CS_STOP:
      break;
    case CO_NMT_CS_ENTER_PREOP:
      co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);
      break;
    case CO_NMT_CS_RESET_NODE:
      // Initiate a system reset.
      exit(0);
      break;
    case CO_NMT_CS_RESET_COMM:
      break;
  }
}

static void on_time(co_time_t* time, const struct timespec* tp, void* data) {
  (void)time;
  (void)data;

  // Update the wall clock, _not_ the monotonic clock used by the CAN
  // network.
  clock_settime(CLOCK_REALTIME, tp);
}

double gamma_corrected_dutycycle(uint32_t f_max, uint32_t f) {
  double x = (double)f / (double)f_max;
  double dutycycle = pow(x, 2.0) * 500;
  // dutycycle = dutycycle  * 500;
  TIM2->CCR1 = (uint32_t)dutycycle;
  return dutycycle;
}

uint32_t co_hal_read_digital_inputs() {
  digital_inputs io;
  io.positive_limit_switch = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
  uint32_t entry_60FD_00 = read_inputs(io);
  return entry_60FD_00;
}
/*
enum homing_progress try_homing(co_dev_t* dev) {
  static enum homing_progress progress = homing_disabled;
  switch (progress) {
    case homing_disabled:
      if (co_sub_get_val_u32(co_dev_find_sub(dev, 0x6060, 0)) == 0x6)
       {
          progress = opmode_configured;
       }
      break;
    case opmode_configured:
      uint32_t homing_profile =
          co_sub_get_val_u32(co_dev_find_sub(dev, 0x6098, 0));
      if (homing_profile == 0x1) {
        progress = homing_profile_configured;
      }
      break;
    case homing_profile_configured:
      mode = co_sub_get_val_u32(co_dev_find_sub(dev, 0x6060, 0));
      if (mode == 0xF6) {
        progress = homing_started;
      }
      break;
    case homing_started:
      //trace("Homing:Start Homing");
      //trace("Homing:Acceleration=%lu, Velocity=%lu",
            co_sub_get_val_u32(co_dev_find_sub(dev, 0x609A, 0)),
            co_sub_get_val_u32(co_dev_find_sub(dev, 0x6099, 0)));
      HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
      HAL_Delay(3000);
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
      HAL_Delay(2);
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
      uint8_t pData[] = {0, 0xD0, 0, 0, 0x21, 0x00, 0x00, 0x00, 0xb8};
      //			  HAL_SPI_Transmit(&hspi1, pData, sizeof(pData),
      // 10);
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
      HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
      progress = homing_active;
      break;
    case homing_active:
      if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
        progress = homing_done;
        //trace("Homing: Negative Endswitch triggered. Disabling drive");
      }
      break;
    case homing_done:
      HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_2);
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
      uint8_t pDisableCmd[] = {0, 0xA8};
      //			  HAL_SPI_Transmit(&hspi1, pDisableCmd,
      // sizeof(pDisableCmd), 10);
      HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
      break;
    
    default:
      break;
  }
  return progress;
}
*/
void set_statusword(co_dev_t* dev) {
  co_unsigned32_t val = co_hal_read_digital_inputs();
  //		co_sub_set_val_u32(co_dev_find_sub(dev, 0x60FD, 0),&val);
  // Controlword
  co_obj_t* obj = co_dev_find_obj(dev, 0x6040);
  uint32_t ctrl_word = co_sub_get_val_u32(co_dev_find_sub(dev, 0x6040, 0));
  // Statusword
  run_transition(ctrl_word);
  uint16_t statusword = get_statusword_lowbyte(get_state());
  obj = co_dev_find_obj(dev, 0x6041);
  co_obj_set_val(obj, 0x00, &statusword, sizeof(val));
}

/*
double run_motion_engine(enum mode selected_mode, int t,
                           struct trapezoidal_ramp params) {
  if ((selected_mode == profile_position_mode) ||
      (selected_mode == cyclic_position_mode)) {
    // TODO: Return Error "struct params uninitialized"
    const co_obj_t* obj = co_dev_find_obj(dev, 0x607A);
    move_to(&params, co_obj_get_val_u32(obj, 0));
    double rpm = ramp_update(&params, t);
    return rpm;
  } else if ((selected_mode == profile_velocity_mode) ||
             (selected_mode == cyclic_velocity_mode)) {
    const co_obj_t* obj = co_dev_find_obj(dev, 0x60FF);
    return co_obj_get_val_u32(obj, 0);
  } else if (selected_mode == homing) {
    try_homing(dev);
  } else if (selected_mode == no_mode_selected) {
  } else {
    //LOG(CLI_LOG_CAT1, "Mode %i is not implemented", selected_mode);
  }
  return 0;
}
*/
/* USER CODE END Application */