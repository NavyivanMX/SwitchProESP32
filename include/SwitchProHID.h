// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.h
// Función : Capa de abstracción HID.
//
// En esta etapa:
//
//   - No inicializa Bluetooth todavía.
//   - Solamente prepara la arquitectura.
//
// La implementación Bluetooth llegará después de obtener
// nuestro primer Build verde con ESP-IDF puro.
// ============================================================

#pragma once


class SwitchProHID
{
public:

    // --------------------------------------------------------
    // Inicialización
    // --------------------------------------------------------

    bool begin();


    // --------------------------------------------------------
    // Actualización
    // --------------------------------------------------------

    void update();
};