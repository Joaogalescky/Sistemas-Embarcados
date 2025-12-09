// Wi-Fi
#include <WiFi.h>
#include <WiFiClientSecure.h>
// Bot do Telegram
#include <UniversalTelegramBot.h>
#include "Adafruit_LEDBackpack.h"
#include <TelegramCertificate.h>

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
#define CHAT_ID "SEU_CHAT_ID"

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

const unsigned long BOT_MTBS = 1000; // intervalo entre a verificação de novas mensagens
unsigned long bot_lasttime = 0;      // ultimo intervalo de leitura feita

// Gerenciamento de IDs para notificação
#define MAX_CHATS 5
String validChatIds[MAX_CHATS];
int numValidChats = 0;

// Pinos
#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_LED_B 12
#define PIN_POT A0

// Pinos PWM ESP32 (0 a 15)
#define PWM_CHANNEL 0
#define PWM_FREQ 5000
#define PWM_RES 12

Adafruit_7segment dis = Adafruit_7segment();

// Controle de tempo
unsigned long previousTime = 0;
unsigned long interval = 5000;

// Sensor do potenciometro
int sensorValue = 0;
float outputValue = 0;

bool isChatIdValid(String chatId)
{
  for (int i = 0; i < numValidChats; i++)
  {
    if (validChatIds[i] == chatId)
    {
      return true;
    }
  }
  return false;
}

bool addChatId(String chatId)
{
  if (numValidChats < MAX_CHATS && !isChatIdValid(chatId))
  {
    validChatIds[numValidChats++] = chatId;
    return true;
  }
  return false;
}

bool removeChatId(String chatId)
{
  for (int i = 0; i < numValidChats; i++)
  {
    if (validChatIds[i] == chatId)
    {
      // Mover proximos elementos para a posição
      for (int j = i; j < numValidChats - 1; j++)
      {
        validChatIds[j] = validChatIds[j + 1];
      }
      validChatIds[numValidChats - 1] = "";
      numValidChats--;
      return true;
    }
  }
  return false;
}

void sendNotification(String message)
{
  for (int i = 0; i < numValidChats; i++)
  {
    bot.sendMessage(validChatIds[i], message, "");
  }
}

void handleNewMessage(int numMessages)
{
  Serial.print("Total de novas mensagens: ");
  Serial.println(numMessages);

  for (int i = 0; i < numMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    String from_name = bot.messages[i].from_name;
    if (from_name == "")
      from_name = "Guest";

    Serial.print("Mensagem recebida de ");
    Serial.print(from_name);
    Serial.print(" (ID: ");
    Serial.print(chat_id);
    Serial.print("): ");
    Serial.println(text);

    if (text.equalsIgnoreCase("/addid"))
    {
      if (addChatId(chat_id))
      {
        bot.sendMessage(chat_id, "SUCESSO: ID adicionado para notificações!", "");
      }
      else if (isChatIdValid(chat_id))
      {
        bot.sendMessage(chat_id, "ATENÇÃO! ID já está na lista.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "ERRO: Não foi possível adicionar o ID. Lista cheia.", "");
      }
    }
    else if (text.equalsIgnoreCase("/removeid"))
    {
      if (removeChatId(chat_id))
      {
        bot.sendMessage(chat_id, "SUCESSO: ID removido da lista de notificações.", "");
      }
      else
      {
        bot.sendMessage(chat_id, "ATENÇÃO! Seu ID não está na lista para ser removido.", "");
      }
    }

    else if (text.equalsIgnoreCase("/info"))
    {
      String infoMsg = "--- Status do Feeder ---\n";
      infoMsg += "Leitura do Sensor (0-1023): " + String(sensorValue) + "\n";
      infoMsg += "Nível Mapeado (0-2000): " + String(outputValue) + "\n";
      infoMsg += "IDs Registrados: " + String(numValidChats) + "/" + String(MAX_CHATS) + "\n";
      bot.sendMessage(chat_id, infoMsg, "");
    }

    else if (text == "/start")
    {
      String welcome = "Bem-vindo ao FeederBot, " + from_name + ".\n\n";
      welcome += "Use os comandos:\n";
      welcome += "/addid - Receber notificações do status.\n";
      welcome += "/removeid - Parar de receber notificações.\n";
      welcome += "/info - Ver o status atual do alimentador.\n";
      welcome += "/options - Exibir teclado inline de exemplo.\n";

      bot.sendMessage(chat_id, welcome, "Markdown");
    }
    else if (text == "/options")
    {
      String keyboardJson = "[[{ \"text\" : \"Google\", \"url\" : \"https://www.google.com\" }],[{ \"text\" : \"Callback Teste\", \"callback_data\" : \"inline_data_exemplo\" }]]";
      bot.sendMessageWithInlineKeyboard(chat_id, "Escolha uma opção:", "", keyboardJson);
    }
  }
}

void telegramLoop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numMessages)
    {
      Serial.println("Recebeu resposta");
      handleNewMessage(numMessages);
      numMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}

void setupFeeder()
{
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
  dis.begin(0x70);
  dis.setBrightness(0);
  ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RES);
  ledcAttachPin(PIN_LED_R, PWM_CHANNEL);
  Serial.println();
}

void feederEsp32()
{
  unsigned long currentTime = millis();

  sensorValue = analogRead(PIN_POT);
  outputValue = map(sensorValue, 0, 4095, 0, 4095); // Adaptar conforme o modelo do ESP32
  ledcWrite(PWM_CHANNEL, outputValue);

  if (currentTime - previousTime >= interval)
  {
    previousTime = currentTime;

    if (outputValue > 3000)
    {
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    }
    else if (outputValue > 1500)
    {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    }
    else
    {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      sendNotification("ATENÇÃO! Nível do alimentador baixo: " + String(outputValue));
    }

    dis.println((int)outputValue);
    dis.writeDisplay();
  }
}

void setupTelegram()
{
  // Telegram
  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

// task que será alocada no core 0
void taskTelegram(void *pvParameters)
{
  delay(100);
  Serial.print("TaskTelegram running on core ");
  Serial.println(xPortGetCoreID());

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("\nWifi status: OK");

      for (;;)
  {
    telegramLoop();
    vTaskDelay(1200 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  setupFeeder();
  setupTelegram();
  xTaskCreatePinnedToCore(
      taskTelegram,
      "TelegramTask",
      10000,
      NULL,
      1,
      NULL,
      0);
}

void loop()
{
  feederEsp32();
}
