// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDispatcher.cpp
// Función : Implementación del Response Dispatcher.
//
// ETAPA 5.8.5
// ============================================================

#include "SwitchProDispatcher.h"


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
    // Entregar el paquete al parser
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
    // Por ahora solamente reconocemos el Subcommand.
    //
    // La construcción de respuestas específicas llegará en
    // las siguientes etapas.
    // ========================================================

    switch (subcommand)
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

            // ------------------------------------------------
            // El comando es conocido.
            //
            // Todavía no generamos su payload definitivo.
            // ------------------------------------------------

            m_response.build(
                subcommand,
                nullptr,
                0
            );

            m_hasResponse = true;

            return true;


        default:

            // ------------------------------------------------
            // Subcommand desconocido.
            // ------------------------------------------------

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