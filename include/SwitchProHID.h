// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.h
// Función : Capa Bluetooth HID.
// ============================================================

#pragma once

#include "SwitchProControllerState.h"
#include "SwitchProInputReport.h"

#include "esp_hidd_api.h"
#include "esp_gap_bt_api.h"


class SwitchProHID
{
public:

    SwitchProHID();

    bool begin();

    bool sendInputReport(
        const SwitchProControllerState& state
    );

    void update();


private:

    // ========================================================
    // Input Report
    // ========================================================

    SwitchProInputReport m_inputReport;


    // ========================================================
    // Bluetooth
    // ========================================================

    bool initBluetoothController();

    bool initBluedroid();


    // ========================================================
    // GAP
    // ========================================================

    bool initBluetoothGAP();


    // ========================================================
    // HID
    // ========================================================

    bool initHID();


    // ========================================================
    // GAP callback
    // ========================================================

    static void gapCallback(
        esp_bt_gap_cb_event_t event,
        esp_bt_gap_cb_param_t* param
    );


    // ========================================================
    // HID events
    // ========================================================

    static void handleOutputReport(
        const esp_hidd_cb_param_t* param
    );

    static void handleGetReport(
        const esp_hidd_cb_param_t* param
    );



    // ========================================================
    // Estado de conexión HID
    // ========================================================

    static volatile bool s_hidConnected;
    static volatile bool s_reportBusy;

    // ========================================================
    // HID Callback
    // ========================================================

    static void hidCallback(
        esp_hidd_cb_event_t event,
        esp_hidd_cb_param_t* param
    );

    // ========================================================
    // HID eventos recibidos
    // ========================================================


    static void handleSetReport(
        const esp_hidd_cb_param_t* param
    );

    static void handleSetProtocol(
        const esp_hidd_cb_param_t* param
    );

    static void handleInterruptData(
        const esp_hidd_cb_param_t* param
    );

    static void handleOpen(
        const esp_hidd_cb_param_t* param
    );

    static void handleClose(
        const esp_hidd_cb_param_t* param
    );

    static void handleSendReport(
        const esp_hidd_cb_param_t* param
    );    
};