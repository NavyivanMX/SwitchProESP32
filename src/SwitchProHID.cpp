// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.cpp
// Función : Implementación Bluetooth HID.
//
// Compatible con ESP-IDF 6.0.1
//
// Objetivo de esta etapa:
//   - Inicializar Bluetooth Classic.
//   - Inicializar Bluedroid.
//   - Configurar GAP.
//   - Hacer el dispositivo discoverable/connectable.
//   - Inicializar HID Device.
//   - Registrar aplicación HID.
//   - Observar eventos HID.
//   - Observar GET/SET REPORT.
//   - Preparar Input Reports.
// ============================================================

#include "SwitchProHID.h"
#include "SwitchProDescriptor.h"

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_hidd_api.h"
#include "esp_err.h"
#include "esp_log.h"

#include <cstring>

// ============================================================
// TAG
// ============================================================

static const char* TAG = "SwitchProHID";

// ============================================================
// Estado estático
// ============================================================

volatile bool SwitchProHID::s_hidConnected = false;
volatile bool SwitchProHID::s_reportBusy = false;

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
    esp_hidd_cb_param_t* param
)
{
    ESP_LOGI(
        TAG,
        "HID event: %d",
        static_cast<int>(event)
    );

    if (param == nullptr)
    {
        ESP_LOGW(
            TAG,
            "HID event has NULL parameter"
        );

        return;
    }

    switch (event)
    {
        // ----------------------------------------------------
        // HID initialized
        // ----------------------------------------------------

        case ESP_HIDD_INIT_EVT:
        {
            ESP_LOGI(
                TAG,
                "HID Device initialized"
            );

            break;
        }

        // ----------------------------------------------------
        // HID deinitialized
        // ----------------------------------------------------

        case ESP_HIDD_DEINIT_EVT:
        {
            ESP_LOGI(
                TAG,
                "HID Device deinitialized"
            );

            s_hidConnected = false;
            s_reportBusy = false;

            break;
        }

        // ----------------------------------------------------
        // Application registered
        // ----------------------------------------------------

        case ESP_HIDD_REGISTER_APP_EVT:
        {
            ESP_LOGI(
                TAG,
                "HID application registered"
            );

            break;
        }

        // ----------------------------------------------------
        // Application unregistered
        // ----------------------------------------------------

        case ESP_HIDD_UNREGISTER_APP_EVT:
        {
            ESP_LOGI(
                TAG,
                "HID application unregistered"
            );

            break;
        }

        // ----------------------------------------------------
        // Host connected
        // ----------------------------------------------------

        case ESP_HIDD_OPEN_EVT:
        {
            handleOpen(param);

            break;
        }

        // ----------------------------------------------------
        // Host disconnected
        // ----------------------------------------------------

        case ESP_HIDD_CLOSE_EVT:
        {
            handleClose(param);

            break;
        }

        // ----------------------------------------------------
        // Input report finished
        // ----------------------------------------------------

        case ESP_HIDD_SEND_REPORT_EVT:
        {
            handleSendReport(param);

            break;
        }

        // ----------------------------------------------------
        // HID report error
        // ----------------------------------------------------

        case ESP_HIDD_REPORT_ERR_EVT:
        {
            ESP_LOGE(
                TAG,
                "HID REPORT ERROR"
            );

            ESP_LOGE(
                TAG,
                "Status: %d",
                static_cast<int>(
                    param->report_err.status
                )
            );

            ESP_LOGE(
                TAG,
                "Reason: %u",
                param->report_err.reason
            );

            s_reportBusy = false;

            break;
        }

        // ----------------------------------------------------
        // Host asks for a report
        // ----------------------------------------------------

        case ESP_HIDD_GET_REPORT_EVT:
        {
            handleGetReport(param);

            break;
        }

        // ----------------------------------------------------
        // Host sends a SET_REPORT
        // ----------------------------------------------------

        case ESP_HIDD_SET_REPORT_EVT:
        {
            handleSetReport(param);

            break;
        }

        // ----------------------------------------------------
        // Host changes protocol mode
        // ----------------------------------------------------

        case ESP_HIDD_SET_PROTOCOL_EVT:
        {
            handleSetProtocol(param);

            break;
        }

        // ----------------------------------------------------
        // Data received on interrupt channel
        // ----------------------------------------------------

        case ESP_HIDD_INTR_DATA_EVT:
        {
            handleInterruptData(param);

            break;
        }

        // ----------------------------------------------------
        // Virtual cable unplug
        // ----------------------------------------------------

        case ESP_HIDD_VC_UNPLUG_EVT:
        {
            ESP_LOGW(
                TAG,
                "HID virtual cable unplug"
            );

            s_hidConnected = false;
            s_reportBusy = false;

            break;
        }

        // ----------------------------------------------------
        // Unknown / API error
        // ----------------------------------------------------

        case ESP_HIDD_API_ERR_EVT:
        {
            ESP_LOGE(
                TAG,
                "HID API ERROR"
            );

            break;
        }

        default:
        {
            ESP_LOGI(
                TAG,
                "Unhandled HID event: %d",
                static_cast<int>(event)
            );

            break;
        }
    }
}

