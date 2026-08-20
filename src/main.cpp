// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : main.cpp
// Etapa   : 5.8.14
// Función : Punto de entrada principal del firmware.
// ============================================================

#include "SwitchProController.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_err.h"
#include "esp_log.h"

// ============================================================
// Controlador principal
// ============================================================

static SwitchProController g_controller;


// ============================================================
// app_main()
// ============================================================

extern "C"
void app_main()
{
    // ========================================================
    // Inicializar NVS
    // ========================================================

    esp_err_t nvsResult = nvs_flash_init();

    if (
        nvsResult == ESP_ERR_NVS_NO_FREE_PAGES ||
        nvsResult == ESP_ERR_NVS_NEW_VERSION_FOUND
    )
    {
        ESP_LOGW(
            "SwitchProESP32",
            "NVS requires erase"
        );

        nvsResult = nvs_flash_erase();

        if (nvsResult != ESP_OK)
        {
            ESP_LOGE(
                "SwitchProESP32",
                "NVS erase failed: %s",
                esp_err_to_name(nvsResult)
            );

            return;
        }

        nvsResult = nvs_flash_init();
    }


    if (nvsResult != ESP_OK)
    {
        ESP_LOGE(
            "SwitchProESP32",
            "NVS initialization failed: %s",
            esp_err_to_name(nvsResult)
        );

        return;
    }


    ESP_LOGI(
        "SwitchProESP32",
        "NVS: READY"
    );


    // ========================================================
    // Inicializar controlador
    // ========================================================

    if (!g_controller.begin())
    {
        return;
    }


    // ========================================================
    // Bucle principal
    // ========================================================

    while (true)
    {
        g_controller.update();

        vTaskDelay(
            pdMS_TO_TICKS(1)
        );
    }
}