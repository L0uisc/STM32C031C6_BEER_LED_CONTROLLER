/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    app_threadx.c
  * @author  MCD Application Team
  * @brief   ThreadX applicative file
  ******************************************************************************
    * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "app_threadx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <stdint.h>

#include "main.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  RGB_LED_OFF = 0,
  RGB_LED_RED = (1 << 0),
  RGB_LED_GREEN = (1 << 1),
  RGB_LED_BLUE = (1 << 2),
  RGB_LED_YELLOW = RGB_LED_RED | RGB_LED_GREEN,
  RGB_LED_CYAN = RGB_LED_GREEN | RGB_LED_BLUE,
  RGB_LED_MAGENTA = RGB_LED_BLUE | RGB_LED_RED,
  RGB_LED_WHITE = RGB_LED_RED | RGB_LED_GREEN | RGB_LED_BLUE
} led_colour_t;

typedef struct
{
  uint32_t duration;
  led_colour_t colour;
} led_phase_t;

typedef struct
{
  int group_id;
  uint32_t next_wakeup;
  uint32_t next_phase;
  uint32_t phase_count;
  led_phase_t *phase_sequence;
} led_cycle_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_LED_GROUPS 5
#define LED_THREAD_PRIORITY (TX_MAX_PRIORITIES << 1)
#define TICKS_PER_TIME_UNIT 25
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SEQUENCE_LENGTH(s) (sizeof(s) / sizeof(led_phase_t))
#define CYCLE_DEFINITION(s) { .next_phase = 0, .phase_count = SEQUENCE_LENGTH(s), .phase_sequence = s }
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_SEMAPHORE tx_app_semaphore;
/* USER CODE BEGIN PV */
TX_THREAD led_group_threads[NUM_LED_GROUPS];

led_phase_t group_1_sequence[] = {
		{ .duration = 1, .colour = RGB_LED_YELLOW }
};
led_phase_t group_2_sequence[] = {
		{ .duration = 1, .colour = RGB_LED_WHITE }
};
led_phase_t group_3_sequence[] = {
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 24, .colour = RGB_LED_OFF }
};
led_phase_t group_4_sequence[] = {
		{ .duration = 4, .colour = RGB_LED_OFF },
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 20, .colour = RGB_LED_OFF }
};
led_phase_t group_5_sequence[] = {
		{ .duration = 8, .colour = RGB_LED_OFF },
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 16, .colour = RGB_LED_OFF }
};

