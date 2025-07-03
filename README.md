# Embedded-System-for-a-Particle-Collider

## Overview
This project is a simulation of a particle collider, inspired by the CERN Large Hadron Collider (LHC). The purpose is to emulate, through sensors and embedded systems, the process of particle acceleration, collision, and result analysis.

## What is a Particle Collider?
A particle collider is a device that accelerates beams of particles in opposite directions using electric and magnetic fields. When they collide, large amounts of energy are released, enabling physicists to test theories and discover new particles, such as the Higgs boson.

## Project Structure
### Simulation Steps:
- Detection of particles ready to be accelerated.
- Entry into the pre-accelerators. (Energy potentiometer ≥ 60%)
- When reaching target energy, particles enter the LHC. (Energy potentiometer ≥ 80%)
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

# Portuguese Version
## Visão Geral
Este projeto é uma simulação de um colisor de partículas, inspirado no Grande Colisor de Hádrons (LHC) do CERN. O objetivo é emular, por meio de sensores e sistemas embarcados, o processo de aceleração de partículas, colisão e análise de resultados.

## O que é um Colisor de Partículas?
Um colisor de partículas é um dispositivo que acelera feixes de partículas em direções opostas utilizando campos elétricos e magnéticos. Quando esses feixes colidem, grandes quantidades de energia são liberadas, permitindo que os físicos testem teorias e descubram novas partículas, como o bóson de Higgs.

## Estrutura do Projeto
### Etapas da Simulação:
- Detecção de partículas prontas para serem aceleradas.
- Entrada nos pré-aceleradores. (Potenciômetro da energia ≥ 60%)
- Ao atingir a energia-alvo, as partículas entram no LHC. (Potenciômetro da energia ≥ 80%)
- Colisão e exibição dos resultados.

### Entradas:
- Sensor de Detecção do Feixe de Partículas 1  
- Sensor de Detecção do Feixe de Partículas 2  
- Botão de Emergência  
- Sensor de Energia (analógico)  
- Sensor de Radiação (analógico)  
- Sensor de Temperatura (analógico)  

### Saídas:
- Display Gráfico LCD  
- LED do Pré-Acelerador  
- LED do LHC  
- LED de Colisão  
- LED de Emergência  

## Resultados da Colisão

Dependendo dos valores dos sensores de temperatura e radiação, um dos seguintes resultados é exibido:

- **Bóson de Higgs Detectado**
  - Temperatura ≥ 120  
  - Radiação ≥ 40%

- **Partícula Leve Detectada**
  - Temperatura < 120  
  - Radiação < 40%

- **Evento Não Identificado**
  - As condições não correspondem a nenhuma das anteriores
