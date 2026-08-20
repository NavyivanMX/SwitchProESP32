// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProController.h
// Función : Orquestador principal del controlador.
// ============================================================

#pragma once

#include <stdint.h>

#include "SwitchProControllerState.h"
#include "SwitchProHID.h"

class SwitchProHID;


class SwitchProController
{
public:

SwitchProController();
    // --------------------------------------------------------
    // Inicialización
    // --------------------------------------------------------

    bool begin();


    // ========================================================
    // Actualización del controlador
    // ========================================================

    void update();


    // ========================================================
    // Obtener estado actual
    // ========================================================

    const SwitchProControllerState& state() const;


private:

    // ========================================================
    // Estado del controlador
    // ========================================================

    SwitchProControllerState m_state;


    // ========================================================
    // Control de frecuencia de reportes
    // ========================================================

    uint32_t m_lastReportTime;
    

    // --------------------------------------------------------
    // HID
    // --------------------------------------------------------

    SwitchProHID* m_hid = nullptr;

    // ========================================================
    // Control de frecuencia de reportes
    // ========================================================

    bool reportIntervalElapsed(uint32_t now) const;

    // ========================================================
    // Configuración de reportes
    // ========================================================

    static constexpr uint32_t ReportIntervalMs = 10;    

    // ========================================================
    // Generación de reportes
    // ========================================================

    void updateReport();

    // ========================================================
    // Tiempo
    // ========================================================

    uint32_t getMillis() const;    

    // ========================================================
    // Actualización del estado
    // ========================================================

    void updateState();
};