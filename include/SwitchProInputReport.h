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

// ============================================================
// SwitchProInputReport
// ------------------------------------------------------------
// Construye los Input Reports que enviaremos mediante HID.
//
// Para esta etapa:
//   Report ID = 0x30
//   Standard Full Mode
//
// El reporte contiene:
//   - ID
//   - Timer
//   - Banderas
//   - Botones
//   - Stick izquierdo
//   - Stick derecho
//   - Resto reservado
// ============================================================

class SwitchProInputReport
{
public:

// ========================================================
// Constructor
// ========================================================

SwitchProInputReport();


// ========================================================
// Construir reporte
// ========================================================

size_t build(
    const SwitchProControllerState& state
);


// ========================================================
// Datos
// ========================================================

const uint8_t* data() const;

uint8_t* mutableData();

size_t size() const;


// ========================================================
// Constantes
// ========================================================

static constexpr uint8_t ReportId = 0x30;

static constexpr size_t ReportSize = 49;


private:

// ========================================================
// Buffer
// ========================================================

uint8_t m_data[ReportSize];

size_t m_size;


// ========================================================
// Helpers
// ========================================================

void clear();

void encodeButtons(
    const SwitchProControllerState& state
);

void encodeSticks(
    const SwitchProControllerState& state
);

};
