#ifndef SWITCH_PRO_CONTROLLER_H
#define SWITCH_PRO_CONTROLLER_H

#include <Arduino.h>

#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_gap_bt_api.h"
#include "esp_hidd_api.h"

class SwitchProController
{
public:
    SwitchProController();

    bool begin();

    bool isConnected() const;

    bool isHandshakeComplete() const;

    void update();

private:
    static void hidCallback(
        esp_hidd_cb_event_t event,
        esp_hidd_cb_param_t *param
    );

    static void gapCallback(
        esp_bt_gap_cb_event_t event,
        esp_bt_gap_cb_param_t *param
    );

    static bool s_connected;
    static bool s_handshakeComplete;

    bool m_started;
};

#endif