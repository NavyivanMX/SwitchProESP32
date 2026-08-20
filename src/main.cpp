// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : main.cpp
// Función : Punto de entrada principal del firmware.
//
// ETAPA 5.4
//   Bluetooth Classic.
//
// Framework:
//   ESP-IDF
//
// Punto de entrada:
//
//   app_main()
// ============================================================

#include "SwitchProController.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


// ============================================================
// app_main()
// ============================================================

extern "C" void app_main()
{
    SwitchProController controller;


    // --------------------------------------------------------
    // Inicialización
    // --------------------------------------------------------

    if (!controller.begin())
    {
        while (true)
        {
            // --------------------------------------------
            // Error fatal de inicialización.
            //
            // Por ahora dejamos el sistema detenido.
            // --------------------------------------------

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }


    // --------------------------------------------------------
    // Loop principal
    // --------------------------------------------------------

    while (true)
    {
        controller.update();

        vTaskDelay(pdMS_TO_TICKS(10));
    }
}