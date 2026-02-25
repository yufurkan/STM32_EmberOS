# EmberOS

Real-time flight control software enabling autonomous capabilities for the RC MH-EmberEye UAV. Built on STM32F446RE & FreeRTOS with C++ architecture, featuring a Hardware-in-the-Loop (HIL) testing pipeline.

> *This project is under construction.*

## Overview
EmberOS is designed as a robust, real-time operating system for autonomous flight control, targeting seamless integration with the MH-EmberEye UAV platform. It leverages advanced sensor fusion, precise control algorithms, and thread-safe state management to ensure reliable performance in dynamic environments. The system aims to support manual control, stabilized flight modes, and full autonomy, with built-in failsafe mechanisms for enhanced safety.

## Targeted Features
The project aims to incorporate the following key features:

* **Real-Time Operating System:** FreeRTOS for efficient multitasking and scheduling, including dedicated tasks for sensor processing, control loops, and status indicators.
* **Thread-Safe State Management:** RTOS mutexes to protect flight states (e.g., RC commands including roll, pitch, yaw, throttle, and arming status), preventing data races in multi-threaded operations.
* **Sensor Fusion:** Complementary filter-based processing of MPU6050 accelerometer and gyroscope data for accurate roll and pitch angle estimation.
* **High-Frequency Control Loop:** Synchronous PID loop operating at 250 Hz (4 ms cycle) for responsive attitude control.
* **Custom PID Controller:** Advanced PID implementation with anti-windup protection, output saturation limits, and tunable gains (Kp, Ki, Kd) for roll and pitch axes.
* **PWM Outputs Layer:** Hardware timer-based PWM generation at 50 Hz for servo and motor mixing, enabling precise control of actuators.
* **DMA-Based UART Communication Pipeline:** Efficient, interrupt-free data transfer for RC receiver integration (e.g., Flysky protocol via UART/PPM), supporting reliable command input.
* **Flight Modes and Failsafe:** Support for manual and stabilize modes, with failsafe protections for signal loss, including autonomous software layers to handle emergency scenarios.
* **Hardware-in-the-Loop (HIL) Testing:** Integration with Unity 3D-modeled virtual environments for command and control simulations, allowing safe testing of flight dynamics.
* **Autonomous Mission Layers:** Progression to full autonomous tasks, building on failsafe autonomy for real-world missions such as waypoint navigation and obstacle avoidance.

## Technologies and Components
EmberOS utilizes the following technologies and components:

* **Microcontroller:** STM32F446RE (ARM Cortex-M4 core) for high-performance embedded processing.
* **RTOS:** FreeRTOS for real-time task management, semaphores, and mutexes.
* **Programming Language:** C++ for object-oriented design, including classes for PID controllers, IMU sensors, system management, and status indicators.
* **Sensors:** MPU6050 IMU connected via I2C for acceleration, gyroscope, and fused attitude data.
* **Peripherals and Interfaces:** I2C for sensor communication and scanning; UART (with planned DMA support) for debugging, RC receiver input, and serial data pipelines; SPI for potential expansions; GPIO for status LEDs and general I/O.
* **Control Algorithms:** Custom PID with anti-windup and saturation; complementary filter for sensor fusion.
* **Development Tools:** STM32Cube HAL library for peripheral initialization (e.g., clock config, GPIO, I2C, UART, SPI); RTOS kernel for thread creation and scheduling.
* **Simulation Environment:** Unity 3D for HIL testing, enabling virtual world interactions for validation of control and autonomous features.