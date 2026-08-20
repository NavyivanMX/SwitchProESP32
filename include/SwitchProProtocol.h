// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProProtocol.h
// Función : Protocolo específico de Nintendo Switch.
//
// ETAPA 5.8.1
//
// Este módulo NO maneja:
//
//   - Bluetooth
//   - GPIO
//   - Joysticks
//   - WiFi
//
// Su responsabilidad es representar los conceptos del
// protocolo utilizado por el Nintendo Switch Pro Controller.
//
// Referencia conceptual:
//
//   Input Report
//   Output Report
//   Subcommand
//   Response
//
// ============================================================

#pragma once

#include <stdint.h>
#include <stddef.h>


class SwitchProProtocol
{
public:

    // ========================================================
    // Report IDs
    // ========================================================

    enum class ReportId : uint8_t
    {
        StandardInput = 0x30,

        // Output report utilizado por el protocolo.
        Subcommand = 0x01,

        // Respuesta a subcomandos.
        SubcommandResponse = 0x21
    };


    // ========================================================
    // Constructor
    // ========================================================

    SwitchProProtocol();


    // ========================================================
    // Reset
    // ========================================================
    //
    // Regresa el protocolo a su estado inicial.
    //

    void reset();


    // ========================================================
    // Estado
    // ========================================================

    bool initialized() const;


    // ========================================================
    // Procesamiento de Output Report
    // ========================================================
    //
    // Recibe un paquete proveniente del host.
    //
    // En esta primera etapa todavía no ejecutamos
    // subcomandos reales.
    //

    bool processOutputReport(
        const uint8_t* data,
        size_t length
    );


    // ========================================================
    // Último Report ID recibido
    // ========================================================

    uint8_t lastReportId() const;


private:

    bool m_initialized;

    uint8_t m_lastReportId;
};