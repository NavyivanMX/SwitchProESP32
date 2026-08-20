// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProProtocol.h
// Función : Protocolo Nintendo Switch.
//
// ETAPA 5.8.3
//
// Se agrega una tabla tipada de Subcommands.
//
// La finalidad es evitar trabajar con números mágicos
// directamente en el resto del proyecto.
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
        Subcommand         = 0x01,
        SubcommandResponse = 0x21,
        StandardInput      = 0x30
    };


    // ========================================================
    // Subcommands
    // ========================================================
    //
    // Estos valores corresponden a comandos utilizados por
    // el protocolo del Pro Controller.
    //
    // Todavía NO ejecutamos ninguno de ellos.
    // ========================================================

    enum class Subcommand : uint8_t
    {
        GetControllerState    = 0x02,
        ManualPairing          = 0x01,
        GetDeviceInfo          = 0x02,

        SetInputReportMode     = 0x03,

        TriggerButtonsElapsed  = 0x04,

        GetPageListState       = 0x10,
        SPIFlashRead           = 0x10,

        SetPlayerLights        = 0x30,
        GetPlayerLights        = 0x31,

        EnableIMU              = 0x40,
        SetIMUSensitivity      = 0x41,

        EnableVibration        = 0x48,

        Unknown                = 0xFF
    };


    // ========================================================
    // Constructor
    // ========================================================

    SwitchProProtocol();


    // ========================================================
    // Reset
    // ========================================================

    void reset();


    // ========================================================
    // Estado
    // ========================================================

    bool initialized() const;


    // ========================================================
    // Procesamiento
    // ========================================================

    bool processOutputReport(
        const uint8_t* data,
        size_t length
    );


    // ========================================================
    // Información del último reporte
    // ========================================================

    uint8_t lastReportId() const;

    uint8_t lastSubcommand() const;

    size_t lastSubcommandArgumentLength() const;


    // ========================================================
    // Acceso tipado al último Subcommand
    // ========================================================

    Subcommand lastSubcommandType() const;


    // ========================================================
    // Validación de Subcommand
    // ========================================================

    static bool isKnownSubcommand(
        uint8_t value
    );


    // ========================================================
    // Copiar argumentos
    // ========================================================

    size_t copyLastSubcommandArguments(
        uint8_t* destination,
        size_t destinationSize
    ) const;


private:

    // ========================================================
    // Constantes
    // ========================================================

    static constexpr size_t MaxArguments = 32;


    // ========================================================
    // Estado
    // ========================================================

    bool m_initialized;

    uint8_t m_lastReportId;

    uint8_t m_lastSubcommand;

    uint8_t m_lastArguments[MaxArguments];

    size_t m_lastArgumentLength;
};