#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>
#include <arduino-timer.h>


// Leds...
const int RGBDataPin = 5;
const int RGBClock = 13;
const int NumLed = 8;

int StatusLED = 0;
// Status led color 0 for off or 5 for 255...
int StatusLEDcolor = 0;


// Defining LED array
CRGB leds[NumLed];

bool Status = false; 
int DataRcv;

int NextRcv = (millis() / 1000) + 15;



/*
   timer.every(interval, function_to_call);
   timer.every(interval, function_to_call, argument); // with argument
 */

// On rcv
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {

    memcpy(&DataRcv, incomingData, sizeof(DataRcv));

    Serial.print("data received: ");
    Serial.print(DataRcv);
    Serial.println("");

    if (DataRcv != 1 and DataRcv != 9){
        Serial.println("data other than 1 or 9 received");
        exit(0);
    }

    if (DataRcv == 1){

        for (int c = 0; c <= 2; c++){

            Serial.println(c);

            for (int i = 1; i < NumLed; i++){
                Serial.println("turning led on");
                leds[i].setRGB(0,255,0);
                FastLED.show();
            }


            Serial.println("-----");
            delay(150);

            for (int i = 1; i < NumLed; i++){
                Serial.println("turning led off");
                leds[i].setRGB(0,0,0);
                FastLED.show();
            }

            delay(150);
        }
    } else if (DataRcv == 9){
        Status = true;
        Serial.println("status is now true");
        NextRcv =  (millis() / 1000) + 15;
    }

    else {
        Serial.println("ERROR");
    }

}



void setup() {

    Serial.begin(115200);

    FastLED.addLeds<WS2812B, RGBDataPin, RGB>(leds, NumLed);
    WiFi.mode(WIFI_STA);

    // Printing mac
    Serial.print("MAC Address: ");
    Serial.println(WiFi.macAddress());

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Does things on callback (when data is sent over)
    esp_now_register_recv_cb(OnDataRecv);
}


void loop() {

    if (Status == true and StatusLEDcolor == 0){
        leds[StatusLED].setRGB(0,0,255);
        FastLED.show();
        StatusLEDcolor = 5;
    }

    else if (Status == false and StatusLEDcolor == 5){
        leds[StatusLED].setRGB(0,0,0);
        FastLED.show();
        StatusLEDcolor = 0;
    }

    // if more than 30 seconds pass status led is set to null
    if ((millis()/1000) > NextRcv and Status == true){
        Status = false;
    }
}
