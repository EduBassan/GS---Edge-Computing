# 🌡️ Sistema Ambiente Saudável – IoT + ESP32 + MQTT + Dashboard Web

## 📌 Visão Geral

Este projeto implementa um **sistema inteligente de monitoramento ambiental**, capaz de medir **temperatura**, **umidade** e **índice de calor**, além de emitir alertas automáticos usando **LED** e **buzzer**.
Os dados são enviados via **MQTT** para um **dashboard web em tempo real**, estilizado com **TailwindCSS**, contendo gráficos, indicadores e status de conforto térmico.

O sistema foi desenvolvido com foco em **IoT**, **eficiência**, **monitoramento remoto** e **experiência visual minimalista e profissional**.

---

## 🚀 Funcionalidades

### 🔹 ESP32

* Leitura de **temperatura** e **umidade** via sensor **DHT22**
* Cálculo do **Índice de Calor (Heat Index)**
* Classificação automática:

  * 🟢 **OK**
  * 🟡 **Moderado**
  * 🔴 **Crítico**
* Ações automáticas:

  * LED acende em moderado e crítico
  * Buzzer toca quando crítico
* Envio periódico de JSON via MQTT
* Recebimento de comandos do dashboard:

  * `led_on`
  * `led_off`
  * `buzzer`

---

### 🔹 Dashboard Web (index.html)

* Leitura **em tempo real** dos dados MQTT
* Cartões modernos e minimalistas com:

  * Temperatura
  * Umidade
  * Índice de calor
  * Status ambiental
* Gráfico histórico com atualização automática
* Botões de controle:

  * Ligar/desligar LED
  * Acionar buzzer
* Ideal para computador
* Estilização com **TailwindCSS**
* Animações suaves e design clean

---

## 🧩 Arquitetura do Sistema

```
ESP32 (DHT22) → MQTT → Dashboard Web (index.html)
```

Broker utilizado:

```
broker: test.mosquitto.org
porta: 8081 (WebSocket)
porta: 1883 (ESP32)
```

---

## 📡 Tópicos MQTT Utilizados

### 📤 Publicação (ESP32 → Dashboard)

| Tópico                                        | Descrição                                                     |
| --------------------------------------------- | ------------------------------------------------------------- |
| `globalsolution/esp32_ambiente_001/telemetry` | Envia JSON com temperatura, umidade, índice de calor e status |

Exemplo de payload:

```json
{
  "device": "esp32_ambiente_001",
  "temp": 27.5,
  "hum": 61.2,
  "heatIndex": 28.3,
  "status": "moderado"
}
```

---

### 📥 Assinatura (Dashboard → ESP32)

| Tópico                                  | Comando                                                  |
| --------------------------------------- | -------------------------------------------------------- |
| `globalsolution/esp32_ambiente_001/cmd` | Recebe comandos como `"led_on"`, `"led_off"`, `"buzzer"` |

---

## 🧪 Links Importantes

🔗 **Wokwi ESP32 Simulação:**
https://wokwi.com/projects/448196228367321089

🔗 **Vídeo Explicativo:**
https://youtu.be/OiO5KAbOKss

---

## 🛠️ Como Executar o Projeto

### 1️⃣ Baixar este repositório

```
git clone https://github.com/seuusuario/ambiente-saudavel.git
```

### 2️⃣ Abrir o projeto no Wokwi

* Cole o código `.ino` na plataforma
* Clique em “Start Simulation”

### 3️⃣ Abrir o Dashboard

* Abra o arquivo `index.html` no seu navegador

⚠️ O dashboard só funciona se a simulação estiver rodando no Wokwi.

---

## 🧠 Explicação Técnica (MQTT)

O projeto usa o protocolo MQTT por ser leve e ideal para IoT.
Principais características:

* Transporte **publish/subscribe**
* Baixa latência
* Conexão contínua
* Suporte a JSON
* Broker público gratuito (Mosquitto)

O ESP32 usa a biblioteca:

```cpp
#include <PubSubClient.h>
```

O dashboard usa:

```html
<script src="https://unpkg.com/mqtt/dist/mqtt.min.js"></script>
```

A comunicação WebSocket ocorre via porta **8081**:

```js
mqtt.connect("wss://test.mosquitto.org:8081/mqtt")
```

---

## 👨‍💻 Integrantes do Projeto

* **Eduardo Santiago Bassan | RM: 561474**
* **Vitor Fernandes dos Santos | RM: 566275**

---

## ✔️ Conclusão

Este projeto entrega:

* Monitoramento ambiental inteligente
* Interface web profissional
* Comunicação IoT via MQTT
* Acionamento remoto de dispositivos
* Código claro, modular e comentado

