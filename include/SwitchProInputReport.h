// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProInputReport.h
// Función : Construcción del HID Input Report.
//
// ETAPA 5.8.9
//
// Convierte SwitchProControllerState en una estructura de
// bytes que posteriormente podrá ser enviada mediante HID.
//
// IMPORTANTE:
//
// En esta etapa construimos únicamente la representación.
// El transporte Bluetooth se conectará posteriormente.
//
// ============================================================

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "SwitchProControllerState.h"


class SwitchProInputReport
{
public:

    // ========================================================
    // Constantes
    // ========================================================

    static constexpr size_t ReportSize = 64;


    // ========================================================
    // Constructor
    // ========================================================

    SwitchProInputReport();


    // ========================================================
    // Reset
    // ========================================================

    void reset();


    // ========================================================
    // Construir reporte
    // ========================================================

    size_t build(
        const SwitchProControllerState& state
    );


    // ========================================================
    // Obtener datos
    // ========================================================

    const uint8_t* data() const;

    size_t size() const;


private:

    uint8_t m_buffer[ReportSize];

    size_t m_size;


    // ========================================================
    // Helpers
    // ========================================================

    void encodeButtons(
        const SwitchProControllerState::Buttons& buttons
    );

    void encodeStick(
        size_t offset,
        const SwitchProControllerState::Stick& stick
    );
};