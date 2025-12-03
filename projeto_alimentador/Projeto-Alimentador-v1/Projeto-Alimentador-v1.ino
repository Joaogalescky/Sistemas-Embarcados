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
        validChatIds[j] = validChatIds
      }
    }
  }
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

// rotina de verificação e tratamento das mensagens
void loopTelegram() {
  // a variable to store telegram message data
  TBMessage msg;

  // if there is an incoming message...
  if (myBot.getNewMessage(msg)) {
    long telegramDelay = millis();
    // check what kind of message I received
    if (msg.messageType == CTBotMessageText) {
      Serial.print("\nMensagem: ");
      Serial.println(msg.text);
      Serial.println();

      // received a text message
      if (msg.text.equalsIgnoreCase("show keyboard")) {
        // the user is asking to show the reply keyboard --> show it
        myBot.sendMessage(msg.sender.id, "Reply Keyboard enable. You can send a simple text, your contact, your location or hide the keyboard", myKbd);
        isKeyboardActive = true;
      } else if (msg.text.equalsIgnoreCase("Add Id")) {
        bool result = idsList.isListed(msg.sender.id);
        if (result) {
          myBot.sendMessage(msg.sender.id, "ID já foi adicionado!");
        } else {
          bool result = idsList.addId(msg.sender.id);
          if (result) {
            myBot.sendMessage(msg.sender.id, "ID adicionado!");
          } else {
            myBot.sendMessage(msg.sender.id, "Não foi possível adicionar!");
          }
        }
      } else if (msg.text.equalsIgnoreCase("Remove Id")) {
        bool result = idsList.isListed(msg.sender.id);
        if (result) {
          bool result = idsList.removeId(msg.sender.id);
          if (result) {
            myBot.sendMessage(msg.sender.id, "ID removido!");
          } else {
            myBot.sendMessage(msg.sender.id, "Não foi possível remover!");
          }
        } else {
          myBot.sendMessage(msg.sender.id, "ID não listado!");
        }
      } else if (msg.text.equalsIgnoreCase("Info!")) {
        myBot.sendMessage(msg.sender.id, temp);
      }


      // check if the reply keyboard is active
      else if (isKeyboardActive) {
        // is active -> manage the text messages sent by pressing the reply keyboard buttons
        if (msg.text.equalsIgnoreCase("Hide replyKeyboard")) {
          // sent the "hide keyboard" message --> hide the reply keyboard
          myBot.removeReplyKeyboard(msg.sender.id, "Reply keyboard removed");
          isKeyboardActive = false;
        } else {
          // print every others messages received
          myBot.sendMessage(msg.sender.id, msg.text);
        }
      } else {
        // the user write anything else and the reply keyboard is not active --> show a hint message
        myBot.sendMessage(msg.sender.id, "Try 'show keyboard'");
        Serial.print("Id:");
        Serial.println(msg.sender.id);
      }
    }
    Serial.print("Telegram delay: ");
    Serial.println(millis() - telegramDelay);
  }
}

void createKeyboards() {
  myKbd.addButton("Info!");
  myKbd.addRow();
  myKbd.addButton("Add Id");
  myKbd.addButton("Remove Id");
  myKbd.addRow();
  myKbd.addButton("Hide replyKeyboard");
  myKbd.enableResize();
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

void handleNewMessages(int numNewMessages) {
  for (int i = 0; i < numNewMessages; i++) {
    // Inline buttons with callbacks when pressed will raise a callback_query message
    if (bot.messages[i].type == "callback_query") {
      Serial.print("Call back button pressed by: ");
      Serial.println(bot.messages[i].from_id);
      Serial.print("Data on the button: ");
      Serial.println(bot.messages[i].text);
      bot.sendMessage(bot.messages[i].from_id, bot.messages[i].text, "");
    } else {
      String chat_id = bot.messages[i].chat_id;
      String text = bot.messages[i].text;

      String from_name = bot.messages[i].from_name;
      if (from_name == "")
        from_name = "Guest";

      if (text == "/options") {
        String keyboardJson = "[[{ \"text\" : \"Go to Google\", \"url\" : \"https://www.google.com\" }],[{ \"text\" : \"Send\", \"callback_data\" : \"This was sent by inline\" }]]";
        bot.sendMessageWithInlineKeyboard(chat_id, "Choose from one of the following options", "", keyboardJson);
      }

      if (text == "/start") {
        String welcome = "Welcome to Universal Arduino Telegram Bot library, " + from_name + ".\n";
        welcome += "This is Inline Keyboard Markup example.\n\n";
        welcome += "/options : returns the inline keyboard\n";

        bot.sendMessage(chat_id, welcome, "Markdown");
      }
    }
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

void setupFeeder() {
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G, OUTPUT);
  pinMode(PIN_LED_B, OUTPUT);
  digitalWrite(PIN_LED_R, LOW);
  digitalWrite(PIN_LED_G, LOW);
  digitalWrite(PIN_LED_B, LOW);
  dis.begin(0x70);
  Serial.println();
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
    }

    dis.println(outputValue);
    dis.writeDisplay();
  }
}
