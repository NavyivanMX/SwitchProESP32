// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.cpp
// Función : Implementación de Bluetooth Classic.
//
// ETAPA 5.4
//   Inicialización del sistema Bluetooth.
//
// ESP-IDF:
//   - Bluetooth Controller
//   - Bluetooth Classic
//   - Bluedroid
//
// Todavía NO se inicializa HID.
// ============================================================

#include "SwitchProHID.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_err.h"
#include "esp_log.h"


// ============================================================
// TAG
// ============================================================

static const char* TAG = "SwitchProHID";


// ============================================================
// begin()
// ============================================================

bool SwitchProHID::begin()
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "SwitchProESP32");
    ESP_LOGI(TAG, "Bluetooth initialization");
    ESP_LOGI(TAG, "========================================");


    // --------------------------------------------------------
    // 1. Bluetooth Controller
    // --------------------------------------------------------

    if (!initBluetoothController())
    {
        ESP_LOGE(TAG, "Bluetooth controller initialization failed");

        return false;
    }


    // --------------------------------------------------------
    // 2. Bluedroid
    // --------------------------------------------------------

    if (!initBluedroid())
    {
        ESP_LOGE(TAG, "Bluedroid initialization failed");

        return false;
    }


    // --------------------------------------------------------
    // Bluetooth listo
    // --------------------------------------------------------

    ESP_LOGI(TAG, "Bluetooth Classic: READY");

    return true;
}


// ============================================================
// initBluetoothController()
// ============================================================

bool SwitchProHID::initBluetoothController()
{
    ESP_LOGI(TAG, "Initializing Bluetooth controller...");


    // --------------------------------------------------------
    // Configuración por defecto del controlador
    // --------------------------------------------------------

    esp_bt_controller_config_t btConfig =
        BT_CONTROLLER_INIT_CONFIG_DEFAULT();


    // --------------------------------------------------------
    // Inicializar controlador
    // --------------------------------------------------------

    esp_err_t result =
        esp_bt_controller_init(&btConfig);


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_controller_init failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(TAG, "Bluetooth controller: OK");


    // --------------------------------------------------------
    // Habilitar Bluetooth Classic
    // --------------------------------------------------------

    result =
        esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT);


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_controller_enable failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(TAG, "Bluetooth Classic enabled");

    return true;
}


// ============================================================
// initBluedroid()
// ============================================================

bool SwitchProHID::initBluedroid()
{
    ESP_LOGI(TAG, "Initializing Bluedroid...");


    // --------------------------------------------------------
    // Inicializar Bluedroid
    // --------------------------------------------------------

    esp_err_t result =
        esp_bluedroid_init();


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bluedroid_init failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(TAG, "Bluedroid initialized");


    // --------------------------------------------------------
    // Habilitar Bluedroid
    // --------------------------------------------------------

    result =
        esp_bluedroid_enable();


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bluedroid_enable failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(TAG, "Bluedroid enabled");

    return true;
}


// ============================================================
// update()
// ============================================================

void SwitchProHID::update()
{
    // --------------------------------------------------------
    // En esta etapa todavía no necesitamos realizar ninguna
    // actualización periódica.
    //
    // El Bluetooth Controller y Bluedroid funcionan mediante
    // sus propios eventos internos de ESP-IDF.
    // --------------------------------------------------------
}