// ============================================================
// GAP CALLBACK
// ============================================================

void SwitchProHID::gapCallback(
    esp_bt_gap_cb_event_t event,
    esp_bt_gap_cb_param_t* param
)
{
    ESP_LOGI(
        TAG,
        "GAP event: %d",
        static_cast<int>(event)
    );

    if (param == nullptr)
    {
        return;
    }

    switch (event)
    {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
        {
            ESP_LOGI(
                TAG,
                "GAP AUTH COMPLETE"
            );

            ESP_LOGI(
                TAG,
                "Status: %d",
                param->auth_cmpl.stat
            );

            ESP_LOGI(
                TAG,
                "Device: %s",
                param->auth_cmpl.device_name
            );

            break;
        }

        case ESP_BT_GAP_PIN_REQ_EVT:
        {
            ESP_LOGI(
                TAG,
                "GAP PIN REQUEST"
            );

            break;
        }

        case ESP_BT_GAP_CFM_REQ_EVT:
        {
            ESP_LOGI(
                TAG,
                "GAP CONFIRM REQUEST"
            );

            break;
        }

        case ESP_BT_GAP_MODE_CHG_EVT:
        {
            ESP_LOGI(
                TAG,
                "GAP MODE CHANGE"
            );

            break;
        }

        default:
        {
            break;
        }
    }
}

// ============================================================
// handleOpen()
// ============================================================

void SwitchProHID::handleOpen(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID HOST CONNECTED"
    );

    ESP_LOGI(
        TAG,
        "Status: %d",
        static_cast<int>(
            param->open.status
        )
    );

    ESP_LOGI(
        TAG,
        "Connection status: %d",
        static_cast<int>(
            param->open.conn_status
        )
    );

    ESP_LOGI(
        TAG,
        "Host BD_ADDR: %02X:%02X:%02X:%02X:%02X:%02X",
        param->open.bd_addr[0],
        param->open.bd_addr[1],
        param->open.bd_addr[2],
        param->open.bd_addr[3],
        param->open.bd_addr[4],
        param->open.bd_addr[5]
    );

    ESP_LOGI(
        TAG,
        "========================================"
    );

    s_hidConnected = true;
    s_reportBusy = false;
}

// ============================================================
// handleClose()
// ============================================================

void SwitchProHID::handleClose(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGW(
        TAG,
        "========================================"
    );

    ESP_LOGW(
        TAG,
        "HID HOST DISCONNECTED"
    );

    ESP_LOGW(
        TAG,
        "Status: %d",
        static_cast<int>(
            param->close.status
        )
    );

    ESP_LOGW(
        TAG,
        "Connection status: %d",
        static_cast<int>(
            param->close.conn_status
        )
    );

    ESP_LOGW(
        TAG,
        "========================================"
    );

    s_hidConnected = false;
    s_reportBusy = false;
}

// ============================================================
// handleSendReport()
// ============================================================

