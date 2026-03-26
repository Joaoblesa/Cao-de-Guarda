#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


const char* ssid = "Brz";
const char* password = "bree2769";


const char* serverUrl = "https://jsonplaceholder.typicode.com/todos/1";

const int PIN_LED_VERDE = 5;
const int PIN_LED_VERMELHO = 18;


unsigned long ultimaChecagem = 0;
const long intervalo = 15000;

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_VERMELHO, OUTPUT);

  digitalWrite(PIN_LED_VERMELHO, HIGH);
  digitalWrite(PIN_LED_VERDE, LOW);

  WiFi.begin(ssid, password);
  Serial.println("Iniciando Watchdog TechMec...");
}

void loop() {

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi Desconectado! Tentando reconectar...");
    digitalWrite(PIN_LED_VERDE, LOW);
    digitalWrite(PIN_LED_VERMELHO, HIGH);
    

    WiFi.begin(ssid, password);
    delay(2000); 
    return; 
  }


  if (millis() - ultimaChecagem >= intervalo) {
    ultimaChecagem = millis();
    verificarServidor();
  }
}

void verificarServidor() {

  WiFiClientSecure *client = new WiFiClientSecure;
  
  if(client) {
  
    client->setInsecure(); 

    HTTPClient http;
    Serial.print("[HTTP] Conectando ao servidor...");

    if (http.begin(*client, serverUrl)) {
      int httpCode = http.GET();

      if (httpCode == HTTP_CODE_OK) {
        Serial.printf(" SUCESSO! Código: %d\n", httpCode);
        digitalWrite(PIN_LED_VERDE, HIGH);
        digitalWrite(PIN_LED_VERMELHO, LOW);
      } else {
        Serial.printf(" ERRO NO SERVIDOR: %d\n", httpCode);
        digitalWrite(PIN_LED_VERDE, LOW);
        digitalWrite(PIN_LED_VERMELHO, HIGH);
      }
      http.end(); 
    } else {
      Serial.println(" Falha na conexão HTTP.");
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_VERMELHO, HIGH);
    }
    
    
    delete client; 
  }
}