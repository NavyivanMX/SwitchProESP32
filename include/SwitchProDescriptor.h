// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDescriptor.h
// Función : Descriptor HID del controlador.
//
// ETAPA 5.6
//   Separación del HID Report Map.
//
// En esta etapa:
//
//   - NO implementamos todavía el descriptor final de
//     Nintendo Switch Pro Controller.
//   - Conservamos un descriptor HID mínimo.
//   - Preparamos la arquitectura para reemplazarlo
//     posteriormente.
// ============================================================

#pragma once

#include <stdint.h>


namespace SwitchProDescriptor
{
    // --------------------------------------------------------
    // HID Report Map
    // --------------------------------------------------------

    extern const uint8_t ReportMap[];

    extern const uint16_t ReportMapSize;
}