# 🚦 Traffic Light Controller — STM32F401RETx

A dual-direction traffic light controller implemented on an STM32F4 microcontroller using STM32 HAL, with a Proteus simulation for verification.

---

## 📋 Overview

This project implements a 4-state traffic light finite state machine (FSM) controlling 6 LEDs across two directions (Vertical and Horizontal) using GPIO outputs on the STM32F401RETx.

The timing sequence mirrors real-world traffic light behavior:

| State | Active Signals         | Duration |
|-------|------------------------|----------|
| 1     | Vertical GREEN + Horizontal RED   | 5 s |
| 2     | Vertical YELLOW + Horizontal RED  | 2 s |
| 3     | Horizontal GREEN + Vertical RED   | 5 s |
| 4     | Horizontal YELLOW + Vertical RED  | 2 s |

---

## 🔌 Hardware

| Component        | Details                        |
|------------------|--------------------------------|
| Microcontroller  | STM32F401RETx (LQFP64)        |
| Clock Source     | HSI — Internal 16 MHz          |
| IDE              | STM32CubeIDE + HAL Library     |
| Simulation Tool  | Proteus 8                      |

---

## 📍 Pin Mapping (GPIOA)

| Pin  | Label  | Direction  | Color  |
|------|--------|------------|--------|
| PA0  | RED_H  | Horizontal | 🔴 Red    |
| PA1  | YEL_H  | Horizontal | 🟡 Yellow |
| PA2  | GRN_H  | Horizontal | 🟢 Green  |
| PA3  | RED_V  | Vertical   | 🔴 Red    |
| PA4  | YEL_V  | Vertical   | 🟡 Yellow |
| PA5  | GRN_V  | Vertical   | 🟢 Green  |

---

## 🗂️ Project Structure

```
Traffic-Light-STM32/
├── Core/
│   ├── Src/
│   │   ├── main.c                  ← Main application logic
│   │   ├── stm32f4xx_hal_msp.c     ← HAL MSP init
│   │   ├── stm32f4xx_it.c          ← Interrupt handlers
│   │   ├── syscalls.c              ← Newlib syscalls
│   │   └── sysmem.c                ← Heap management
│   └── Inc/
│       ├── main.h
│       ├── stm32f4xx_hal_conf.h
│       └── stm32f4xx_it.h
├── Simulation/
│   └── Traffic_Lights_.pdsprj      ← Proteus simulation file
├── STM32F401RETX_FLASH.ld          ← Linker script (Flash)
├── STM32F401RETX_RAM.ld            ← Linker script (RAM)
├── Traffic_Light_Project0.ioc      ← STM32CubeMX config
└── README.md
```

---

## ⚙️ How It Works

The main loop cycles through 4 states using a helper function `Traffic_SetState()` that:
1. Resets **all** pins first (safe state, no conflicts)
2. Sets only the **active** pins for the current state

This approach prevents any momentary overlap between conflicting signals (e.g., both directions showing GREEN simultaneously).

```c
static void Traffic_SetState(uint16_t pins_on)
{
    HAL_GPIO_WritePin(GPIOA, ALL_PINS, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, pins_on,  GPIO_PIN_SET);
}
```

---

## 🚀 Getting Started

### Requirements
- STM32CubeIDE (v1.14+)
- STM32Cube FW_F4 V1.28.3
- Proteus 8 (for simulation)

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/YOUR_USERNAME/Traffic-Light-STM32.git
   ```
2. Open STM32CubeIDE → `File > Open Projects from File System` → select the folder
3. Build: `Project > Build All`
4. Flash via ST-Link or run the Proteus simulation

---

## 🔮 Possible Improvements

- Replace `HAL_Delay()` (blocking) with **TIM timer interrupts** for non-blocking state transitions
- Add a **pedestrian button** input using GPIO EXTI interrupt
- Implement **UART debug logging** to monitor state changes in real time
- Add a **blinking yellow** safe mode triggered by a fault condition

---

## 👤 Author

**Seif Eddine Amara**
Student — Systèmes Embarqués, ISSAT Sousse, Tunisia
[GitHub](https://github.com/YOUR_USERNAME) | [LinkedIn](https://linkedin.com/in/YOUR_PROFILE)

---

## 📄 License

This project is open source under the [MIT License](LICENSE).
