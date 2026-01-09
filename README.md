# ⏱️ Laser Timing System

![DSCll00034-min (1)](https://github.com/user-attachments/assets/e5645aae-995b-4a71-b37e-b9b094e88524)


## 📖 Project Overview
This project is a low-cost, DIY **laser-based timing gate** built using a Raspberry Pi Pico. It serves as a proof-of-concept for an easily installable timing system that detects when a 5 mW laser beam is interrupted.


## ⚙️ How It Works

###  Laser Control & Safety
To prevent overheating, the Pico drives the base of a **PNP transistor** through a resistor. This allows the system to supply power to the laser only when needed rather than running it constantly.

###  Detection (LDR Sensor)
A **Light Dependent Resistor (LDR)** monitors the beam:
* **Beam Active:** Low resistance, signaling the GPIO.
* **Beam Broken:** High resistance, triggering a timing event.

###  User Interface & Stability
* **Push Button:** Used to toggle the system "ON" and start timing sessions.
* **Status LEDs:** Two LEDs (with 100 Ω resistors) indicate if the laser is active and if the beam is currently aligned.
* **Power Stability:** A **220 µF capacitor** provides bulk decoupling to stabilize the voltage when the laser switches states.
  
###  Web Connectivity & Data Logging
The Raspberry Pi Pico sends timing data directly to a web page using its **USB serial interface**. Modern browsers (such as Chrome and Edge) can connect to the Pico using the **Web Serial API**, allowing live data to be displayed without any external software or servers.

---

## 🛠️ Bill of Materials

### Main Hardware
| Item | Quantity | Purpose |
| :--- | :---: | :--- |
| **Raspberry Pi Pico** | 1 | Microcontroller |
| **5 mW Laser Module** | 1 | Beam Source |
| **LDR** | 1 | Detection Sensor |
| **PNP Transistor** | 1 | Power Management |
| **3D-Printed Case** | 1 | Housing |

### Components & Passives
| Component | Value | Qty |
| :--- | :--- | :---: |
| **Resistors** | 100 Ω | 2 |
| **Capacitor** | 220 µF | 1 |
| **LEDs** | Standard | 2 |
| **Push Button** | Momentary | 1 |

## 📖 Website Overview

<img width="1291" height="1284" alt="image" src="https://github.com/user-attachments/assets/dc38505e-27ff-4e0e-b9ac-643d198d199b" />


The web page acts as a live dashboard for the laser timing system. It connects directly to the Raspberry Pi Pico over USB using the Web Serial API and displays timing and sensor data in real time. The page allows the user to start a connection, view live beam-break events, and monitor system status without requiring any additional software or servers.

Incoming data from the Pico is read continuously and displayed immediately, allowing timing results to be seen with minimal latency. Visual indicators on the page update dynamically to reflect the current state of the laser and sensor, making it easy to confirm correct operation at a glance.

The page runs entirely in the browser using HTML, CSS, and JavaScript. All processing, display updates, and data handling occur client-side, keeping the system simple and portable. This design allows the timing system to be demonstrated on any compatible computer by simply opening the page and connecting the Pico via USB.

