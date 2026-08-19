#include <Arduino.h>

#include "SwitchProController.h"

SwitchProController switchController;

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Serial.println();
    Serial.println("========================================");
    Serial.println("          SwitchProESP32");
    Serial.println("========================================");
    Serial.println("Hardware : ESP32-WROOM-32");
    Serial.println("Board    : DOIT ESP32 DEVKIT V1");
    Serial.println("Framework: PlatformIO / Arduino + ESP-IDF");
    Serial.println("========================================");
    Serial.println();

    if (!switchController.begin())
    {
        Serial.println();
        Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        Serial.println("ERROR: No se pudo iniciar el controlador");
        Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

        return;
    }

    Serial.println();
    Serial.println("Controlador Bluetooth inicializado.");
    Serial.println("Esperando Nintendo Switch...");
    Serial.println();
}

void loop()
{
    switchController.update();

    static unsigned long lastStatus = 0;

    if (millis() - lastStatus >= 2000)
    {
        lastStatus = millis();

        Serial.printf(
            "[STATUS] Connected: %s | Handshake: %s\n",
            switchController.isConnected()
                ? "YES"
                : "NO",
            switchController.isHandshakeComplete()
                ? "YES"
                : "NO"
        );
    }

    delay(10);
}