# Embedded-System-for-a-Particle-Collider

## Overview
This project is a simulation of a particle collider, inspired by the CERN Large Hadron Collider (LHC). The purpose is to emulate, through sensors and embedded systems, the process of particle acceleration, collision, and result analysis.

## What is a Particle Collider?
A particle collider is a device that accelerates beams of particles in opposite directions using electric and magnetic fields. When they collide, large amounts of energy are released, enabling physicists to test theories and discover new particles, such as the Higgs boson.

## Project Structure
### Simulation Steps:
- Detection of particles ready to be accelerated.
- Entry into pre-accelerators.
- When reaching target energy, particles enter the LHC.
- Collision and display of results.

### Inputs:
- Particle Beam 1 Detection Sensor
- Particle Beam 2 Detection Sensor
- Emergency Button
- Energy Sensor (analog)
- Radiation Sensor (analog)
- Temperature Sensor (analog)

### Outputs:
- LCD Graphic Display
- Pre-Accelerator LED
- LHC LED
- Collision LED
- Emergency LED

## Collision Results

Depending on the values of the temperature and radiation sensors, one of the following outcomes is displayed:

- **Higgs Boson Detected**
  - Temperature ≥ 120
  - Radiation ≥ 40%

- **Light Particle Detected**
  - Temperature < 120
  - Radiation < 40%

- **Unidentified Event**
  - Conditions do not match either of the above

