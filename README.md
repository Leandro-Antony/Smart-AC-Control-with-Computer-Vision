# Sistema Inteligente de Climatização com Visão Computacional
Este repositório contém o desenvolvimento de um sistema inteligente de climatização baseado em **ESP32**, **ESP32‑CAM**, sensores de temperatura e um modelo de **visão computacional treinado no Edge Impulse**.  
O objetivo é automatizar o uso do ar-condicionado a partir da **temperatura do ambiente** e da **presença de pessoas**, contribuindo para eficiência energética.

---

## Funcionalidades Principais
- Monitoramento contínuo da temperatura ambiente (DHT11)  
- Detecção de presença em tempo real com ESP32CAM  
- Comunicação serial entre ESP32 ↔ ESP32CAM  
- Controle automático de ar-condicionado via emissor IR  
- Integração completa entre sensores, IA embarcada e lógica de climatização  

---

## Tecnologias Utilizadas
- ESP32 (controle principal)  
- ESP32CAM (visão computacional)  
- Sensor DHT11  
- Edge Impulse (treinamento do modelo FOMO MobileNetV2 0.35)  
- Módulos IR (KY‑022 e KY‑005)  
- Arduino Framework  

## Autores
- Leandro Antony Batista Lemos
- Davi Felipe de Assis Borges
