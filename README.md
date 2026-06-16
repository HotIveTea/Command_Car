# Command Car: STM32 From Scratch

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Python](https://img.shields.io/badge/Language-Python-yellow.svg)
![Build](https://img.shields.io/badge/Build-Make-orange.svg)
![MCU](https://img.shields.io/badge/MCU-STM32F108-green.svg)
![Status](https://img.shields.io/badge/Status-Active_Learning-success.svg)

## Documents

| Files                  | Description                                                                             |
| ---------------------- | --------------------------------------------------------------------------------------- |
| [README.md](README.md) | Main project overview, hardware information, firmware features, and operating principle |
| [Build](./Build/)      | Contains files.o, .elf, .map                                                            |
| [Inc](./Inc/)          | Header files, register maps, macros                                                     |
| [Src](./Src/)          | Source C files (main.c, peripheral drivers)                                             |
| [Gui](./Gui/)          | Includes GUI using Python                                                               |

## Project Overview

This project implements a bare-metal Command Car system based on the STM32F108C microcontroller.
The system uses an HC-05 Bluetooth module for receiving navigation commands and a TB6612FNG driver for controlling the DC motors.
The project demonstrates embedded firmware development concepts including:

- Bare-metal register-level programming
- Custom build system (Makefile)
- Memory mapping and custom linker scripts
- Hand-written startup code
- GPIO and USART interfacing
- Hardware timer configuration and PWM generation
- System clock (RCC) and interrupt (NVIC) management
- GUI using Python

## Hardware Components

| Components | Description              |
| ---------- | ------------------------ |
| MCU        | STM32F108C               |
| Driver     | TB6612FNG & V1 TT Motors |
| Bluetooth  | HC-05                    |
| Systick    | RCC & NVIC               |
| GUI        | App desgined by Python   |

## Schematic

<figure style="text-align: center;">
    <img src="Images\Screenshot 2026-05-14 090106.png" width="800" alt="Command Car">
    <figcaption style = "text-align: center">Figure 1: Command Car Schematic</figcaption>
</figure>

## Features

**Command Communication & Parsing**:

- Wireless serial communication via HC-05 Bluetooth module
- Custom command protocol processing (e.g., parsing direction and speed vectors)
- Robust string handling and data extraction on bare-metal MCU
- Error handling for invalid or corrupted incoming packets \
  _The sent packets should look something like this:_

```bash
  [direction][speed],[time] + [direction][speed],[time] + ......
```

- `direction`: L - Left, R - Right, F - Forward, B - Backward
- `speed`: Car's speed from 0 to 200
- `time`: The amount of time the device will run

**PC Control Application**:

- User-friendly GUI for seamless PC-to-MCU interaction
- Dynamic serial port configuration and connection management
- Intuitive directional control interface (on-screen buttons / keyboard mapping)
- Real-time command transmission and communication logging

**Vehicle Control Mechanics**:

- PWM-based variable speed control via hardware timers
- Dual DC motor driving using TB6612FNG
- Real-time mechanical response to parsed control inputs

## App

<figure style="text-align: center;">
    <img src="Images\Screenshot 2026-05-18 101738.png" width="800" alt="App">
    <figcaption style = "text-align: center">Figure 2: User's Interface (GUI)</figcaption>
</figure>

## System Operating Principle

**Core Logic: Circular Queue & RTOS Tasks**
By using a circular queue, we can parse a command string into multiple tasks and store them in the queue. This ensures the MCU has sufficient time to execute each task sequentially, allowing the motors to physically operate without missing instructions.

The concept is based on the Producer-Consumer pattern:

- `queue_head` acts as the Producer (parsing USART strings).
- `queue_tail` acts as the Consumer (executing motor controls).

## Getting Started

Prerequisites
To build this project, you will need a cross-compilation toolchain and build tools:

- `arm-none-eabi-gcc` (GNU Arm Embedded Toolchain)
- `make` (GNU Make)
- A flashing tool (e.g., OpenOCD, ST-Link Utility, or equivalent depending on the programmer used).

### Build Instructions

1. Clone the repository:
   ```bash
   git clone [https://github.com/HotIveTea/Command_Car](https://github.com/HotIveTea/Command_Car)
   cd Command_Car
   ```
2. Build the project:
   ```bash
   make all
   ```
   or
   ```bash
   mingw32-make
   ```
3. Flash the firmware to the board:
   ```bash
   make flash
   ```
   or
   ```bash
   mingw32-make flash
   ```

## Developer's Diary

I document my daily progress, bugs encountered, and architectural concepts learned on Notion. Feel free to follow along with my thought process:
[Bare-metal-coding](https://www.notion.so/Bare-metal-programming-155984656d2c836f813c01230064508a?source=copy_link)
I also document my daily progress, bugs and approachs about RTOS in Notion.
[RTOS from scratch](https://www.notion.so/RTOS-from-scratch-English-ver-33b984656d2c806c9579eb40ed18872a?source=copy_link)

<h3>Contact Me</h3>

<p>
  <a href="https://github.com/HotIveTea">
    <img src="https://img.shields.io/badge/GitHub-HotIveTea-181717?style=for-the-badge&logo=github&logoColor=white"/>
  </a>
  
  <a href="https://www.linkedin.com/in/nguyễn-kim-thành-5ba280389/">
    <img src="https://img.shields.io/badge/LinkedIn-Nguyễn%20Kim%20Thành-0A66C2?style=for-the-badge&logo=linkedin&logoColor=white"/>
  </a>
  
  <a href="mailto:nkimthanh47@gmail.com">
    <img src="https://img.shields.io/badge/Gmail-nkimthanh47%40gmail.com-EA4335?style=for-the-badge&logo=gmail&logoColor=white"/>
  </a>
</p>
