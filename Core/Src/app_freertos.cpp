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
#include "lely/co/obj.h"
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
#include <limits.h>  // for INT_MAX, INT_MIN
#include <math.h>
#include <stdint.h>

#include "../bsp/can.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "spi.h"
#include "version.h"
#include "extern_variables.h"
#include "app_cli.h"
#include "lcd.h"
}

#include "Statemachine.hpp"
#include "../Config/Config.hpp"
#include "../Config/FRAMBackend.hpp"
#include "../Config/Kinematics.hpp"
#include "MotionManager.hpp"
#include "screens.hpp"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

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
uint32_t ctrl_word = 0;
uint32_t statusword = 0;
uint8_t requested_mode = 0;
uint32_t target = 0;
uint32_t response = 0;
volatile bool canopen_initialized = false;

cia402::statemachine::DriveState drive_state = cia402::statemachine::DriveState::kNotReadyToSwitchOn;

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 8,
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
  .stack_size = 256 * 8,
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

auto disable_drive() -> bool;
auto enable_drive() -> bool;
auto read_object(uint16_t index,uint8_t subindex) -> uint32_t;
auto read_gpo() -> bool;
auto get_fault_inputs(motion_manager::Polarity polarity) -> cia402::statemachine::FaultInputs;
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
  //int cnt = 0;
  LCD_init();
  Display::Screens::StatusScreen statusScreen("CiA402 Status", "Operation Enabled");
  statusScreen.draw();
  while(!canopen_initialized){
    osDelay(1);
  }
  /* Infinite loop */
  for (;;) {
    
    Display::Screens::StatusData data{
        .controlWord = ctrl_word,
        .statusWord = statusword,
        .mode = read_object(0x6060, 0),
        .rpm = static_cast<uint32_t>(rpm),
        .target = target,
        .response = response
    };
    
    statusScreen.updateValues(data);
    statusScreen.updateStatus(drive_state);
    statusScreen.draw();
    
/*
    if (cnt > 1000) {
      cnt = 0;
    }
    gamma_corrected_dutycycle(1000, cnt);
    cnt++;
*/
    osDelay(1000);
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
  config::ObjectDictionary<config::backends::FRAMBackend> objectDictionary(&hspi1, FRAM_CS_GPIO_Port, FRAM_CS_Pin);
  objectDictionary.Restore();
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
  cia402::statemachine::SlaveStatemachine statemachine;
  motion_manager::MotionManager manager(
    read_object,
    read_gpo,
    disable_drive,
    enable_drive);

  motion_profile::TargetConstraints target_constraints{
      .acceleration= 100,//0.002,//1000,
      .deceleration= 0.002,
      .end_velocity = 6.0,
      .start_velocity=  0
    };
    manager.Initialize(target_constraints);

  int t = 0;
  motion_manager::Polarity  polarity = motion_manager::Polarity::kPositive;
  motion_manager::MotionModes mode = motion_manager::MotionModes::kNone;
  volatile bool timer_started = false;
  while (!canopen_initialized){};
  enable_drive();
//  HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);

  /* Infinite loop */
  for (;;) {

  co_obj_t* obj = co_dev_find_obj(dev, 0x6040);
  ctrl_word = co_sub_get_val_u32(co_dev_find_sub(dev, 0x6040, 0));

  drive_state= statemachine.HandleControlWord(ctrl_word,get_fault_inputs(polarity));
  statusword = statemachine.GetStatuswordLowbyte(drive_state);
  obj = co_dev_find_obj(dev, 0x6041);
  co_obj_set_val_u32(obj, 0x00, statusword);
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
    uint8_t current_requested_mode = static_cast<uint8_t>(read_object(0x6060, 0));

    bool mode_changed = false;
    if (current_requested_mode != requested_mode)
    {
      requested_mode = current_requested_mode;
      mode = manager.RequestMode(requested_mode);
      t = 0;
      mode_changed = true;
    }

    uint32_t current_target = target;
    bool target_valid = false;
    if((mode == motion_manager::MotionModes::kCylicPositionMode) || (mode == motion_manager::MotionModes::kProfilePositionMode))
    {
      current_target = read_object(0x607A, 0);
      target_valid = true;
    }
    if((mode == motion_manager::MotionModes::kCylicVelocityMode) || (mode == motion_manager::MotionModes::kProfileVelocityMode))
    {
      current_target = read_object(0x60FF, 0);
      target_valid = true;
    }
    if(target_valid && (mode_changed || (current_target != target)))
    {
      target = current_target;
      manager.SetTarget(target);
      t = 0;
    }
    if(drive_state == cia402::statemachine::DriveState::kSwitchedOn)
    {
      enable_drive();
    }
    if((mode != motion_manager::MotionModes::kNone)&&(drive_state==cia402::statemachine::DriveState::kOperationEnabled))
    {
      if(!timer_started){
        HAL_TIM_PWM_Start(&htim8, TIM_CHANNEL_2);
        timer_started = true;
      }
      rpm = manager.GetFrequency(t);
      //TIM2->CCR1 = (uint32_t)rpm;

      t++;
    }
    // Hardware-Controller:
    //call manager.QuickStop() when a break condition occurrs.
    // motion manager will automatically call disable_drive() and sets pwm frequency to zero.
    if(drive_state == cia402::statemachine::DriveState::kFaultReactionActive)
    {
      manager.QuickStop();   
      timer_started = false;
      t=0;
      // Drive should be now in Fault. Clear Fault Condition by writing the statusword.

    }
    manager.Update();
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
/* Hardware IO-Functions */
auto get_fault_inputs(motion_manager::Polarity polarity) -> cia402::statemachine::FaultInputs
{
  auto fault_gpio = read_gpo();
  cia402::statemachine::FaultInputs limit_switch=cia402::statemachine::FaultInputs::kNoFaultDetected;
// We currently only support on gpio: Min Limit Endswitch.
// Polarity | Endswitch triggered | Fault Condition
// -------------------------------------------------  
//   Positive |         Min        | No Fault Detected
//   Positive |         Max        | Right Switch Active
//   Negative |         Min        | Right Switch Active
//   Negative |         Max        | No Fault Detected
  if (fault_gpio)
  {
    limit_switch=cia402::statemachine::FaultInputs::kRightSwitchActive;
  }

  if((limit_switch==cia402::statemachine::FaultInputs::kRightSwitchActive)&&(polarity == motion_manager::Polarity::kPositive))
  {
    return cia402::statemachine::FaultInputs::kRightSwitchActive;
  }
  if((limit_switch==cia402::statemachine::FaultInputs::kLeftSwitchActive)&&(polarity == motion_manager::Polarity::kNegative))
  {
    return cia402::statemachine::FaultInputs::kLeftSwitchActive;
  }
  return cia402::statemachine::FaultInputs::kNoFaultDetected;
}

auto read_gpo()-> bool{
  GPIO_PinState input_ = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);
  // RESET=0
  // SET=1
  return static_cast<bool>(input_);
}

auto read_object(uint16_t index,uint8_t subindex) -> uint32_t
{
  co_obj_t* obj = co_dev_find_obj(dev, index);
  return co_obj_get_val_u32(obj,subindex);
}

auto enable_drive() -> bool{
  HAL_GPIO_WritePin(RST_GPIO_Port, RST_Pin, GPIO_PIN_SET);
  osDelay(3000);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
  osDelay(2);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
  uint8_t pData[] = {0, 0xD0, 0, 0, 0x21, 0x00, 0x00, 0x00, 0xb8};
  HAL_SPI_Transmit(&hspi1, pData, sizeof(pData),10);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
  return true;
}
auto disable_drive() -> bool{
  HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_2);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_RESET);
  uint8_t pDisableCmd[] = {0, 0xA8};
  HAL_SPI_Transmit(&hspi1, pDisableCmd,sizeof(pDisableCmd), 10);
  HAL_GPIO_WritePin(CS_GPIO_Port, CS_Pin, GPIO_PIN_SET);
  return true;
}
/* USER CODE END Application */
