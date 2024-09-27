/* Programa para ESP32 conectar a uma rede WIFI
//editado: o programa so informa data e hora quando o botao do esp é pressionado

* Autor: Michael Martins
* Data: 30/07/2024

* Descrição: Programa para ESP32 conectar a uma rede WIFI e mostrar data e hora ao pressionar o botão BOOT.
*/

// Bibliotecas
#include <Arduino.h>
#include <WiFi.h>
#include "senhas.h"
#include <time.h>

// Configuração do servidor NTP
const char* ntpserver = "pool.ntp.org";
const long  gmtOffset_sec = (-3 * 60 * 60); // fuso horário de -3 horas em segundos
const int   daylightOffset_sec = 0;

// Pino do botão BOOT (GPIO 0)
#define botaoBoot 0

// Função para verificar se o botão BOOT foi pressionado
bool botaoBootPressed() {
  return digitalRead(botaoBoot) == LOW;
}

// Função para pegar a hora 
void horaLocal();

void setup() 
{
  Serial.begin(9600);

  // Configurar o pino do botão BOOT como entrada
  pinMode(botaoBoot, INPUT_PULLUP);

  // Conectar à rede WIFI
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado à rede ");
  Serial.print(ssid);
  Serial.println(" com sucesso!");

  // Configurar a hora via NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpserver);
}

void loop() 
{
  // Verifica se o botão BOOT foi pressionado (estado LOW)
  if (botaoBootPressed())  
  {
    horaLocal(); // Exibe a hora no monitor serial quando o botão for pressionado
    while (botaoBootPressed()) {
      // Aguarda o botão ser solto para evitar leituras repetidas
      delay(10);
    }
  }
  delay(100); // Pequeno delay para evitar leituras múltiplas durante a mesma pressão
}

// Função para pegar a hora
void horaLocal()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Falha ao obter o horário");
    return;
  }

  int ano = timeinfo.tm_year + 1900;
  int mes = timeinfo.tm_mon + 1;
  int dia = timeinfo.tm_mday;
  int hora = timeinfo.tm_hour;
  int min = timeinfo.tm_min;
  int sec = timeinfo.tm_sec;

  Serial.printf("Data: %02d/%02d/%d - Hora: %02d:%02d:%02d\n", dia, mes, ano, hora, min, sec);
}
