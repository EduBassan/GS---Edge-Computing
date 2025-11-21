#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>

// ======================= CONFIG ==========================
const char* ssid = "Wokwi-GUEST";
const char* password = "";
const char* mqtt_server = "test.mosquitto.org";

String deviceId = "esp32_ambiente_001";  // identificação do dispositivo

const int DHT_PIN = 15;
const int LED_PIN = 2;
const int BUZZ_PIN = 4;

unsigned long lastPublish = 0;
const unsigned long publishInterval = 4000; // 4 segundos
// ==========================================================

WiFiClient espClient;
PubSubClient client(espClient);
DHTesp dht;

// ================= FUNÇÃO WIFI ===========================
void setup_wifi() {
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nWiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

// =============== MQTT CALLBACK ============================
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("\n[Comando recebido] ");
  Serial.print(topic);
  Serial.print(": ");

  String msg = "";
  for (int i = 0; i < length; i++) msg += (char)payload[i];
  Serial.println(msg);

  // Comandos simples vindos do Node-RED
  if (msg == "led_on") digitalWrite(LED_PIN, HIGH);
  if (msg == "led_off") digitalWrite(LED_PIN, LOW);
  if (msg == "buzzer") tone(BUZZ_PIN, 1000, 300);
}

// =============== RECONNECT MQTT ============================
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao broker... ");

    if (client.connect(deviceId.c_str())) {
      Serial.println("Conectado!");
      client.subscribe(("globalsolution/" + deviceId + "/cmd").c_str());
    } else {
      Serial.print("Falhou, código = ");
      Serial.println(client.state());
      delay(2000);
    }
  }
}

// ================ HEAT INDEX (Índice de Calor) ===============
float calcHeatIndex(float temp, float hum) {
  return -8.784695 + 
         1.61139411 * temp + 
         2.338549 * hum -
         0.14611605 * temp * hum -
         0.01230809 * pow(temp, 2) -
         0.01642482 * pow(hum, 2) +
         0.00221173 * pow(temp, 2) * hum +
         0.00072546 * temp * pow(hum, 2) -
         0.00000358 * pow(temp, 2) * pow(hum, 2);
}

// ====================== SETUP ===============================
void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZ_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  digitalWrite(BUZZ_PIN, LOW);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  Serial.println("Sistema Ambiente Saudável iniciado!");
}

// ======================= LOOP ==============================
void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastPublish >= publishInterval) {
    lastPublish = now;

    TempAndHumidity th = dht.getTempAndHumidity();
    float temp = th.temperature;
    float hum = th.humidity;
    float heatIndex = calcHeatIndex(temp, hum);

    // Classificação simples de conforto térmico
    String status = "OK";
    if (heatIndex > 27 && heatIndex <= 32) status = "moderado";
    else if (heatIndex > 32) status = "critico";

    // Alerta automático
    if (status == "moderado") {
      digitalWrite(LED_PIN, HIGH);
      noTone(BUZZ_PIN);
    } else if (status == "critico") {
      digitalWrite(LED_PIN, HIGH);
      tone(BUZZ_PIN, 1000);
    } else {
      digitalWrite(LED_PIN, LOW);
      noTone(BUZZ_PIN);
    }

    // ====== JSON COMPLETO ======
    String json = "{";
    json += "\"device\":\"" + deviceId + "\",";
    json += "\"temp\":" + String(temp, 1) + ",";
    json += "\"hum\":" + String(hum, 1) + ",";
    json += "\"heatIndex\":" + String(heatIndex, 1) + ",";
    json += "\"status\":\"" + status + "\"";
    json += "}";

    client.publish(("globalsolution/" + deviceId + "/telemetry").c_str(), json.c_str());

    Serial.println(json);
  }
}
