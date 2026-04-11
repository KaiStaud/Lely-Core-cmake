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
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
    .stack_size = 128 * 4};

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
  /* Infinite loop */
  for (;;) {
    osDelay(1);
  }
  /* USER CODE END CANOpenTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
