// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProProtocol.cpp
// Función : Implementación del protocolo Nintendo.
//
// ETAPA 5.8.1
// ============================================================

#include "SwitchProProtocol.h"


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
//
// En esta etapa solamente identificamos el tipo de reporte.
//
// Todavía NO:
//
//   - respondemos subcomandos
//   - accedemos a SPI
//   - enviamos información de calibración
//   - manejamos rumble
//
// Eso vendrá posteriormente.
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
    // El primer byte identifica el tipo de reporte.
    // --------------------------------------------------------

    m_lastReportId = data[0];


    // --------------------------------------------------------
    // Por ahora reconocemos únicamente los reportes que
    // forman parte de nuestra arquitectura.
    // --------------------------------------------------------

    switch (m_lastReportId)
    {
        case static_cast<uint8_t>(
            ReportId::Subcommand
        ):

            // ----------------------------------------------
            // Output report de subcomando.
            // ----------------------------------------------
            //
            // Todavía no lo ejecutamos.
            //

            m_initialized = true;

            return true;


        default:

            return false;
    }
}


// ============================================================
// lastReportId()
// ============================================================

uint8_t SwitchProProtocol::lastReportId() const
{
    return m_lastReportId;
}