// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProReport.cpp
// Función : Implementación del estado del controlador.
//
// ETAPA 5.7.1
// ============================================================

#include "SwitchProReport.h"

#include <string.h>


// ============================================================
// Constructor
// ============================================================

SwitchProReport::SwitchProReport()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProReport::reset()
{
    m_buttons = 0;

    // --------------------------------------------------------
    // D-Pad neutral
    // --------------------------------------------------------

    m_dPad = 0x08;


    // --------------------------------------------------------
    // Sticks centrados
    //
    // El rango del protocolo es de 12 bits dentro de
    // tres bytes por stick.
    //
    // 0x800 representa aproximadamente el centro.
    // --------------------------------------------------------

    m_leftStickX = 0x800;
    m_leftStickY = 0x800;

    m_rightStickX = 0x800;
    m_rightStickY = 0x800;
}


// ============================================================
// Buttons
// ============================================================

void SwitchProReport::setButtons(uint32_t buttons)
{
    m_buttons = buttons;
}


uint32_t SwitchProReport::getButtons() const
{
    return m_buttons;
}


// ============================================================
// D-Pad
// ============================================================

void SwitchProReport::setDPad(uint8_t value)
{
    m_dPad = value & 0x0F;
}


uint8_t SwitchProReport::getDPad() const
{
    return m_dPad;
}


// ============================================================
// Left Stick
// ============================================================

void SwitchProReport::setLeftStick(
    uint16_t x,
    uint16_t y
)
{
    m_leftStickX = x & 0x0FFF;
    m_leftStickY = y & 0x0FFF;
}


uint16_t SwitchProReport::getLeftStickX() const
{
    return m_leftStickX;
}


uint16_t SwitchProReport::getLeftStickY() const
{
    return m_leftStickY;
}


// ============================================================
// Right Stick
// ============================================================

void SwitchProReport::setRightStick(
    uint16_t x,
    uint16_t y
)
{
    m_rightStickX = x & 0x0FFF;
    m_rightStickY = y & 0x0FFF;
}


uint16_t SwitchProReport::getRightStickX() const
{
    return m_rightStickX;
}


uint16_t SwitchProReport::getRightStickY() const
{
    return m_rightStickY;
}


// ============================================================
// serialize()
// ============================================================
//
// Formato básico:
//
// Byte 0
//   Report ID = 0x30
//
// Byte 1
//   Timer
//
// Byte 2
//   Battery / connection info
//
// Byte 3-5
//   Buttons
//
// Byte 6-8
//   Left Stick
//
// Byte 9-11
//   Right Stick
//
// Byte 12
//   Rumble input / reserved
//
// ============================================================

void SwitchProReport::serialize(uint8_t* buffer) const
{
    if (buffer == nullptr)
    {
        return;
    }


    memset(
        buffer,
        0,
        ReportSize
    );


    // --------------------------------------------------------
    // Report ID
    // --------------------------------------------------------

    buffer[0] = ReportId;


    // --------------------------------------------------------
    // Timer
    //
    // En esta primera etapa permanece en cero.
    // Posteriormente será administrado por el controlador.
    // --------------------------------------------------------

    buffer[1] = 0x00;


    // --------------------------------------------------------
    // Battery / connection information
    //
    // Valor provisional.
    // --------------------------------------------------------

    buffer[2] = 0x81;


    // --------------------------------------------------------
    // Buttons
    //
    // El protocolo utiliza 3 bytes para el estado de botones.
    // --------------------------------------------------------

    buffer[3] =
        static_cast<uint8_t>(
            m_buttons & 0xFF
        );

    buffer[4] =
        static_cast<uint8_t>(
            (m_buttons >> 8) & 0xFF
        );

    buffer[5] =
        static_cast<uint8_t>(
            (m_buttons >> 16) & 0xFF
        );


    // --------------------------------------------------------
    // Left Stick
    //
    // 12 bits X + 12 bits Y = 3 bytes.
    // --------------------------------------------------------

    buffer[6] =
        static_cast<uint8_t>(
            m_leftStickX & 0xFF
        );

    buffer[7] =
        static_cast<uint8_t>(
            ((m_leftStickX >> 8) & 0x0F) |
            ((m_leftStickY & 0x0F) << 4)
        );

    buffer[8] =
        static_cast<uint8_t>(
            (m_leftStickY >> 4) & 0xFF
        );


    // --------------------------------------------------------
    // Right Stick
    // --------------------------------------------------------

    buffer[9] =
        static_cast<uint8_t>(
            m_rightStickX & 0xFF
        );

    buffer[10] =
        static_cast<uint8_t>(
            ((m_rightStickX >> 8) & 0x0F) |
            ((m_rightStickY & 0x0F) << 4)
        );

    buffer[11] =
        static_cast<uint8_t>(
            (m_rightStickY >> 4) & 0xFF
        );


    // --------------------------------------------------------
    // Reserved / Rumble input
    // --------------------------------------------------------

    buffer[12] = 0x00;
}