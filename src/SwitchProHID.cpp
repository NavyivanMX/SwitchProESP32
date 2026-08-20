// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.cpp
// Función : Implementación Bluetooth HID.
//
// ETAPA 5.8.20
//
// Objetivo:
//   Observar la comunicación HID recibida desde el host.
//
// Todavía NO implementamos respuestas del protocolo Nintendo.
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
// Constructor
// ============================================================

SwitchProHID::SwitchProHID()
    : m_inputReport()
{
}


// ============================================================
// HID CALLBACK
// ============================================================

void SwitchProHID::hidCallback(
    esp_hidd_cb_event_t event,
    esp_hidd_cb_param_t* param)
{
    // --------------------------------------------------------
    // Mostrar siempre el evento recibido.
    // --------------------------------------------------------

    ESP_LOGI(
        TAG,
        "HID event: %d",
        static_cast<int>(event)
    );


    // --------------------------------------------------------
    // Si no tenemos parámetros, terminamos.
    // --------------------------------------------------------

    if (param == nullptr)
    {
        ESP_LOGW(
            TAG,
            "HID event has NULL parameter"
        );

        return;
    }


    // --------------------------------------------------------
    // Output Report
    // --------------------------------------------------------

    if (event == ESP_HIDD_SET_REPORT_EVT)
    {
        handleOutputReport(param);
    }
}


// ============================================================
// HANDLE OUTPUT REPORT
// ============================================================

void SwitchProHID::handleOutputReport(
    const esp_hidd_cb_param_t* param)
{
    if (param == nullptr)
    {
        return;
    }


    // --------------------------------------------------------
    // Información básica del reporte.
    // --------------------------------------------------------

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID OUTPUT REPORT RECEIVED"
    );

    ESP_LOGI(
        TAG,
        "Report ID: %u",
        param->set_report.report_id
    );

    ESP_LOGI(
        TAG,
        "Report type: %u",
        param->set_report.report_type
    );

    ESP_LOGI(
        TAG,
        "Report length: %u",
        param->set_report.len
    );


    // --------------------------------------------------------
    // Mostrar bytes recibidos.
    // --------------------------------------------------------

    if (
        param->set_report.data != nullptr &&
        param->set_report.len > 0
    )
    {
        ESP_LOG_BUFFER_HEX(
            TAG,
            param->set_report.data,
            param->set_report.len
        );
    }
    else
    {
        ESP_LOGI(
            TAG,
            "Report contains no data"
        );
    }


    ESP_LOGI(
        TAG,
        "========================================"
    );
}


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
    // Configuración por defecto
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
    // Registrar callback HID
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
    // Inicializar HID Device
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
    // Parámetros de aplicación HID
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
    // QoS
    // --------------------------------------------------------

    esp_hidd_qos_param_t inQos{};

    esp_hidd_qos_param_t outQos{};


    // --------------------------------------------------------
    // Registrar aplicación HID
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
// sendInputReport()
// ============================================================

bool SwitchProHID::sendInputReport(
    const SwitchProControllerState& state
)
{
    // --------------------------------------------------------
    // Construir reporte
    // --------------------------------------------------------

    const size_t reportSize =
        m_inputReport.build(state);


    if (reportSize == 0)
    {
        ESP_LOGE(
            TAG,
            "Failed to build HID input report"
        );

        return false;
    }


    ESP_LOGD(
        TAG,
        "HID input report built (%u bytes)",
        static_cast<unsigned>(reportSize)
    );


    return true;
}


// ============================================================
// update()
// ============================================================

void SwitchProHID::update()
{
    // --------------------------------------------------------
    // Por ahora no enviamos Input Reports.
    //
    // Los eventos HID llegan mediante hidCallback().
    // --------------------------------------------------------
}