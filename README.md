# Command Car: STM32 From Sccratch

![C](https://img.shields.io/badge/Language-C-blue.svg)
![Build](https://img.shields.io/badge/Build-Make-orange.svg)
![MCU](https://img.shields.io/badge/MCU-STM32F108-green.svg)
![Status](https://img.shields.io/badge/Status-Active_Learning-success.svg)

## Project Overview

This project is a deep dive into **Bare-Metal Coding** by focusing on the pratical implementation
of STM32F108C microcontroller to control a real-world application: a Command Car. Rather than relying on high-level Hardware Abstraction Layers (HAL) or standard peripheral libraries, this firmware is built entirely from scratch.

## Key Objectives

- **Custom Build System** : Utilizing a custom `Makefile` for compiling, assembling, and linking the firmware without relying on external IDE abstractions.
- **Memory Mapping & Linker Scripts** : Custom `.ld` linker scripts to precisely define memory regions (Flash, RAM) and section placement (`.text`, `.data`, `.bss`).
- **Startup Code** : Hand-written startup code in C/Assembly to initialize the vector table, copy the `.data` section to SRAM, zero out the `.bss` section, and branch to `main()`.
- **Register-Level Driver Development** : Direct memory access to configure and manipulate hardware registers.

## Hardware & Peripherals Used

The project interfaces with the following MCU peripherals at the register level to drive the Command Car's mechanics and communications:

- **GPIO (General Purpose Input/Output)**: Configured for sensor inputs, LED status indicators, and motor driver control signals.
- **TIMER**: Utilized for generating precise hardware delays and generating PWM signals to control the speed of the DC motors.
- **USART (Universal Synchronous/Asynchronous Receiver-Transmitter)**: Handles asynchronous serial communication for parsing incoming commands (e.g., from a Bluetooth module or serial terminal) to control the car's navigation.
- **Core Architecture (ARM Cortex-M / STM32)**: Managing system clocks (RCC) and interrupts (NVIC).
- **Hardware** : In this project, we will use HC-05 for Bluetooth and TB6612FNG for controlling motors

## Project Structure

_(Adjust the tree below based on your actual repository structure)_

```text
📦 Command_Car
 ┣ 📂 .vscode
 ┣ 📂 Inc               # Header files, register maps, macros
 ┣ 📂 Src               # Source C files (main.c, peripheral drivers)
 ┣ 📂 Build         # Contains files.o, .elf, .map
 ┣ 📜 Makefile          # Build automation
 ┣ 📜 stm32f103.ld    # Linker script for memory layout
 ┗ 📜 README.md         # Project documentation
```

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

## Philosophy

Bare-metal programming can be unforgiving, but it is deeply rewarding. A hard fault or a segmentation fault is just a stepping stone to fully mastering embedded systems. Keep tinkering, keep breaking things, and keep learning.
