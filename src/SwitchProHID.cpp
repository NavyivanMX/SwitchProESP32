// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.cpp
// Función : Implementación Bluetooth HID.
//
// ETAPA 5.5
//   Inicialización de Bluetooth HID.
//
// Arquitectura:
//
//   Bluetooth Controller
//          ↓
//      Bluedroid
//          ↓
//       HID Device
//
// IMPORTANTE:
//
//   En esta etapa todavía NO implementamos:
//   - Descriptor HID definitivo
//   - Reportes del Pro Controller
//   - Botones
//   - Joysticks
//   - Pairing con Nintendo Switch
//
// El objetivo es únicamente comprobar que el ESP32
// puede inicializar correctamente Bluetooth HID.
// ============================================================

#include "SwitchProHID.h"
#include "SwitchProDescriptor.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_hidd_api.h"
#include "esp_err.h"
#include "esp_log.h"


// ============================================================
// TAG
// ============================================================

static const char* TAG = "SwitchProHID";


// ============================================================
// HID CALLBACK
// ============================================================
//
// Este callback recibirá los eventos generados por el
// dispositivo HID.
//
// Por ahora solamente los mostramos en el monitor serial.
//
// Más adelante aquí manejaremos eventos como:
//
//   - conexión
//   - desconexión
//   - handshake
//   - control de protocolo
//   - envío de reportes
//
// ============================================================

static void hidCallback(
    esp_hidd_cb_event_t event,
    esp_hidd_cb_param_t* param)
{
    if (param == nullptr)
    {
        ESP_LOGI(
            TAG,
            "HID event: %d",
            static_cast<int>(event)
        );

        return;
    }


    ESP_LOGI(
        TAG,
        "HID event: %d",
        static_cast<int>(event)
    );
}


// ============================================================
// HID REPORT MAP
// ============================================================
//
// Este descriptor es INTENCIONALMENTE mínimo.
//
// Describe un dispositivo HID de tipo Game Pad, pero todavía
// NO representa al Nintendo Switch Pro Controller.
//
// El descriptor definitivo lo construiremos en una etapa
// posterior.
//
// ============================================================




// ============================================================
// begin()
// ============================================================

bool SwitchProHID::begin()
{
    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "SwitchProESP32"
    );

    ESP_LOGI(
        TAG,
        "Bluetooth HID initialization"
    );

    ESP_LOGI(
        TAG,
        "========================================"
    );


    // --------------------------------------------------------
    // 1. Bluetooth Controller
    // --------------------------------------------------------

    if (!initBluetoothController())
    {
        ESP_LOGE(
            TAG,
            "Bluetooth controller initialization failed"
        );

        return false;
    }


    // --------------------------------------------------------
    // 2. Bluedroid
    // --------------------------------------------------------

    if (!initBluedroid())
    {
        ESP_LOGE(
            TAG,
            "Bluedroid initialization failed"
        );

        return false;
    }


    // --------------------------------------------------------
    // 3. HID
    // --------------------------------------------------------

    if (!initHID())
    {
        ESP_LOGE(
            TAG,
            "Bluetooth HID initialization failed"
        );

        return false;
    }


    // --------------------------------------------------------
    // HID listo
    // --------------------------------------------------------

    ESP_LOGI(
        TAG,
        "Bluetooth HID: READY"
    );


    return true;
}


// ============================================================
// initBluetoothController()
// ============================================================

bool SwitchProHID::initBluetoothController()
{
    ESP_LOGI(
        TAG,
        "Initializing Bluetooth controller..."
    );


    // --------------------------------------------------------
    // Configuración por defecto del Bluetooth Controller
    // --------------------------------------------------------

    esp_bt_controller_config_t btConfig =
        BT_CONTROLLER_INIT_CONFIG_DEFAULT();


    // --------------------------------------------------------
    // Inicializar Controller
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


    ESP_LOGI(
        TAG,
        "Bluetooth controller: OK"
    );


    // --------------------------------------------------------
    // Habilitar Bluetooth Classic
    // --------------------------------------------------------

    result =
        esp_bt_controller_enable(
            ESP_BT_MODE_CLASSIC_BT
        );


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_controller_enable failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(
        TAG,
        "Bluetooth Classic enabled"
    );


    return true;
}


// ============================================================
// initBluedroid()
// ============================================================

bool SwitchProHID::initBluedroid()
{
    ESP_LOGI(
        TAG,
        "Initializing Bluedroid..."
    );


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


    ESP_LOGI(
        TAG,
        "Bluedroid initialized"
    );


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


    ESP_LOGI(
        TAG,
        "Bluedroid enabled"
    );


    return true;
}


// ============================================================
// initHID()
// ============================================================

bool SwitchProHID::initHID()
{
    ESP_LOGI(
        TAG,
        "Initializing Bluetooth HID Device..."
    );


    // --------------------------------------------------------
    // 1. Registrar callback HID
    // --------------------------------------------------------

    esp_err_t result =
        esp_bt_hid_device_register_callback(
            hidCallback
        );


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_hid_device_register_callback failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(
        TAG,
        "HID callback registered"
    );


    // --------------------------------------------------------
    // 2. Inicializar HID Device
    // --------------------------------------------------------

    result =
        esp_bt_hid_device_init();


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_hid_device_init failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(
        TAG,
        "HID Device initialized"
    );


    // --------------------------------------------------------
    // 3. Parámetros de la aplicación HID
    // --------------------------------------------------------

    esp_hidd_app_param_t appParam{};


    appParam.name =
        const_cast<char*>(
            "SwitchProESP32"
        );


    appParam.description =
        const_cast<char*>(
            "Nintendo Switch Controller"
        );


    appParam.provider =
        const_cast<char*>(
            "SwitchProESP32"
        );


    // --------------------------------------------------------
    // Subclass
    // --------------------------------------------------------
    //
    // 0x04 = Game Pad
    //
    // IMPORTANTE:
    // Todavía no estamos afirmando que esto sea un
    // Nintendo Switch Pro Controller.
    //
    // La identidad definitiva vendrá después.
    // --------------------------------------------------------

    appParam.subclass = 0x04;


    // --------------------------------------------------------
    // Report Map
    // --------------------------------------------------------

appParam.desc_list =
    const_cast<uint8_t*>(
        SwitchProDescriptor::ReportMap
    );

appParam.desc_list_len =
    SwitchProDescriptor::ReportMapSize;

    // --------------------------------------------------------
    // 4. QoS
    // --------------------------------------------------------

    esp_hidd_qos_param_t inQos{};

    esp_hidd_qos_param_t outQos{};


    // --------------------------------------------------------
    // 5. Registrar aplicación HID
    // --------------------------------------------------------

    result =
        esp_bt_hid_device_register_app(
            &appParam,
            &inQos,
            &outQos
        );


    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_hid_device_register_app failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }


    ESP_LOGI(
        TAG,
        "HID application registered"
    );


    return true;
}


// ============================================================
// update()
// ============================================================

void SwitchProHID::update()
{
    // --------------------------------------------------------
    // En esta etapa todavía no enviamos reportes HID.
    //
    // Los eventos Bluetooth/HID son manejados mediante
    // callbacks.
    // --------------------------------------------------------
}