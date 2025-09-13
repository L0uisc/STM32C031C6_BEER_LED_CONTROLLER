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
  const led_phase_t *const phase_sequence;
} led_cycle_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define NUM_LED_GROUPS 10
#define LED_THREAD_STACK_SIZE TX_MINIMUM_STACK
#define LED_THREAD_PRIORITY (TX_MAX_PRIORITIES >> 1)
#define TICKS_PER_TIME_UNIT 25
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define SEQUENCE_LENGTH(s) (sizeof(s) / sizeof(led_phase_t))
#define CYCLE_DEFINITION(s) { \
        .next_wakeup = 0, \
        .next_phase = 0, \
        .phase_count = SEQUENCE_LENGTH(s), \
        .phase_sequence = s \
}
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TX_SEMAPHORE tx_app_semaphore;
/* USER CODE BEGIN PV */
TX_THREAD led_group_threads[NUM_LED_GROUPS];

const led_phase_t group_1_sequence[] = {
		{ .duration = 1, .colour = RGB_LED_YELLOW }
};
const led_phase_t group_2_sequence[] = {
		{ .duration = 1, .colour = RGB_LED_WHITE }
};
const led_phase_t group_3_sequence[] = {
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 28, .colour = RGB_LED_OFF }
};
const led_phase_t group_4_sequence[] = {
		{ .duration = 4, .colour = RGB_LED_OFF },
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 24, .colour = RGB_LED_OFF }
};
const led_phase_t group_5_sequence[] = {
		{ .duration = 8, .colour = RGB_LED_OFF },
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 20, .colour = RGB_LED_OFF }
};
const led_phase_t group_6_sequence[] = {
		{ .duration = 12, .colour = RGB_LED_WHITE },
		{ .duration = 4, .colour = RGB_LED_OFF },
		{ .duration = 4, .colour = RGB_LED_BLUE },
		{ .duration = 4, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE }
};
const led_phase_t group_7_sequence[] = {
		{ .duration = 12, .colour = RGB_LED_BLUE },
		{ .duration = 5, .colour = RGB_LED_OFF },
		{ .duration = 3, .colour = RGB_LED_BLUE },
		{ .duration = 1, .colour = RGB_LED_OFF },
		{ .duration = 5, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE }
};
const led_phase_t group_8_sequence[] = {
		{ .duration = 12, .colour = RGB_LED_WHITE },
		{ .duration = 6, .colour = RGB_LED_OFF },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_OFF },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE }
};
const led_phase_t group_9_sequence[] = {
		{ .duration = 12, .colour = RGB_LED_BLUE },
		{ .duration = 7, .colour = RGB_LED_OFF },
		{ .duration = 1, .colour = RGB_LED_BLUE },
		{ .duration = 3, .colour = RGB_LED_OFF },
		{ .duration = 3, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE },
		{ .duration = 2, .colour = RGB_LED_WHITE },
		{ .duration = 2, .colour = RGB_LED_BLUE }
};
const led_phase_t group_10_sequence[] = {
		{ .duration = 1, .colour = RGB_LED_GREEN }
};