led_cycle_t led_cycle[NUM_LED_GROUPS] = {
		CYCLE_DEFINITION(group_1_sequence),
		CYCLE_DEFINITION(group_2_sequence),
		CYCLE_DEFINITION(group_1_sequence),
		CYCLE_DEFINITION(group_2_sequence),
		CYCLE_DEFINITION(group_3_sequence)
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
VOID App_Led_Group_Function(ULONG data);
led_phase_t *App_Led_Group_Sequence_Select(int index);
GPIO_TypeDef *App_Led_Group_Get_GPIO_Port(int index);
uint16_t App_Led_Group_Get_Pin_Mask(int index, led_colour_t colour, GPIO_PinState set_reset);
uint16_t App_Led_Group_Get_Red_Pin_Mask(int index);
uint16_t App_Led_Group_Get_Green_Pin_Mask(int index);
uint16_t App_Led_Group_Get_Blue_Pin_Mask(int index);
/* USER CODE END PFP */

/**
  * @brief  Application ThreadX Initialization.
  * @param memory_ptr: memory pointer
  * @retval int
  */
UINT App_ThreadX_Init(VOID *memory_ptr)
{
  UINT ret = TX_SUCCESS;

  /* USER CODE BEGIN App_ThreadX_MEM_POOL */
  TX_BYTE_POOL *byte_pool = (TX_BYTE_POOL*)memory_ptr;
  /* USER CODE END App_ThreadX_MEM_POOL */

  /* Create tx app semaphore.  */
  if (tx_semaphore_create(&tx_app_semaphore, "tx app semaphore", 0) != TX_SUCCESS)
  {
    return TX_SEMAPHORE_ERROR;
  }

  /* USER CODE BEGIN App_ThreadX_Init */
  for (int i = 0; i < NUM_LED_GROUPS; i++)
  {
    void *buffer;
    if ((ret = tx_byte_allocate(byte_pool, &buffer, TX_MINIMUM_STACK, TX_NO_WAIT)) != TX_SUCCESS)
    {
      goto err;
    }

    char name[9];
    sprintf(name, "Group %d", i);
    if ((ret = tx_thread_create(&led_group_threads[i],
    		name,
			App_Led_Group_Function,
			0,
			buffer,
			TX_MINIMUM_STACK,
			LED_THREAD_PRIORITY,
			LED_THREAD_PRIORITY,
			TX_NO_TIME_SLICE,
			TX_AUTO_START)) != TX_SUCCESS)
    {
      goto err;
    }
  }

err:
  /* USER CODE END App_ThreadX_Init */

  return ret;
}

  /**
  * @brief  Function that implements the kernel's initialization.
  * @param  None
  * @retval None
  */
void MX_ThreadX_Init(void)
{
  /* USER CODE BEGIN Before_Kernel_Start */
  puts("Starting kernel...\r\n");
  /* USER CODE END Before_Kernel_Start */

  tx_kernel_enter();

  /* USER CODE BEGIN Kernel_Start_Error */
  puts("Kernel startup failed\r\n");
  /* USER CODE END Kernel_Start_Error */
}

/* USER CODE BEGIN 1 */
VOID App_Led_Group_Function(ULONG data)
{
  for (;;)
  {
    led_cycle_t *cycle = &led_cycle[data];
    int phase_idx = cycle->next_phase;
    led_phase_t *next_phase = &cycle->phase_sequence[phase_idx];
    cycle->next_wakeup += next_phase->duration * TICKS_PER_TIME_UNIT;
    cycle->next_phase = (phase_idx + 1) % cycle->phase_count;

    GPIO_TypeDef *gpio_port = App_Led_Group_Get_GPIO_Port(data);
    uint16_t reset_pin_mask = App_Led_Group_Get_Pin_Mask(data, next_phase->colour, GPIO_PIN_RESET);
    uint16_t set_pin_mask = App_Led_Group_Get_Pin_Mask(data, next_phase->colour, GPIO_PIN_SET);
    HAL_GPIO_WriteMultipleStatePin(gpio_port, reset_pin_mask, set_pin_mask);

    uint32_t now = tx_time_get();
    tx_thread_sleep(cycle->next_wakeup - now);
  }
}

led_phase_t *App_Led_Group_Sequence_Select(int index)
{
  switch (index + 1)
  {
  case 1:
  	return group_1_sequence;
  case 2:
  	return group_2_sequence;
  case 3:
  	return group_3_sequence;
  case 4:
  	return group_4_sequence;
  case 5:
  	return group_5_sequence;
  default:
  	return NULL;
  }
}

GPIO_TypeDef *App_Led_Group_Get_GPIO_Port(int index)
{
  switch (index + 1)
  {
  case 1:
    return GROUP1_R_GPIO_Port;
  case 2:
    return GROUP2_R_GPIO_Port;
  case 3:
    return GROUP3_R_GPIO_Port;
  case 4:
    return GROUP4_R_GPIO_Port;
  case 5:
    return GROUP5_R_GPIO_Port;
  default:
    return NULL;
  }
}

uint16_t App_Led_Group_Get_Pin_Mask(int index, led_colour_t colour, GPIO_PinState set_reset)
{
  uint16_t mask = 0;
  if (colour & RGB_LED_RED == set_reset) mask |= App_Led_Group_Get_Red_Pin_Mask(index);
  if (colour & RGB_LED_GREEN == set_reset) mask |= App_Led_Group_Get_Green_Pin_Mask(index);
  if (colour & RGB_LED_BLUE == set_reset) mask |= App_Led_Group_Get_Blue_Pin_Mask(index);
}

uint16_t App_Led_Group_Get_Red_Pin_Mask(int index)
{
  switch (index + 1)
  {
  case 1:
    return GROUP1_R_Pin;
  case 2:
    return GROUP2_R_Pin;
  case 3:
    return GROUP3_R_Pin;
  case 4:
    return GROUP4_R_Pin;
  case 5:
    return GROUP5_R_Pin;
  default:
    return 0;
  }
}

uint16_t App_Led_Group_Get_Green_Pin_Mask(int index)
{
  switch (index + 1)
  {
  case 1:
    return GROUP1_G_Pin;
  case 2:
    return GROUP2_G_Pin;
  case 3:
    return GROUP3_G_Pin;
  case 4:
    return GROUP4_G_Pin;
  case 5:
    return GROUP5_G_Pin;
  default:
    return 0;
  }
}

uint16_t App_Led_Group_Get_Blue_Pin_Mask(int index)
{
  switch (index + 1)
  {
  case 1:
    return GROUP1_B_Pin;
  case 2:
    return GROUP2_B_Pin;
  case 3:
    return GROUP3_B_Pin;
  case 4:
    return GROUP4_B_Pin;
  case 5:
    return GROUP5_B_Pin;
  default:
    return 0;
  }
}
/* USER CODE END 1 */
