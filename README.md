# WAVENGER: 4WD Omnidirectional Mecanum Robotic Platform with Dual Gesture & Web Teleoperation

WAVENGER is a high-agility, 4-wheel drive holonomic robotic platform engineered around dual **ESP32 microcontrollers** and **Mecanum wheels**. The platform achieves 3-degree-of-freedom planar mobility (forward/reverse translation, lateral strafing, diagonal tracking, and rotational yaw) driven by an onboard kinematic mixing engine. Teleoperation is achieved through two concurrent wireless control pipelines: an ultra-low latency **ESP-NOW wearable inertial gesture remote (MPU-6050)** and a standalone **HTML5/TailwindCSS web control dashboard** operating over local HTTP REST endpoints.

---

## Key Features

- **Holonomic Mecanum Drive Kinematics:** 4 independently actuated DC gear motors mapped with 12 distinct directional vectors:
  - Linear: Forward (`F`), Backward (`B`)
  - Rotational: Rotate Left (`L`), Rotate Right (`R`)
  - Pure Lateral: Left Strafe (`Left_Side`), Right Strafe (`Right_Side`)
  - Diagonal: Forward-Left (`G`), Forward-Right (`H`), Backward-Left (`I`), Backward-Right (`J`)
  - Intermediate Vectors: Lateral Front-Left (`D`), Front-Right (`K`), Back-Left (`N`), Back-Right (`V`)
- **Dual Teleoperation Architecture:**
  1. **ESP-NOW Peer-to-Peer Pipeline:** Sub-10ms direct MAC-addressed wireless link transmitting inertial attitude packets from the gesture remote without requiring an intermediate Wi-Fi router.
  2. **Asynchronous HTTP WebServer Dashboard:** Embedded `/control?command=<CMD>` REST endpoint paired with an interactive 360° analog touch joystick, dynamic IP configuration, and instant telemetry feedback.
- **Inertial Attitude Estimation:** Wearable transmitter calculates continuous trigonometric Pitch ($\theta$) and Roll ($\phi$) from raw MPU-6050 accelerometer and gyroscope data with dead-zone noise filtering ($\pm 15^\circ$).
- **Dual-Rail Power Architecture:** High-current motor supply rail isolated from sensitive 3.3V/5V logic electronics via buck regulation (LM2596) to eliminate inductive flyback noise and brownout risks.
- **Auxiliary Actuation:** Software-toggled visual headlight illumination on `GPIO 2` and horn alert telemetry.

---

## System Architecture

```text
[Wearable Gesture Transmitter]
  └── ESP32 + MPU-6050 (Pitch/Roll vector parsing)
              │
              ▼ (ESP-NOW 2.4GHz Peer-to-Peer / Sub-10ms)
[WAVENGER Rover Core: ESP32] ◄─── (Wi-Fi 802.11 b/g/n HTTP /control) ─── [HTML5 Web Controller]
  ├── Dual Teleoperation (ESP-NOW Callback + Embedded WebServer)
  ├── Kinematic Motion Parsing (12 Directional Vector Matrix)
  └── Dual L298N / Dual H-Bridge Driver Stage
              │
              ▼
[4WD Mecanum Wheel Powertrain]
  ├── Front-Left Motor (GPIO 25, 32 | ENA: GPIO 13)
  ├── Front-Right Motor (GPIO 14, 26 | ENA: GPIO 13)
  ├── Back-Left Motor  (GPIO 19, 18 | ENB: GPIO 15)
  └── Back-Right Motor (GPIO 16, 4  | ENB: GPIO 15)
```

---

## Hardware Interconnect & Schematics

### 1. WAVENGER Rover (Receiver Unit)
<p align="center">
  <img src="docs/WAVENGER%20Rover%20(Receiver%20Unit)%20Schematic%20Connection%20Diagram.png" alt="WAVENGER Rover Schematic" width="100%">
</p>