led_cycle_t led_cycle[NUM_LED_GROUPS] = {
		CYCLE_DEFINITION(group_1_sequence),
		CYCLE_DEFINITION(group_2_sequence),
		CYCLE_DEFINITION(group_3_sequence),
		CYCLE_DEFINITION(group_4_sequence),
		CYCLE_DEFINITION(group_5_sequence),
		CYCLE_DEFINITION(group_6_sequence),
		CYCLE_DEFINITION(group_7_sequence),
		CYCLE_DEFINITION(group_8_sequence),
		CYCLE_DEFINITION(group_9_sequence),
		CYCLE_DEFINITION(group_10_sequence)
};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */
VOID App_Led_Group_Function(ULONG data);
static inline const led_phase_t *const App_Led_Group_Sequence_Select(int index);
static inline GPIO_TypeDef *App_Led_Group_Get_GPIO_Port(int index);
static inline uint16_t App_Led_Group_Get_Pin_Mask(int index, led_colour_t colour, GPIO_PinState set_reset);
static inline uint16_t App_Led_Group_Get_Red_Pin_Mask(int index);
static inline uint16_t App_Led_Group_Get_Green_Pin_Mask(int index);
static inline uint16_t App_Led_Group_Get_Blue_Pin_Mask(int index);
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
  for (int i = 0; i < NUM_LED_GROUPS; ++i)
  {
    void *buffer;
    if ((ret = tx_byte_allocate(byte_pool, &buffer, LED_THREAD_STACK_SIZE, TX_NO_WAIT)) != TX_SUCCESS)
    {
      printf("Allocation error: %08X", ret);
      goto err;
    }

    if ((ret = tx_thread_create(&led_group_threads[i],
    		"LED group control thread",
			App_Led_Group_Function,
			i,
			buffer,
			LED_THREAD_STACK_SIZE,
			LED_THREAD_PRIORITY,
			LED_THREAD_PRIORITY,
			TX_NO_TIME_SLICE,
			TX_AUTO_START)) != TX_SUCCESS)
    {
      printf("Thread create error: %08X", ret);
      goto err;
    }
  }

  tx_time_set(0);

  for (int i = 0; i < NUM_LED_GROUPS; ++i)
  {
    if ((ret = tx_semaphore_ceiling_put(&tx_app_semaphore, NUM_LED_GROUPS)) != TX_SUCCESS)
    {
      printf("Semaphore put error: %08X", ret);
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
  tx_semaphore_get(&tx_app_semaphore, TX_WAIT_FOREVER);

  for (;;)
  {
    led_cycle_t *cycle = &led_cycle[data];
    int phase_idx = cycle->next_phase;
    const led_phase_t *const next_phase = &cycle->phase_sequence[phase_idx];
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

static inline const led_phase_t *const App_Led_Group_Sequence_Select(int index)
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
  case 6:
  	return group_6_sequence;
  case 7:
  	return group_7_sequence;
  case 8:
  	return group_8_sequence;
  case 9:
  	return group_9_sequence;
  case 10:
  	return group_10_sequence;
  default:
  	return NULL;
  }
}

static inline GPIO_TypeDef *App_Led_Group_Get_GPIO_Port(int index)
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
  case 6:
    return GROUP6_R_GPIO_Port;
  case 7:
    return GROUP7_R_GPIO_Port;
  case 8:
    return GROUP8_R_GPIO_Port;
  case 9:
    return GROUP9_R_GPIO_Port;
  case 10:
    return GROUP10_R_GPIO_Port;
  default:
    return NULL;
  }
}

static inline uint16_t App_Led_Group_Get_Pin_Mask(int index, led_colour_t colour, GPIO_PinState set_reset)
{
  uint16_t mask = 0;
  if ((colour & RGB_LED_RED) == (set_reset ? RGB_LED_RED : 0)) mask |= App_Led_Group_Get_Red_Pin_Mask(index);
  if ((colour & RGB_LED_GREEN) == (set_reset ? RGB_LED_GREEN : 0)) mask |= App_Led_Group_Get_Green_Pin_Mask(index);
  if ((colour & RGB_LED_BLUE) == (set_reset ? RGB_LED_BLUE : 0)) mask |= App_Led_Group_Get_Blue_Pin_Mask(index);

  return mask;
}

static inline uint16_t App_Led_Group_Get_Red_Pin_Mask(int index)
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
  case 6:
    return GROUP6_R_Pin;
  case 7:
    return GROUP7_R_Pin;
  case 8:
    return GROUP8_R_Pin;
  case 9:
    return GROUP9_R_Pin;
  case 10:
    return GROUP10_R_Pin;
  default:
    return 0;
  }
}

static inline uint16_t App_Led_Group_Get_Green_Pin_Mask(int index)
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
  case 6:
    return GROUP6_G_Pin;
  case 7:
    return GROUP7_G_Pin;
  case 8:
    return GROUP8_G_Pin;
  case 9:
    return GROUP9_G_Pin;
  case 10:
    return GROUP10_G_Pin;
  default:
    return 0;
  }
}

static inline uint16_t App_Led_Group_Get_Blue_Pin_Mask(int index)
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
  case 6:
    return GROUP6_B_Pin;
  case 7:
    return GROUP7_B_Pin;
  case 8:
    return GROUP8_B_Pin;
  case 9:
    return GROUP9_B_Pin;
  case 10:
    return GROUP10_B_Pin;
  default:
    return 0;
  }
}
/* USER CODE END 1 */
