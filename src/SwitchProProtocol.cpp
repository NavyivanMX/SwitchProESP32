// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProProtocol.cpp
// Función : Implementación del protocolo Nintendo.
//
// ETAPA 5.8.3
// ============================================================

#include "SwitchProProtocol.h"

#include <string.h>


// ============================================================
// Constructor
// ============================================================

SwitchProProtocol::SwitchProProtocol()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProProtocol::reset()
{
    m_initialized = false;

    m_lastReportId = 0x00;

    m_lastSubcommand = 0x00;

    m_lastArgumentLength = 0;

    memset(
        m_lastArguments,
        0,
        sizeof(m_lastArguments)
    );
}


// ============================================================
// initialized()
// ============================================================

bool SwitchProProtocol::initialized() const
{
    return m_initialized;
}


// ============================================================
// processOutputReport()
// ============================================================

bool SwitchProProtocol::processOutputReport(
    const uint8_t* data,
    size_t length
)
{
    if (data == nullptr)
    {
        return false;
    }

    if (length == 0)
    {
        return false;
    }


    // --------------------------------------------------------
    // Limpiar estado anterior
    // --------------------------------------------------------

    m_lastReportId = data[0];

    m_lastSubcommand = 0x00;

    m_lastArgumentLength = 0;

    memset(
        m_lastArguments,
        0,
        sizeof(m_lastArguments)
    );


    // ========================================================
    // Verificar Report ID
    // ========================================================

    if (
        m_lastReportId !=
        static_cast<uint8_t>(
            ReportId::Subcommand
        )
    )
    {
        return false;
    }


    // ========================================================
    // Ubicación del Subcommand
    // ========================================================

    constexpr size_t SubcommandOffset = 10;


    if (length <= SubcommandOffset)
    {
        return false;
    }


    // ========================================================
    // Leer Subcommand
    // ========================================================

    m_lastSubcommand =
        data[SubcommandOffset];


    // ========================================================
    // Copiar argumentos
    // ========================================================

    const size_t argumentOffset =
        SubcommandOffset + 1;


    const size_t availableArguments =
        length - argumentOffset;


    m_lastArgumentLength =
        availableArguments < MaxArguments
            ? availableArguments
            : MaxArguments;


    if (m_lastArgumentLength > 0)
    {
        memcpy(
            m_lastArguments,
            &data[argumentOffset],
            m_lastArgumentLength
        );
    }


    // --------------------------------------------------------
    // Reporte reconocido
    // --------------------------------------------------------

    m_initialized = true;

    return true;
}


// ============================================================
// lastReportId()
// ============================================================

uint8_t SwitchProProtocol::lastReportId() const
{
    return m_lastReportId;
}


// ============================================================
// lastSubcommand()
// ============================================================

uint8_t SwitchProProtocol::lastSubcommand() const
{
    return m_lastSubcommand;
}


// ============================================================
// lastSubcommandArgumentLength()
// ============================================================

size_t SwitchProProtocol::lastSubcommandArgumentLength() const
{
    return m_lastArgumentLength;
}


// ============================================================
// lastSubcommandType()
// ============================================================

SwitchProProtocol::Subcommand
SwitchProProtocol::lastSubcommandType() const
{
    switch (m_lastSubcommand)
    {
        case 0x01:
            return Subcommand::ManualPairing;

        case 0x02:
            return Subcommand::GetDeviceInfo;

        case 0x03:
            return Subcommand::SetInputReportMode;

        case 0x04:
            return Subcommand::TriggerButtonsElapsed;

        case 0x10:
            return Subcommand::SPIFlashRead;

        case 0x30:
            return Subcommand::SetPlayerLights;

        case 0x31:
            return Subcommand::GetPlayerLights;

        case 0x40:
            return Subcommand::EnableIMU;

        case 0x41:
            return Subcommand::SetIMUSensitivity;

        case 0x48:
            return Subcommand::EnableVibration;

        default:
            return Subcommand::Unknown;
    }
}


// ============================================================
// isKnownSubcommand()
// ============================================================

bool SwitchProProtocol::isKnownSubcommand(
    uint8_t value
)
{
    switch (value)
    {
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x10:
        case 0x30:
        case 0x31:
        case 0x40:
        case 0x41:
        case 0x48:

            return true;

        default:

            return false;
    }
}


// ============================================================
// copyLastSubcommandArguments()
// ============================================================

size_t SwitchProProtocol::copyLastSubcommandArguments(
    uint8_t* destination,
    size_t destinationSize
) const
{
    if (destination == nullptr)
    {
        return 0;
    }

    if (destinationSize == 0)
    {
        return 0;
    }


    const size_t bytesToCopy =
        m_lastArgumentLength < destinationSize
            ? m_lastArgumentLength
            : destinationSize;


    if (bytesToCopy > 0)
    {
        memcpy(
            destination,
            m_lastArguments,
            bytesToCopy
        );
    }


    return bytesToCopy;
}