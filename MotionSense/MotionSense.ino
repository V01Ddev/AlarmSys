#include <Arduino.h>
#include "WiFi.h"
#include <esp_now.h>
#include <arduino-timer.h>


// Pir sensor --> d19

int PIr = 19;
bool PIRState;
bool OldPIRState;

// Timer things
auto timer = timer_create_default(); 
// Rcv mac
uint8_t broadcastAddress[] = {0x7C, 0x87, 0xCE, 0x2D, 0xD2, 0x94};

esp_now_peer_info_t peerInfo;


bool SendStatus(void *){
    int ESPState = 9;

    Serial.println("Sending status packet");

    esp_err_t Sresult = esp_now_send(broadcastAddress, (uint8_t *) &ESPState, sizeof(ESPState));
    return true;
}

void setup(){
    Serial.begin(115200);

    // Setting up PIR sensor 
    pinMode(PIr, INPUT);

    // More timer things
    timer.every(10000, SendStatus);

    // Setting up esp_now
    WiFi.mode(WIFI_STA);

    // Initialize ESP-NOW
    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    // Register the send callback

    // Register peer
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;

    // Add peer        
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
        Serial.println("Failed to add peer");
        return;
    }
}



void loop(){

    // PIR Data

    PIRState = digitalRead(PIr);

    if (PIRState == 1) {

        OldPIRState = PIRState;

        Serial.println("");
        Serial.println("Initial movement detected");
        delay(800);
        PIRState = digitalRead(PIr);

        if (PIRState == 1 && OldPIRState == 1){

            Serial.println("Movement confirmed");
            // Send message via ESP-NOW

            esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &PIRState, sizeof(PIRState));

            Serial.print("\nCurrent packet send status: ");

            if (result == ESP_OK) {
                Serial.print("transmitted");
                Serial.println("");
            }
            else {
                Serial.print("transmission failed");
                Serial.println("");
            }
        } 
        delay(1500);
    }
    timer.tick();
}