> **Vector Document:** Full high-resolution vector PDF is archived under [`docs/WAVENGER Rover (Receiver Unit) Schematic Connection Diagram.pdf`](docs/WAVENGER%20Rover%20(Receiver%20Unit)%20Schematic%20Connection%20Diagram.pdf).

---

### 2. Gesture Remote (Transmitter Unit)
<p align="center">
  <img src="docs/Gesture%20Remote%20(Transmitter%20Unit)%20Schematic%20Connection%20Diagram.png" alt="Gesture Remote Schematic" width="100%">
</p>

> **Vector Document:** Full high-resolution vector PDF is archived under [`docs/Gesture Remote (Transmitter Unit) Schematic Connection Diagram.pdf`](docs/Gesture%20Remote%20(Transmitter%20Unit)%20Schematic%20Connection%20Diagram.pdf).

---

## Detailed Hardware Pinout Matrix

### 1. WAVENGER Rover Controller (ESP32 Development Node)
| Subsystem / Peripheral | Pin Label | ESP32 Pin | Signal Type | Logic Level | Functional Description |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Motor Driver 1 Enable** | `ENA` | `GPIO 13` | Output (PWM) | 3.3V Logic | Hardware PWM speed regulation for Front Axle motors (0–255) |
| **Front-Left Motor (M1)** | `m1r` / `m1b` | `GPIO 25` / `GPIO 32` | Digital Output | 3.3V Logic | Directional H-Bridge inputs for Motor 1 (Forward/Reverse) |
| **Front-Right Motor (M2)** | `m2r` / `m2b` | `GPIO 14` / `GPIO 26` | Digital Output | 3.3V Logic | Directional H-Bridge inputs for Motor 2 (Forward/Reverse) |
| **Motor Driver 2 Enable** | `ENB` | `GPIO 15` | Output (PWM) | 3.3V Logic | Hardware PWM speed regulation for Rear Axle motors (0–255) |
| **Back-Left Motor (M3)** | `m3r` / `m3b` | `GPIO 19` / `GPIO 18` | Digital Output | 3.3V Logic | Directional H-Bridge inputs for Motor 3 (Forward/Reverse) |
| **Back-Right Motor (M4)** | `m4r` / `m4b` | `GPIO 16` / `GPIO 4` | Digital Output | 3.3V Logic | Directional H-Bridge inputs for Motor 4 (Forward/Reverse) |
| **Auxiliary Headlight** | `LED` | `GPIO 2` | Digital Output | Active-HIGH | Software-toggled LED illumination |
| **Logic Power In** | `VIN` / `GND` | `VIN` / `GND` | Power | 5.0VDC / 0V | Regulated 5V logic input from LM2596 step-down rail |

---

### 2. Gesture Remote Transmitter (ESP32 + MPU-6050)
| Subsystem / Peripheral | Module Pin | ESP32 Pin | Signal Type | Voltage Rail | Functional Description |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **MPU-6050 6-DoF IMU** | `VCC` | `3V3` | Power Rail | 3.3VDC | Core sensor power rail |
| | `GND` | `GND` | Ground Reference | 0V | Common system ground |
| | `SDA` | `GPIO 21` | Bidirectional Open-Drain | 3.3V Logic | Hardware I2C Serial Data bus |
| | `SCL` | `GPIO 22` | Bidirectional Open-Drain | 3.3V Logic | Hardware I2C Serial Clock bus |
| **Power Input** | `3V3` / `GND` | `3V3` / `GND` | Power Rail | 3.3VDC / 0V | Regulated 3.3V supply from onboard LDO / 3.7V LiPo |

---

## Holonomic Motion Kinematics Mapping

The Mecanum wheel layout resolves translational and rotational forces by varying wheel rotation directions:

| Command Name | Byte Code | Front-Left (M1) | Front-Right (M2) | Back-Left (M3) | Back-Right (M4) | Kinematic Motion Result |
| :--- | :---: | :---: | :---: | :---: | :---: | :--- |
| **Forward** | `'F'` | Forward | Forward | Forward | Forward | Longitudinal Forward Translation |
| **Backward** | `'B'` | Reverse | Reverse | Reverse | Reverse | Longitudinal Reverse Translation |
| **Rotate Left** | `'L'` | Reverse | Forward | Reverse | Forward | In-place Counter-Clockwise Yaw |
| **Rotate Right** | `'R'` | Forward | Reverse | Forward | Reverse | In-place Clockwise Yaw |
| **Lateral Left** | `'Left_Side'` | Reverse | Forward | Forward | Reverse | True 90° Left Lateral Strafe |
| **Lateral Right** | `'Right_Side'` | Forward | Reverse | Reverse | Forward | True 90° Right Lateral Strafe |
| **Diagonal FL** | `'G'` | Forward | Off | Off | Forward | 45° Forward-Left Translation |
| **Diagonal FR** | `'H'` | Off | Forward | Forward | Off | 45° Forward-Right Translation |
| **Diagonal BL** | `'I'` | Off | Reverse | Reverse | Off | 45° Backward-Left Translation |
| **Diagonal BR** | `'J'` | Reverse | Off | Off | Reverse | 45° Backward-Right Translation |
| **Stop** | `'S'` | Off | Off | Off | Off | Instant Dynamic Braking / Idle |
| **Horn** | `'Y'` | — | — | — | — | Serial / Acoustic Alert Trigger |
| **Light** | `'Z'` | — | — | — | — | Auxiliary Headlight State Toggle |

---

## Software & Library Requirements

### External Libraries (`libraries.txt`)
- `Adafruit MPU6050` (by Adafruit) — IMU data acquisition and register configuration
- `Adafruit Unified Sensor` (by Adafruit) — Sensor driver hardware abstraction layer

### Core Built-in ESP32 Libraries
- `WiFi.h` — 802.11 b/g/n station mode and local network connectivity
- `esp_now.h` — Espressif proprietary 2.4GHz peer-to-peer radio protocol
- `WebServer.h` — Embedded HTTP server routing incoming REST query requests
- `Wire.h` — Hardware I2C communication engine

---

## Project Structure

```text
WAVENGER-4WD-Platform/
├── .gitignore
├── LICENSE
├── libraries.txt
├── README.md
├── web_controller/
│   └── controller.html
├── src/
│   ├── WAVENGER_Rover.ino
│   └── Gesture_Transmitter.ino
└── docs/
    ├── Gesture Remote (Transmitter Unit) Schematic Connection Diagram.pdf
    ├── Gesture Remote (Transmitter Unit) Schematic Connection Diagram.png
    ├── WAVENGER Rover (Receiver Unit) Schematic Connection Diagram.pdf
    └── WAVENGER Rover (Receiver Unit) Schematic Connection Diagram.png
```

---

## Getting Started & Deployment

### 1. MAC Address Pairing (ESP-NOW)
1. Flash a temporary sketch reading `WiFi.macAddress()` to the **WAVENGER Rover ESP32**.
2. Note the 6-byte hexadecimal MAC address (e.g., `{0x24, 0x6F, 0x28, 0xAA, 0xBB, 0xCC}`).
3. Open `src/Gesture_Transmitter.ino` and replace `receiverAddress[]` with the Rover's actual MAC address.

### 2. Firmware Flashing
1. Open the Arduino IDE or PlatformIO.
2. Target Board: **ESP32 Dev Module** (Baud Rate: `115200`).
3. Upload `src/WAVENGER_Rover.ino` to the rover chassis controller.
4. Upload `src/Gesture_Transmitter.ino` to the wearable remote controller.

### 3. Web Teleoperation Setup
1. Power on the WAVENGER Rover. It will connect to the configured local Wi-Fi network and print its assigned IP address over the Serial Monitor.
2. Open `web_controller/index.html` in any web browser on a smartphone, tablet, or PC connected to the same network.
3. Enter the Rover's IP in the configuration input bar and click **SET IP**.
4. Teleoperate the platform using the interactive 360° touch joystick, directional buttons, or keyboard arrow keys.

---

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.