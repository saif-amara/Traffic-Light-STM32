/* ============================================================
 * Traffic Light Controller — STM32F401RETx
 * Author  : Seif Eddine Amara
 * Board   : STM32F4 (LQFP64), HSI @ 16 MHz
 * Toolchain: STM32CubeIDE + HAL
 * ============================================================
 *
 * Pin Mapping (GPIOA):
 *  PA0 → RED_H   (Horizontal Red)
 *  PA1 → YEL_H   (Horizontal Yellow)
 *  PA2 → GRN_H   (Horizontal Green)
 *  PA3 → RED_V   (Vertical Red)
 *  PA4 → YEL_V   (Vertical Yellow)
 *  PA5 → GRN_V   (Vertical Green)
 *
 * Traffic Sequence:
 *  [1] Vertical   GO   → GRN_V + RED_H   (5 s)
 *  [2] Vertical   SLOW → YEL_V + RED_H   (2 s)
 *  [3] Horizontal GO   → GRN_H + RED_V   (5 s)
 *  [4] Horizontal SLOW → YEL_H + RED_V   (2 s)
 * ============================================================ */

#include "stm32f4xx_hal.h"

/* ── Pin Definitions ──────────────────────────────────────── */
#define RED_H   GPIO_PIN_0
#define YEL_H   GPIO_PIN_1
#define GRN_H   GPIO_PIN_2
#define RED_V   GPIO_PIN_3
#define YEL_V   GPIO_PIN_4
#define GRN_V   GPIO_PIN_5

#define ALL_PINS (RED_H | YEL_H | GRN_H | RED_V | YEL_V | GRN_V)

/* ── Timing Constants (ms) ────────────────────────────────── */
#define DELAY_GO    5000U   /* Green phase duration  */
#define DELAY_SLOW  2000U   /* Yellow phase duration */

/* ── Function Prototypes ──────────────────────────────────── */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void Traffic_SetState(uint16_t pins_on);

/* ============================================================
 * Main
 * ============================================================ */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    while (1)
    {
        /* State 1 — Vertical GO: GRN_V + RED_H */
        Traffic_SetState(GRN_V | RED_H);
        HAL_Delay(DELAY_GO);

        /* State 2 — Vertical SLOW: YEL_V + RED_H */
        Traffic_SetState(YEL_V | RED_H);
        HAL_Delay(DELAY_SLOW);

        /* State 3 — Horizontal GO: GRN_H + RED_V */
        Traffic_SetState(GRN_H | RED_V);
        HAL_Delay(DELAY_GO);

        /* State 4 — Horizontal SLOW: YEL_H + RED_V */
        Traffic_SetState(YEL_H | RED_V);
        HAL_Delay(DELAY_SLOW);
    }
}

/* ============================================================
 * Traffic_SetState
 * Turns ON only the specified pins, turns OFF all others.
 * This prevents any accidental simultaneous conflicting signals.
 * ============================================================ */
static void Traffic_SetState(uint16_t pins_on)
{
    HAL_GPIO_WritePin(GPIOA, ALL_PINS,  GPIO_PIN_RESET); /* all off first */
    HAL_GPIO_WritePin(GPIOA, pins_on,   GPIO_PIN_SET);   /* then set active */
}

/* ============================================================
 * GPIO Initialization
 * PA0–PA5 configured as Push-Pull outputs, no pull resistor
 * ============================================================ */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* Set all output pins low before configuring direction */
    HAL_GPIO_WritePin(GPIOA, ALL_PINS, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = ALL_PINS;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* ============================================================
 * System Clock Configuration
 * Source : HSI (16 MHz internal oscillator)
 * No PLL — sufficient for GPIO toggling application
 * ============================================================ */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    RCC_OscInitStruct.HSIState            = RCC_HSI_ON;
    RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType      = RCC_CLOCKTYPE_HCLK  | RCC_CLOCKTYPE_SYSCLK |
                                       RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}
