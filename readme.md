# 🚀 EmberOS - Autonomous Flight Controller

**EmberOS** is a FreeRTOS-based autonomous flight software that runs on the STM32 ARM Cortex-M4 architecture; it provides thread-safe state management with RTOS mutexes, complementary filter-based IMU sensor fusion, and a custom PID controller with anti-windup protection.

## 🛠️ Key Features (Current Status)
* **Operating System:** FreeRTOS
* **Thread-Safe Architecture:** Flight states (`RCState`) are protected with osMutex to prevent race-conditioning (data splitting).
* **Sensor Fusion:** MPU6050 accelerometer and gyroscope data are processed with a complementary filter.
* **Control Loop:** 250Hz (4ms) synchronous PID loop. * **PID Brain:** Special PID class with Anti-Windup and output limiter (Saturation) for Roll and Pitch axes.

## 🚧 Upcoming Features (Roadmap)
- [ ] 50Hz PWM Servo/Motor Mixer with STM32 Timer hardware.
- [ ] Flysky RC Receiver integration (UART/PPM).
- [ ] Failsafe (Signal loss protection) and Flight Modes (Manual/Stabilize).
- [ ] Hardware-in-the-Loop (HIL) Simulation Tests via Unity 3D.

---
*Developing by Yusuf Furkan Umutlu*