void SwitchProHID::handleSendReport(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "HID SEND REPORT COMPLETE"
    );

    ESP_LOGI(
        TAG,
        "Status: %d",
        static_cast<int>(
            param->send_report.status
        )
    );

    ESP_LOGI(
        TAG,
        "Reason: %u",
        param->send_report.reason
    );

    ESP_LOGI(
        TAG,
        "Report type: %d",
        static_cast<int>(
            param->send_report.report_type
        )
    );

    ESP_LOGI(
        TAG,
        "Report ID: 0x%02X",
        param->send_report.report_id
    );

    s_reportBusy = false;
}

// ============================================================
// handleGetReport()
// ============================================================

void SwitchProHID::handleGetReport(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID GET_REPORT"
    );

    ESP_LOGI(
        TAG,
        "Report type: %d",
        static_cast<int>(
            param->get_report.report_type
        )
    );

    ESP_LOGI(
        TAG,
        "Report ID: 0x%02X",
        param->get_report.report_id
    );

    ESP_LOGI(
        TAG,
        "Requested buffer size: %u",
        param->get_report.buffer_size
    );

    ESP_LOGI(
        TAG,
        "========================================"
    );

    // --------------------------------------------------------
    // IMPORTANTE:
    //
    // ESP-IDF 6.0.1 no expone aquí:
    //
    //   param->get_report.len
    //
    // ni:
    //
    //   esp_bt_hid_device_report_reply()
    //
    // Por ahora solamente registramos la petición.
    //
    // La respuesta específica de Nintendo se implementará
    // después de observar el comportamiento real de la Switch.
    // --------------------------------------------------------
}

// ============================================================
// handleSetReport()
// ============================================================

void SwitchProHID::handleSetReport(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID SET_REPORT"
    );

    ESP_LOGI(
        TAG,
        "Report type: %d",
        static_cast<int>(
            param->set_report.report_type
        )
    );

    ESP_LOGI(
        TAG,
        "Report ID: 0x%02X",
        param->set_report.report_id
    );

    ESP_LOGI(
        TAG,
        "Length: %u",
        param->set_report.len
    );

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

    ESP_LOGI(
        TAG,
        "========================================"
    );
}

// ============================================================
// handleOutputReport()
//
// Mantener como wrapper porque tu .h ya lo declara.
// ============================================================

void SwitchProHID::handleOutputReport(
    const esp_hidd_cb_param_t* param
)
{
    handleSetReport(param);
}

// ============================================================
// handleSetProtocol()
// ============================================================

void SwitchProHID::handleSetProtocol(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID SET_PROTOCOL"
    );

    ESP_LOGI(
        TAG,
        "Protocol mode: %d",
        static_cast<int>(
            param->set_protocol.protocol_mode
        )
    );

    ESP_LOGI(
        TAG,
        "========================================"
    );
}

// ============================================================
// handleInterruptData()
// ============================================================

void SwitchProHID::handleInterruptData(
    const esp_hidd_cb_param_t* param
)
{
    if (param == nullptr)
    {
        return;
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "HID INTERRUPT DATA"
    );

    ESP_LOGI(
        TAG,
        "Report ID: 0x%02X",
        param->intr_data.report_id
    );

    ESP_LOGI(
        TAG,
        "Length: %u",
        param->intr_data.len
    );

    if (
        param->intr_data.data != nullptr &&
        param->intr_data.len > 0
    )
    {
        ESP_LOG_BUFFER_HEX(
            TAG,
            param->intr_data.data,
            param->intr_data.len
        );
    }

    ESP_LOGI(
        TAG,
        "========================================"
    );
}

