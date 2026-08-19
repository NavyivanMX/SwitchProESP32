#include "SwitchProController.h"

bool SwitchProController::s_connected = false;
bool SwitchProController::s_handshakeComplete = false;

SwitchProController::SwitchProController()
    : m_started(false)
{
}

bool SwitchProController::begin()
{
    if (m_started)
    {
        return true;
    }

    Serial.println();
    Serial.println("--------------------------------");
    Serial.println("SwitchProController");
    Serial.println("--------------------------------");

    Serial.println("Inicializando Bluetooth...");

    esp_err_t result;

    result = esp_bt_controller_mem_release(
        ESP_BT_MODE_BLE
    );

    if (result != ESP_OK)
    {
        Serial.printf(
            "Aviso: no se pudo liberar BLE: %s\n",
            esp_err_to_name(result)
        );
    }

    if (!esp_bt_controller_is_initialized())
    {
        esp_bt_controller_config_t btConfig =
            BT_CONTROLLER_INIT_CONFIG_DEFAULT();

        result = esp_bt_controller_init(&btConfig);

        if (result != ESP_OK)
        {
            Serial.printf(
                "ERROR: esp_bt_controller_init(): %s\n",
                esp_err_to_name(result)
            );

            return false;
        }
    }

    if (!esp_bt_controller_is_enabled())
    {
        result = esp_bt_controller_enable(
            ESP_BT_MODE_CLASSIC_BT
        );

        if (result != ESP_OK)
        {
            Serial.printf(
                "ERROR: esp_bt_controller_enable(): %s\n",
                esp_err_to_name(result)
            );

            return false;
        }
    }

    if (!esp_bluedroid_get_status())
    {
        result = esp_bluedroid_init();

        if (result != ESP_OK)
        {
            Serial.printf(
                "ERROR: esp_bluedroid_init(): %s\n",
                esp_err_to_name(result)
            );

            return false;
        }
    }

    result = esp_bluedroid_enable();

    if (result != ESP_OK)
    {
        Serial.printf(
            "ERROR: esp_bluedroid_enable(): %s\n",
            esp_err_to_name(result)
        );

        return false;
    }

    esp_bt_gap_register_callback(gapCallback);

    Serial.println("Bluetooth Classic inicializado.");

    m_started = true;

    return true;
}

bool SwitchProController::isConnected() const
{
    return s_connected;
}

bool SwitchProController::isHandshakeComplete() const
{
    return s_handshakeComplete;
}

void SwitchProController::update()
{
    // Próximamente:
    //
    // 1. Procesamiento del estado HID
    // 2. Handshake con Nintendo Switch
    // 3. Respuestas a subcomandos
    // 4. Envío de reportes 0x30
}

void SwitchProController::hidCallback(
    esp_hidd_cb_event_t event,
    esp_hidd_cb_param_t *param)
{
    Serial.printf(
        "[HID] evento: %d\n",
        static_cast<int>(event)
    );

    switch (event)
    {
        case ESP_HIDD_OPEN_EVT:
            s_connected = true;

            Serial.println(
                "[HID] Nintendo Switch conectada."
            );
            break;

        case ESP_HIDD_CLOSE_EVT:
            s_connected = false;
            s_handshakeComplete = false;

            Serial.println(
                "[HID] Nintendo Switch desconectada."
            );
            break;

        default:
            break;
    }
}

void SwitchProController::gapCallback(
    esp_bt_gap_cb_event_t event,
    esp_bt_gap_cb_param_t *param)
{
    Serial.printf(
        "[GAP] evento: %d\n",
        static_cast<int>(event)
    );

    switch (event)
    {
        case ESP_BT_GAP_AUTH_CMPL_EVT:

            if (param != nullptr)
            {
                Serial.printf(
                    "[GAP] Autenticación completada. Estado: %d\n",
                    param->auth_cmpl.stat
                );
            }

            break;

        default:
            break;
    }
}