// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProResponse.cpp
// Función : Implementación del Response Builder.
//
// ETAPA 5.8.4
// ============================================================

#include "SwitchProResponse.h"

#include <string.h>


// ============================================================
// Constructor
// ============================================================

SwitchProResponse::SwitchProResponse()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProResponse::reset()
{
    memset(
        m_buffer,
        0,
        sizeof(m_buffer)
    );

    m_size = 0;
}


// ============================================================
// build()
// ============================================================

size_t SwitchProResponse::build(
    uint8_t subcommand,
    const uint8_t* data,
    size_t dataLength
)
{
    // --------------------------------------------------------
    // Limpiar respuesta anterior
    // --------------------------------------------------------

    reset();


    // --------------------------------------------------------
    // Limitar cantidad de datos
    // --------------------------------------------------------

    constexpr size_t HeaderSize = 2;

    constexpr size_t MaxPayload =
        ReportSize - HeaderSize;


    if (dataLength > MaxPayload)
    {
        dataLength = MaxPayload;
    }


    // ========================================================
    // Report ID
    // ========================================================

    m_buffer[0] = 0x21;


    // ========================================================
    // Subcommand
    // ========================================================

    m_buffer[1] = subcommand;


    // ========================================================
    // Payload
    // ========================================================

    if (
        data != nullptr &&
        dataLength > 0
    )
    {
        memcpy(
            &m_buffer[HeaderSize],
            data,
            dataLength
        );
    }


    // ========================================================
    // Tamaño final
    // ========================================================

    m_size =
        HeaderSize + dataLength;


    return m_size;
}


// ============================================================
// data()
// ============================================================

const uint8_t* SwitchProResponse::data() const
{
    return m_buffer;
}


// ============================================================
// size()
// ============================================================

size_t SwitchProResponse::size() const
{
    return m_size;
}