// ============================================================
// sendInputReport()
// ============================================================
bool SwitchProHID::sendInputReport(
    const SwitchProControllerState& state
)
{
    if (!s_hidConnected)
    {
        return false;
    }

    if (s_reportBusy)
    {
        return false;
    }

    const size_t reportSize =
        m_inputReport.build(state);

    if (reportSize == 0)
    {
        ESP_LOGW(
            TAG,
            "Failed to build HID input report"
        );

        return false;
    }

    esp_err_t result =
        esp_bt_hid_device_send_report(
            ESP_HIDD_REPORT_TYPE_INTRDATA,
            SwitchProInputReport::ReportId,
            static_cast<uint16_t>(reportSize),
            m_inputReport.mutableData()
        );

    if (result != ESP_OK)
    {
        ESP_LOGW(
            TAG,
            "HID send failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }

    s_reportBusy = true;

    return true;
}


// bool SwitchProHID::sendInputReport(
//     const SwitchProControllerState& state
// )
// {
//     // --------------------------------------------------------
//     // No conectado
//     // --------------------------------------------------------

//     if (!s_hidConnected)
//     {
//         return false;
//     }

//     // --------------------------------------------------------
//     // El stack todavía está enviando el reporte anterior
//     // --------------------------------------------------------

//     if (s_reportBusy)
//     {
//         return false;
//     }

//     // --------------------------------------------------------
//     // Construir reporte
//     // --------------------------------------------------------

//     const size_t reportSize =
//         m_inputReport.build(state);

//     if (reportSize == 0)
//     {
//         ESP_LOGE(
//             TAG,
//             "Failed to build HID input report"
//         );

//         return false;
//     }

//     // --------------------------------------------------------
//     // Enviar
//     //
//     // ESP-IDF 6.0.1 exige uint8_t* mutable.
//     // --------------------------------------------------------

//     esp_err_t result =
//         esp_bt_hid_device_send_report(
//             ESP_HIDD_REPORT_TYPE_INTRDATA,
//             SwitchProInputReport::ReportId,
//             static_cast<uint16_t>(
//                 reportSize
//             ),
//             m_inputReport.mutableData()
//         );

//     if (result != ESP_OK)
//     {
//         ESP_LOGW(
//             TAG,
//             "HID send failed: %s",
//             esp_err_to_name(result)
//         );

//         return false;
//     }

//     // --------------------------------------------------------
//     // Esperar SEND_REPORT_EVT antes del siguiente
//     // --------------------------------------------------------

//     s_reportBusy = true;

//     return true;
// }

// ============================================================
// update()
// ============================================================

void SwitchProHID::update()
{
    // --------------------------------------------------------
    // Por ahora NO enviamos reportes automáticamente.
    //
    // Primero necesitamos conseguir una conexión HID real
    // con la Nintendo Switch y observar:
    //
    //   OPEN
    //   GET_REPORT
    //   SET_REPORT
    //   SET_PROTOCOL
    //   INTR_DATA
    //
    // Después implementaremos el protocolo Switch Pro.
    // --------------------------------------------------------
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
    // 1. Bluetooth controller
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
    // 3. GAP
    // --------------------------------------------------------

    if (!initBluetoothGAP())
    {
        ESP_LOGE(
            TAG,
            "Bluetooth GAP initialization failed"
        );

        return false;
    }

    // --------------------------------------------------------
    // 4. HID
    // --------------------------------------------------------

    if (!initHID())
    {
        ESP_LOGE(
            TAG,
            "Bluetooth HID initialization failed"
        );

        return false;
    }

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

    esp_bt_controller_config_t btConfig =
        BT_CONTROLLER_INIT_CONFIG_DEFAULT();

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
// initBluetoothGAP()
// ============================================================

bool SwitchProHID::initBluetoothGAP()
{
    ESP_LOGI(
        TAG,
        "Initializing Bluetooth GAP..."
    );

    // --------------------------------------------------------
    // Registrar GAP callback
    // --------------------------------------------------------

    esp_err_t result =
        esp_bt_gap_register_callback(
            gapCallback
        );

    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_gap_register_callback failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }

    // --------------------------------------------------------
    // Nombre Bluetooth
    // --------------------------------------------------------

    result =
        esp_bt_gap_set_device_name(
            "SwitchProESP32"
        );

    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_dev_set_device_name failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }

    // --------------------------------------------------------
    // Discoverable + Connectable
    // --------------------------------------------------------

    result =
        esp_bt_gap_set_scan_mode(
            ESP_BT_CONNECTABLE,
            ESP_BT_GENERAL_DISCOVERABLE
        );

    if (result != ESP_OK)
    {
        ESP_LOGE(
            TAG,
            "esp_bt_gap_set_scan_mode failed: %s",
            esp_err_to_name(result)
        );

        return false;
    }

    ESP_LOGI(
        TAG,
        "Bluetooth GAP: CONNECTABLE + DISCOVERABLE"
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
    // Report Descriptor
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
    // Registrar aplicación
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