# Ultrasonic Object Detection and Distance Measurement System using HC-SR04, LPC1768, and 16x2 LCD

A real-time ultrasonic object detection system developed in Embedded C for the LPC1768 microcontroller using Keil μVision. It uses the HC-SR04 sensor to measure distance, displays the result on a 16x2 LCD, and activates an LED when an object is detected within a 30 cm threshold—ideal for basic proximity alert applications in embedded systems. 

---

### Components

- LPC1768 Cortex-M3 Development Board (with LCD and LEDs)
- HC-SR04 Ultrasonic Sensor
- Jumper wires
- USB Cable

---

### Pin Configuration

| Component   | LPC1768 Pin |
|-------------|-------------|
| Trigger     | P0.15       |
| Echo        | P0.16       |
| LED         | P0.8        |
| LCD RS      | P0.27       |
| LCD EN      | P0.28       |
| LCD D4-D7   | P0.23-P0.26 |

---

### Setup
1. Connect components as per pin configuration
2. Power system with 3.3V supply
3. Connect LCD contrast potentiometer
4. Ensure proper ground connections

---

### Usage
1. System continuously displays distance on LCD
2. LED illuminates when object is within 30cm
3. Measurement updates every 300ms

---

### Dependencies
- CMSIS library for LPC1768
- Standard C library for printf functions
---

### Notes
- Adjust distance threshold by modifying `distance < 30` in main loop
- Calibrate speed of sound constant (343 m/s) for different environments
- Ensure sensor has clear line-of-sight for accurate measurements
