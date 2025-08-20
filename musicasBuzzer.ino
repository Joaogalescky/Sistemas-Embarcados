
//https://github.com/ArminJo/PlayRtttl
//https://picaxe.com/RTTTL-Ringtones-for-Tune-Command/
//https://github.com/granadaxronos/120-SONG_NOKIA_RTTTL_RINGTONE_PLAYER_FOR_ARDUINO_UNO/blob/master/RTTTL_PLAYER/songs.h
//https://ringtone.vulc.in/
//http://arcadetones.emuunlim.com/pc.htm

#include <Arduino.h>
#include <PlayRtttl.hpp>


const int TONE_PIN = 11;

char InRam[] =
        // "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
        // "Walk of Life:d=4,o=5,b=160:b.,b.,p,8p,8f#,8g,b,8g,8f,e.,e.,p,2p,p,8f,8g,b.,b.,p,8p,8f,8g,b,8g,f,e.,e.,p,8p,8f,8g,b,8g,8f,8e";
        "Doom Level 1:d=32,o=5,b=56:f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,f,f,f6,f,f,d#6,f,f,c#6,f,f,b,f,f,c6,c#6,f,f,f6,f,f,d#6,f,f,c#6,f,f,8b.,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,e6,a#,a#,f6,f#6,a#,a#,a#6,a#,a#,g#6,a#,a#,f#6,a#,a#,8e6";


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!


    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_PLAY_RTTTL));

    // extrair nome até o primeiro ':'
    char musicName[32];  // buffer para o nome
    char *pos = strchr(InRam, ':'); // procura o primeiro ':'
    if (pos != NULL) {
        size_t len = pos - InRam;    // tamanho do nome
        if (len >= sizeof(musicName)) len = sizeof(musicName) - 1;
        strncpy(musicName, InRam, len);
        musicName[len] = '\0';  // termina string
    } else {
        strcpy(musicName, "Desconhecido");
    }

    Serial.print(F("Play "));
    Serial.println(musicName);
   
    playRtttlBlocking(TONE_PIN, InRam);
    delay(10000);
}

void loop() {
    
    const char * const*tSongPtr;   

    for (uint_fast8_t i = 0; i < ARRAY_SIZE_MELODIES; ++i) {
        tSongPtr = &RTTTLMelodies[i];

        printNamePGMPGM(tSongPtr, &Serial);
        Serial.print(F("Index="));
        Serial.println(i);

        startPlayRtttlPGMPGM(TONE_PIN, tSongPtr);
        while (updatePlayRtttl()) {
            /*
             * your own code here...
             */
            delay(1);
        }
        delay(2000);
    }
    delay(20000);
}

