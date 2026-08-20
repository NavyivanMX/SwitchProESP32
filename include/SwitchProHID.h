// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.h
// Función : Capa Bluetooth HID.
//
// ETAPA 5.8.20
//
// Objetivo:
//   - Inicializar Bluetooth Classic.
//   - Inicializar Bluedroid.
//   - Inicializar HID Device.
//   - Registrar aplicación HID.
//   - Observar eventos HID.
//   - Observar Output Reports recibidos.
//
// IMPORTANTE:
//
//   Todavía NO implementamos:
//   - Handshake Nintendo.
//   - Subcommands.
//   - Rumble.
//   - Joysticks.
//   - Botones.
//   - Envío periódico de Input Reports.
//
//   En esta etapa queremos descubrir exactamente qué
//   información envía el host durante la conexión.
// ============================================================

#pragma once

#include "SwitchProControllerState.h"
#include "SwitchProInputReport.h"

#include "esp_hidd_api.h"


class SwitchProHID
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    SwitchProHID();


    // ========================================================
    // Inicialización
    // ========================================================

    bool begin();


    // ========================================================
    // HID Input Report
    // ========================================================

    bool sendInputReport(
        const SwitchProControllerState& state
    );


    // ========================================================
    // Actualización
    // ========================================================

    void update();


private:

    // ========================================================
    // Input Report Builder
    // ========================================================

    SwitchProInputReport m_inputReport;


    // ========================================================
    // Bluetooth
    // ========================================================

    bool initBluetoothController();

    bool initBluedroid();


    // ========================================================
    // HID
    // ========================================================

    bool initHID();


    // ========================================================
    // HID Callback
    // ========================================================

    static void hidCallback(
        esp_hidd_cb_event_t event,
        esp_hidd_cb_param_t* param
    );


    // ========================================================
    // HID Output Report
    // ========================================================

    static void handleOutputReport(
        const esp_hidd_cb_param_t* param
    );
};