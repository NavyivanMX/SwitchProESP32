// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : main.cpp
// Función : Punto de entrada principal del firmware.
//
// Framework:
//   ESP-IDF
//
// En ESP-IDF la aplicación comienza en:
//
//   app_main()
//
// No utilizamos Arduino setup()/loop().
// ============================================================

#include "SwitchProController.h"


// ============================================================
// app_main()
// ============================================================

extern "C" void app_main()
{
    SwitchProController controller;

    controller.begin();

    while (true)
    {
        controller.update();
    }
}