
#include <Arduino.h>
#include <PlayRtttl.hpp>


const int TONE_PIN = 11;

char StarWarsInRam[] =
        "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";


void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);
    delay(4000); // To be able to connect Serial monitor after reset or power up and before first print out. Do not wait for an attached Serial Monitor!


    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_PLAY_RTTTL));

   
    Serial.println(F("Play StarWars"));
    playRtttlBlocking(TONE_PIN, StarWarsInRam);
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

