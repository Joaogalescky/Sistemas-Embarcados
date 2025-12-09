// Wi-Fi
#include <WiFi.h>
#include <WiFiClientSecure.h>
#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"
WiFiClientSecure secured_client;

// Bot do Telegram
#include <UniversalTelegramBot.h>
#include "Adafruit_LEDBackpack.h"
#define BOT_TOKEN "XXXXXXXXX:XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"
const unsigned long BOT_MTBS = 1000;  // intervalo entre a verificação de novas mensagens
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime;  // last time messages' scan has been done

// Gerenciamento de IDs para notificação
#define MAX_CHATS 5
String validChatIds[MAX_CHATS];
int numValidChats = 0;

// Pinos
#define PIN_LED_R 13
#define PIN_LED_G 11
#define PIN_LED_B 12
#define PIN_POT A0
// #define PIN_SDA A4
// #define PIN_SCL A5

// Controle de tempo anterior e atual
unsigned long previousTime = 0;
unsigned long interval = 5000;

// Sensor do potenciometro
int sensorValue = 0;
float outputValue = 0;

Adafruit_7segment dis = Adafruit_7segment();

bool isChatIdValid(String chatId) {
  for (int i = 0; i < numValidChats; i ++) {
    if (validChatIds[i] == chatId) {
      return true;
    }
  }
  return false;
}

bool addChatId(String chatId) {
  if (numValidChats < MAX_CHATS && !isChatIdValid(chatId)) {
    validChatIds[numValidChats] = chatId;
    numValidChats++;
    return true;
  }
  return false;
}

bool removeChatId(String chatId) {
  for (int i = 0; i < numValidChats; i++) {
    if (validChatIds[i] == chatId) {
      // Mover proximos elementos para a posição
      for (int j = i; j < numValidChats - 1; j++) {
        validChatIds[j] = validChatIds[j + 1];
      }
      validChatIds[numValidChats - 1] = "";
      numValidChats--;
      return true;
    }
  }
  return false;
}

void sendNotification(String message) {
    for (int i = 0; i < numValidChats; i++) {
        bot.sendMessage(validChatIds[i], message, "");
    }
}

void handleNewMessage(int numMessages) {
Serial.print("Total de novas mensagens: ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++) {
    // Inline buttons with callbacks when pressed will raise a callback_query message
    if (bot.messages[i].type == "callback_query") {
      Serial.print("Callback button pressed by: ");
      Serial.println(bot.messages[i].from_id);
      Serial.print("Data: ");
      Serial.println(bot.messages[i].text);
      bot.sendMessage(bot.messages[i].from_id, "Você pressionou: " + bot.messages[i].text, "");
    } else {
      String chat_id = bot.messages[i].chat_id;
      String text = bot.messages[i].text;
      String from_name = bot.messages[i].from_name;
      if (from_name == "") from_name = "Guest";

      Serial.print("Mensagem recebida de ");
      Serial.print(from_name);
      Serial.print(" (ID: ");
      Serial.print(chat_id);
      Serial.print("): ");
      Serial.println(text);

      if (text.equalsIgnoreCase("/addid")) {
        if (addChatId(chat_id)) {
          bot.sendMessage(chat_id, "SUCESSO: ID adicionado para notificações!", "");
        } else if (isChatIdValid(chat_id)) {
          bot.sendMessage(chat_id, "ATENÇÃO! ID já está na lista.", "");
        } else {
          bot.sendMessage(chat_id, "ERRO: Não foi possível adicionar o ID. Lista cheia.", "");
        }
      } else if (text.equalsIgnoreCase("/removeid")) {
        if (removeChatId(chat_id)) {
          bot.sendMessage(chat_id, "SUCESSO: ID removido da lista de notificações.", "");
        } else {
          bot.sendMessage(chat_id, "ATENÇÃO! Seu ID não está na lista para ser removido.", "");
        }
      }

      else if (text.equalsIgnoreCase("/info")) {
        String infoMsg = "--- Status do Feeder ---\n";
        infoMsg += "Leitura do Sensor (0-1023): " + String(sensorValue) + "\n";
        infoMsg += "Nível Mapeado (0-2000): " + String(outputValue) + "\n";
        infoMsg += "IDs Registrados: " + String(numValidChats) + "/" + String(MAX_CHATS) + "\n";
        bot.sendMessage(chat_id, infoMsg, "");
      }

      else if (text == "/start") {
        String welcome = "Bem-vindo ao FeederBot, " + from_name + ".\n\n";
        welcome += "Use os comandos:\n";
        welcome += "/addid - Receber notificações do status.\n";
        welcome += "/removeid - Parar de receber notificações.\n";
        welcome += "/info - Ver o status atual do alimentador.\n";
        welcome += "/options - Exibir teclado inline de exemplo.\n";

        bot.sendMessage(chat_id, welcome, "Markdown");
      } else if (text == "/options") {
        String keyboardJson = "[[{ \"text\" : \"Google\", \"url\" : \"https://www.google.com\" }],[{ \"text\" : \"Callback Teste\", \"callback_data\" : \"inline_data_exemplo\" }]]";
        bot.sendMessageWithInlineKeyboard(chat_id, "Escolha uma opção:", "", keyboardJson);
      }
    }
  }
}

void telegramLoop() {
  if (millis() - bot_lasttime > BOT_MTBS) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}

void setupFeeder() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
  dis.begin(0x70);
  dis.setBrightness(0);
  Serial.println();
}

void feederEsp32() {
  unsigned long currentTime = millis();

  sensorValue = analogRead(PIN_POT);
  outputValue = map(sensorValue, 0, 1023, 0, 2000);
  analogWrite(sensorValue, outputValue);

  if (currentTime - previousTime >= interval) {
    previousTime = currentTime;

    if (outputValue > 1000) {
      digitalWrite(PIN_LED_R, LOW);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    } else if (outputValue > 500) {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, HIGH);
      digitalWrite(PIN_LED_B, LOW);
    } else {
      digitalWrite(PIN_LED_R, HIGH);
      digitalWrite(PIN_LED_G, LOW);
      digitalWrite(PIN_LED_B, LOW);
      sendNotification("ATENÇÃO! Nível do alimentador baixo: ", String(outputValue));
    }

    dis.println((int)outputValue);
    dis.writeDisplay();
  }
}

void setupTelegram() {
  // Telegram
  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org");  // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600) {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);

  bot.sendMessage(CHAT_ID, "Bot started up", "");
}

void initWIFI() {
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);  // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println();
}

// task que será alocada no core 0
void taskTelegram(void* pvParameters) {
  delay(10);
  Serial.print("TaskTelegram running on core ");
  Serial.println(xPortGetCoreID());
  // Telgram bot
  myBot.wifiConnect(WIFI_SSID, WIFI_PASSWORD);
  myBot.setTelegramToken(BOT_TOKEN);
  // check if all things are ok
  if (myBot.testConnection()) {
    Serial.println("\ntestConnection OK");
  } else {
    Serial.println("\ntestConnection NOK");
  }
  // Create keyboards
  createKeyboards();
  isKeyboardActive = false;
  for (;;) {
    loopTelegram();
    vTaskDelay(1200 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  setupFeeder();
  setupTelegram();
  initWIFI();
}

void loop() {
  feederEsp32();
  telegramLoop();
}
