# Wireless PC Communication with the Arduino Nano 33 Series
This repository showcases different methods to create a wireless communication between the Arduino Nano 33 Series boards and a PC. The Arduino Nano 33 Series is made up of three boards, all of which have an integrated IMU sensor:

- The 9-axis IMU module (LSM9DS1) is present in the Nano 33 BLE and Nano 33 BLE Sense modules
- The 6-axis IMU module (LSM6DS3) is present in the Nano 33 IoT module

While these boards are very convenient for rapid prototyping, wireless connections between these Arduinos and a PC can be frustrating, especially for Windows users. Here is a guide to 2 quick solutions to get your Arduino 33 board connected to your PC.

## BLE Communication through a 'Dongle'
**You will need:** 2x Arduino Nano 33 BLE/BLE Sense

A way to get these Arduinos connected to your PC is by having one of your (A) Nano 33 BLE/Sense communicate via BLE to another (B) Nano 33 BLE/Sense, which will be connected to a PC by USB. The latter (B) will act as a Bluetooth 'dongle' and will transmit data received from the other Arduino (A) to the PC through serial communication. In short, the wireless Arduino will act as a peripheral device, and the Arduino connected to the PC will act as a central device listening to data from the peripheral and redirecting it to the PC.

![octocat](https://github.com/pili-zhangqiu/Wireless-PC-Communication-with-the-Arduino-Nano-33-Series/blob/main/img/Arduino_BLE-IMU.jpg)

To do this, upload the following files to your devices:
- **BLE_IMU_CENTRAL.ino**: upload to the Arduino BLE/Sense connected to the PC (aka. dongle or A)
- **BLE_IMU_PERIPHERAL.ino**: upload to the other Arduino BLE/Sense (B)

These scripts will allow the central device to read the gyroscope data from the peripheral, as well as sending commands to the integrated LED in the peripheral to blink.

## Unlocking the Bluetooth Classic Feature in the Nano 33 Iot
**You will need:** 1x Arduino Nano 33 IoT

While the original boards only feature BLE, there is one way to use Bluetooth Classic in the Nano 33 Series, especifically in the IoT board. The following information was extracted from the [user *danielkopiec*'s post in the Arduino forum][1].

**About the Nano 33 IoT board**
The Arduino NANO 33 IoT's PCB integrates 2 modules: Cortex M0 SAMD21 microcontroller and the NINA W102 communication module. In the original version of the NANO 33 IoT Bluetooth Classic is not supported but only BLE. However, this is not limited by hardware but only by the software implemented in NINA module. 

There is a way to hack into the NINA module by connecting to the PCB back pins with a UART to USB converter (e.g. CH340 board)
![octocat](https://github.com/pili-zhangqiu/Wireless-PC-Communication-with-the-Arduino-Nano-33-Series/blob/main/img/Iot33_Pins.jpg)

https://github.com/pili-zhangqiu/Wireless-PC-Communication-with-the-Arduino-Nano-33-Series/blob/main/img/FlexSleeve_BTDataFlow.jpg
Unfortunately, it is not possible to unlock the Bluetooth Classic functionality in the Nano BLE Sense.

[1]: https://www.youtube.com/watch?v=4OFo_bktEWM

