# 3-Wheel robot
## PCB
The PCB is the main controller of the 3-wheel robot. Its centered around an STM32F401 MCU with a few peripherials and headers:
- Dual motor driver
- WS2812 RGB LED
- USB-C for programming/debug
- Power supply switchable between 2S Li-ion batteries or USB power.
- 2 Encoder headers
- ESP8266 module header
- MPU6050 module header

### Schematic
I used the provided schematic to ensure compatibility with parts provided in class.
<img width="1063" height="727" alt="image" src="https://github.com/user-attachments/assets/d659c931-cfa2-4495-a3f4-532b36db4d60" />


### Layout
The layout was designed with the following (self imposed) constraints:
- Must use an Arduino UNO board outline.
  - Screw hole locations should be in the same place
  - USB header should be in the same place
- Headers should be on the edge of the board for easy access.
- MPU6050 and ESP8266 modules should not hang off the board.
- All silkscreen labels should be visible and unobstructed.

I placed the motor headers as close to the motor driver as possible, and the motor driver as close to the battery header as possible. This minimized the resistance of this high current path and allowed me to use a "power island" for battery power instead of a trace.
The regulators were also placed as close to the battery header as possible for similar reasons.
All 4 status leds were placed next to each other for cleanliness purposes and all header pins were labelled.
<img width="980" height="765" alt="image" src="https://github.com/user-attachments/assets/bd60e7ae-e6d5-4efd-8b6c-3c503d0461d0" />

## Code
Most of the code was straightforward. The motors simply needed one of two signals for direction and a pwm signal driven by a timer for speed.
The UART from the ESP8266 is received one byte at a time using an interrupt while the transmit uses a buffer and DMAs an entire string at a time.
The MPU6050 uses polling reads/writes to do initial configuration, then uses a DMA based approach for reading the registers. Each DMA burst is triggered by the interrupt from the MPU6050. The raw register data is then offset by a measured offset value, converted to float, then combined using a kalman filter. Acceleration data is weighted by how close its magnitude is to 1g. If its very far from 1g its likely untrustworthy and is ignored. If its exactly 1g its prioritized over gyro data.
MPU6050 code can be found in MPU6050.c/MPU6050.h
The WS2812 was by far the most difficult to achieve and I ended up directly writing the registers of the timer driving it. I wasn't able to get the HAL to work for it. My initial attempt used a DMA based approach to DMA the RGB bits to the LED, but after the transfer completed it would continnue sending pulses until software could turn it off and software was not fast enough to disable the timer in time. I ended up using one-pulse mode with an interrupt after each pulse to generate the pulses. This isnt exactly an optimal approach but it appears to work.
WS2812 driver can be found in WS2812.c/WS2812.h
