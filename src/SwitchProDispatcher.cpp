// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDispatcher.cpp
// Función : Implementación del Response Dispatcher.
//
// ETAPA 5.8.7
//
// Integra los diferentes handlers de protocolo con el
// Response Builder.
//
// ============================================================

#include "SwitchProDispatcher.h"

#include "SwitchProDeviceInfo.h"


// ============================================================
// Constructor
// ============================================================

SwitchProDispatcher::SwitchProDispatcher()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProDispatcher::reset()
{
    m_protocol.reset();

    m_response.reset();

    m_hasResponse = false;
}


// ============================================================
// process()
// ============================================================

bool SwitchProDispatcher::process(
    const uint8_t* data,
    size_t length
)
{
    // --------------------------------------------------------
    // Limpiar respuesta anterior.
    // --------------------------------------------------------

    m_response.reset();

    m_hasResponse = false;


    // ========================================================
    // Entregar paquete al protocolo
    // ========================================================

    if (
        !m_protocol.processOutputReport(
            data,
            length
        )
    )
    {
        return false;
    }


    // ========================================================
    // Obtener Subcommand
    // ========================================================

    const uint8_t subcommand =
        m_protocol.lastSubcommand();


    // ========================================================
    // Procesar Subcommand
    // ========================================================

    switch (subcommand)
    {
        // ----------------------------------------------------
        // Device Info
        // ----------------------------------------------------

        case 0x02:
        {
            uint8_t deviceInfo[16];

            const size_t infoSize =
                SwitchProDeviceInfo::build(
                    deviceInfo,
                    sizeof(deviceInfo)
                );


            m_response.build(
                subcommand,
                deviceInfo,
                infoSize
            );

            m_hasResponse = true;

            return true;
        }


        // ----------------------------------------------------
        // Comandos conocidos pero todavía no implementados.
        // ----------------------------------------------------

        case 0x01:
        case 0x03:
        case 0x04:
        case 0x10:
        case 0x30:
        case 0x31:
        case 0x40:
        case 0x41:
        case 0x48:
        {
            m_response.build(
                subcommand,
                nullptr,
                0
            );

            m_hasResponse = true;

            return true;
        }


        // ----------------------------------------------------
        // Desconocido
        // ----------------------------------------------------

        default:
            return false;
    }
}


// ============================================================
// hasResponse()
// ============================================================

bool SwitchProDispatcher::hasResponse() const
{
    return m_hasResponse;
}


// ============================================================
// response()
// ============================================================

const SwitchProResponse&
SwitchProDispatcher::response() const
{
    return m_response;
}