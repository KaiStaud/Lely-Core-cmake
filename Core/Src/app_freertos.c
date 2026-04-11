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
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <errno.h>  // for errno
#include <lely/can/net.h>
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
#include <stdlib.h>  // for strtol
#include <trapezoidal_ramp.h>

#include "../bsp/can.h"
#include "rtc.h"
#include "tim.h"
#include "version.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

static int on_can_send(const struct can_msg* msg, void* data);
static void on_nmt_cs(co_nmt_t* nmt, co_unsigned8_t cs, void* data);
static void on_time(co_time_t* time, const struct timespec* tp, void* data);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#include <lely/util/diag.h>
#define trace(...) \
  diag_at(DIAG_DEBUG, 0, &(struct floc){__FILE__, __LINE__, 0}, __VA_ARGS__)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
    .name = "defaultTask",
    .priority = (osPriority_t)osPriorityNormal,
    .stack_size = 128 * 4};
/* Definitions for disableTask */
osThreadId_t disableTaskHandle;
const osThreadAttr_t disableTask_attributes = {
    .name = "disableTask",
    .priority = (osPriority_t)osPriorityLow,
    .stack_size = 128 * 4};
/* Definitions for enableTask */
osThreadId_t enableTaskHandle;
const osThreadAttr_t enableTask_attributes = {
    .name = "enableTask",
    .priority = (osPriority_t)osPriorityLow,
    .stack_size = 128 * 4};
/* Definitions for canopenTask */
osThreadId_t canopenTaskHandle;
const osThreadAttr_t canopenTask_attributes = {
    .name = "canopenTask",
    .priority = (osPriority_t)osPriorityLow,
    .stack_size = 128 * 8};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void* argument);
void DisableTask(void* argument);
void EnableTask(void* argument);
void CANOpenTask(void* argument);

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
  defaultTaskHandle =
      osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of disableTask */
  disableTaskHandle = osThreadNew(DisableTask, NULL, &disableTask_attributes);

  /* creation of enableTask */
  enableTaskHandle = osThreadNew(EnableTask, NULL, &enableTask_attributes);

  /* creation of canopenTask */
  canopenTaskHandle = osThreadNew(CANOpenTask, NULL, &canopenTask_attributes);

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
void StartDefaultTask(void* argument) {
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
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_DisableTask */
/**
 * @brief Function implementing the disableTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_DisableTask */
void DisableTask(void* argument) {
  /* USER CODE BEGIN DisableTask */
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END DisableTask */
}

/* USER CODE BEGIN Header_EnableTask */
/**
 * @brief Function implementing the enableTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_EnableTask */
void EnableTask(void* argument) {
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
/* USER CODE END Header_CANOpenTask */
void CANOpenTask(void* argument) {
  /* USER CODE BEGIN CANOpenTask */

  co_nmt_t* nmt;
  can_net_t* net;
  // Generated by `dcf2c --no-strings lpc17xx.dcf lpc17xx_sdev -o src/sdev.c`
  extern const struct co_sdev lpc17xx_sdev;
  co_dev_t* dev;
  struct timespec now = {0, 0};

  can_init(125);
  trace("SW-Version %s (Commit %s, build on %s)", PROJECT_VERSION, APP_GIT_HASH,
        BUILD_TIME);
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
    trace("tdpo 1 not created");
  }
  co_rpdo_t* rpdo_1 = co_rpdo_create(net, dev, 1);
  if (rpdo_1 == NULL) {
    trace("rdpo 1 not created");
  }

  if (co_tpdo_start(tpdo_1) != 0) {
    trace("could not start tpdo");
  }
  if (co_rpdo_start(rpdo_1) != 0) {
    trace("could not start rpdo");
  }

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
      // HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }

    osDelay(1);
  }
  /* USER CODE END CANOpenTask */
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

/* USER CODE END Application */
