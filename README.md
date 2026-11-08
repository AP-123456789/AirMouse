# AirMouse

The **AirMouse** is a wrist-worn wireless mouse designed to control a computer cursor through wrist movements. It is built around the Seeed Studio XIAO ESP32-C3 and an MPU6050 motion sensor, with Bluetooth Low Energy (BLE) planned for wireless cursor communication. Physical buttons provide left-click, right-click, and cursor-enable functions.

## Project Design & Development Overview

1. **Schematic & Component Selection:** Designed in KiCad using the Seeed Studio XIAO ESP32-C3 and MPU6050. Added three control buttons, a power switch, LiPo battery connector, status LED, resistors, and capacitors. Connected the MPU6050 to the microcontroller through the I²C bus and wired the button, power, and LED circuits.

    <img width="512" height="290" alt="schematic" src="https://github.com/user-attachments/assets/ed7c21fb-e374-4be6-92f1-7837df9ff853" />


2. **Rules Checking & Footprints:** Corrected schematic power and pin issues, added appropriate No Connect markers and power flags, and completed KiCad Electrical Rules Check (ERC) with zero errors. Assigned physical footprints to the XIAO ESP32-C3, MPU6050, buttons, switch, LED, resistors, capacitors, and battery connector.

     <img width="512" height="290" alt="image" src="https://github.com/user-attachments/assets/6e1f1c6b-5bcf-422c-a201-a17cefd62382" />


3. **PCB Layout & Routing:** Arranged the XIAO ESP32-C3, MPU6050, control buttons, power switch, battery connector, LED circuit, and passive components on the PCB. Defined the board boundary using `Edge.Cuts`, routed power, ground, I²C, button, and LED traces, and corrected track-width issues until the configured Design Rules Check (DRC) reported zero violations and zero unconnected items.

   <img width="512" height="290" alt="PCB Layout" src="https://github.com/user-attachments/assets/a0889a03-4888-4b9c-89bf-da0a50c4c7ca" />


4. **Manufacturing Preparation:** Added a GND copper zone on the `B.Cu` layer, generated the Gerber fabrication layers, inspected the exported files in Gerber Viewer, and exported a STEP model from KiCad’s 3D Viewer for mechanical visualization and future enclosure integration.

   <img width="512" height="290" alt="image" src="https://github.com/user-attachments/assets/5925d399-2e87-4dc7-89a0-52fce226c3d2" />


5. **Firmware Development:** Created the AirMouse firmware project in Visual Studio Code using PlatformIO. Selected the Seeed Studio XIAO ESP32-C3 board and Arduino framework, configured `platformio.ini`, added the BLE mouse and NimBLE dependencies, and successfully compiled the initial firmware project.

   <img width="512" height="290" alt="image" src="https://github.com/user-attachments/assets/2bfe13e5-84f0-4c45-bd29-c02b6f972c03" />


## Key Features

- **Wearable Cursor Control:** Intended wrist-motion-based computer cursor control.
- **Microcontroller Core:** Seeed Studio XIAO ESP32-C3 for sensor processing and BLE communication.
- **Motion Sensing:** MPU6050 accelerometer and gyroscope for detecting wrist movement.
- **Wireless Communication:** Bluetooth Low Energy (BLE) mouse communication.
- **Physical Controls:** Left-click, right-click, and cursor-enable buttons.
- **Portable Power:** 3.7 V LiPo battery and power switch.
- **Status Indication:** LED circuit for device status.
- **Custom PCB Design:** KiCad PCB layout with routed traces, ground copper zone, and manufacturing outputs.

## Hardware Pinout and Schematic Mapping

| **Microcontroller Pin** | **Connected Hardware Component** | **Signal / Net** | **Function** |
|---|---|---|---|
| D6 | MPU6050 SDA | `I2C_SDA` | I²C data |
| D7 | MPU6050 SCL | `I2C_SCL` | I²C clock |
| 3.3 V | MPU6050 VDD / VLOGIC | `3V3` | Sensor power |
| GND | MPU6050 and circuit ground | `GND` | Common ground |
| Button GPIOs | Left-click, right-click, cursor-enable switches | Button signals | User input |
| LED GPIO | Status LED through 1 kΩ resistor | LED signal | Status indication |

*Confirm the final button and LED GPIO assignments from the schematic/firmware before using this table as a wiring reference.*

## Repository Structure

```text
AirMouse/
├── Hardware/
│   ├── AirMouse.kicad_pro       # KiCad project file
│   ├── AirMouse.kicad_sch       # Schematic
│   ├── AirMouse.kicad_pcb       # PCB layout
│   ├── Gerbers/                  # Gerber and drill outputs
│   
├── Firmware/
│   ├── platformio.ini            # PlatformIO configuration
│   └── src/
│       └── main.cpp               # Firmware source
├── images/                         # Project screenshots
└── README.md                       # Project documentation
```

## Firmware Setup and Compilation

### Prerequisites

1. Install [Visual Studio Code](https://code.visualstudio.com/).
2. Install the **PlatformIO IDE** extension from the VS Code Marketplace.

### Build Instructions

1. Clone or download this repository.
2. Open the `Firmware` directory in Visual Studio Code.
3. Check the board configuration and dependencies in `platformio.ini`:

   ```ini
   [env:seeed_xiao_esp32c3]
   platform = espressif32
   board = seeed_xiao_esp32c3
   framework = arduino
   monitor_speed = 115200

   lib_deps =
       hijelhub/HijelHID_BLEMouse
       h2zero/NimBLE-Arduino
   ```

4. Build the project using the PlatformIO toolbar or terminal:

   ```bash
   pio run
   ```

5. The initial firmware project compiled successfully. Uploading to a physical board, BLE pairing, cursor movement, and button testing are pending.

## References

- [KiCad Official Documentation](https://www.kicad.org/documentation/)
- [Seeed Studio XIAO ESP32-C3 Getting Started Guide](https://wiki.seeedstudio.com/XIAO_ESP32C3_Getting_Started/)
- [MPU-6050 Product Specification and Register Map](https://invensense.tdk.com/products/motion-tracking/6-axis/mpu-6050/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Visual Studio Code](https://code.visualstudio.com/
