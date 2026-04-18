/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
extern "C"{
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "fdcan.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
}

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#include <lely/util/diag.h>
#define trace(...) \
	diag_at(DIAG_DEBUG, 0, &(struct floc){ __FILE__, __LINE__, 0 }, \
			__VA_ARGS__)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
//void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
// dn : master to slave
// up : slave to 
// These functions are called by cli:
// TODO: Implement write to Mode Register

// Variables for to monitor with STM32CubeMonitor.
// Need to be global!
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/*
Init + Loop:
[build] Memory region         Used Size  Region Size  %age Used
[build]              RAM:        5464 B       128 KB      4.17%
[build]            FLASH:      269944 B       512 KB     51.49%

No user code:

[build]              RAM:        4176 B       128 KB      3.19%
[build]            FLASH:         37 KB       512 KB      7.23%

Init only:

[build] Memory region         Used Size  Region Size  %age Used
[build]              RAM:        5440 B       128 KB      4.15%
[build]            FLASH:      265656 B       512 KB     50.67%

Loop Only:

[build] Memory region         Used Size  Region Size  %age Used
[build]              RAM:        4360 B       128 KB      3.33%
[build]            FLASH:       54512 B       512 KB     10.40%

//Init: Only create_from_sdev: 

[build]              RAM:        3384 B       128 KB      2.58%
[build]            FLASH:      133768 B       512 KB     25.51%

*/
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
//hello_easyspin();
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FDCAN1_Init();
  MX_TIM17_Init();
  MX_USART2_UART_Init();
  MX_RTC_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, (uint8_t *)&huart2.Instance->RDR, 1);
  /*
  can_init(125);
  trace("SW-Version %s (Commit %s, build on %s)",PROJECT_VERSION,APP_GIT_HASH,BUILD_TIME);
  net = can_net_create();
  assert(net);
  can_net_set_send_func(net, &on_can_send, NULL);
  clock_gettime(1, &now);
  can_net_set_time(net, &now);

  dev = co_dev_create_from_sdev(&lpc17xx_sdev);
  assert(dev);
mod
  nmt = co_nmt_create(net, dev);
  assert(nmt);
  co_nmt_cs_ind(nmt, CO_NMT_CS_RESET_NODE);
  co_nmt_set_cs_ind(nmt, &on_nmt_cs, NULL);
  co_time_set_ind(co_nmt_get_time(nmt), &on_time, NULL);

  // Set the download (SDO write) indication function for sub-object 2000:01.
  co_sub_set_dn_ind(co_dev_find_sub(dev, 0x2000, 0x00), &on_dn_2000_00, NULL);
  // Set the upload (SDO read) indication function for sub-object 2001:01.
  co_sub_set_up_ind(co_dev_find_sub(dev, 0x2001, 0x00), &on_up_2001_00, NULL);
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
  trace("Press PB1 to continue to start CanOpen loop");
  trace("Sending dummy data [7F 12 34");
  int cnt=0;
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);

  while (!HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin)) {
    struct can_msg msg;
    msg.len = 2;
    msg.data[1] = 0x12;
    msg.data[2] = 0x34;
    HAL_Delay(1);
    if(cnt>1000){
      cnt=0;  
      can_send(&msg,3);
    }
    d = gamma_corrected_dutycycle(1000,cnt);
    cnt++;
    // Disable Interrupt an poll can rx buffer
  }
  trace("Running CANOpen Application");
  */
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();  /* Call init function for freertos objects (in cmsis_os2.c) */
  MX_FREERTOS_Init();

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
/*
    clock_gettime(1, &now);
    localtime(&now.tv_sec);

    RTC_DateTypeDef gDate;
    RTC_TimeTypeDef gTime;
    // gTime.Hours gTime.Seconds
    HAL_RTC_GetTime(&hrtc, &gTime, RTC_FORMAT_BIN);
    HAL_RTC_GetDate(&hrtc, &gDate, RTC_FORMAT_BIN);

    // trace("%02d:%02d:%02d",gTime.Hours, gTime.Minutes, gTime.Seconds);

    if ((last_tp.tv_sec == now.tv_sec) && (last_tp.tv_nsec == now.tv_nsec)) {
      // trace("clock did not progressed %lu:%lu",now.tv_sec,now.tv_nsec);
    }
    can_net_set_time(net, &now);

    last_tp.tv_nsec = now.tv_nsec;
    last_tp.tv_sec = now.tv_sec;
    // Process any received CAN frames.
    struct can_msg msg;
    int n_frames = can_recv(&msg, 1);
    if (n_frames != 0) {
      can_net_recv(net, &msg);
      //HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }
    set_statusword(dev);
    struct trapezoidal_ramp params;
    params.a_max = 100; // [mm / s*E-2]
    params.v_max= 300; // [mm / s]
    // TODO: Should be never unititialized. 
                                    // Either read from cli or FRAM!
    if (get_state() == drive_state_operation_enabled)
    {
      t++;
    }
    else{
      t=0;
    }
    enum mode modes_of_operation = get_mode(dev);
    rpm = run_motion_engine(modes_of_operation,t,params);
    if (rpm > 0){
             LOG(CLI_LOG_CAT1, "i=%lu, f=%lu",t,rpm);

        }
    CLI_RUN();
*/
    HAL_Delay(1);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 21;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM16 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM16)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
