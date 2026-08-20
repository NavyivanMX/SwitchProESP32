// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProInputReport.cpp
// Función : Implementación del HID Input Report.
//
// ETAPA 5.8.10
//
// Formato Nintendo Pro Controller 0x30.
//
// ============================================================

#include "SwitchProInputReport.h"

#include <string.h>


// ============================================================
// Constructor
// ============================================================

SwitchProInputReport::SwitchProInputReport()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProInputReport::reset()
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

size_t
SwitchProInputReport::build(
    const SwitchProControllerState& state
)
{
    reset();


    // ========================================================
    // Report ID
    // ========================================================

    m_buffer[0] = 0x30;


    // ========================================================
    // Timer
    // ========================================================
    //
    // Por ahora utilizamos un valor estático.
    //
    // Posteriormente lo convertiremos en un contador real.
    //
    // ========================================================

    m_buffer[1] = 0x00;


    // ========================================================
    // Battery / Connection
    // ========================================================
    //
    // High nibble:
    //     nivel de batería.
    //
    // Low nibble:
    //     información de conexión.
    //
    // Para nuestro estado inicial:
    //
    //     batería llena
    //     controlador tipo Pro
    //
    // ========================================================

    uint8_t batteryLevel =
        state.batteryLevel();

    uint8_t batteryNibble = 0x8;

    if (batteryLevel >= 75)
    {
        batteryNibble = 0x8;
    }
    else if (batteryLevel >= 50)
    {
        batteryNibble = 0x6;
    }
    else if (batteryLevel >= 25)
    {
        batteryNibble = 0x4;
    }
    else if (batteryLevel > 0)
    {
        batteryNibble = 0x2;
    }
    else
    {
        batteryNibble = 0x0;
    }


    // Pro Controller / normal connection information.
    constexpr uint8_t ConnectionInfo = 0x00;

    m_buffer[2] =
        static_cast<uint8_t>(
            (batteryNibble << 4) |
            ConnectionInfo
        );


    // ========================================================
    // Botones
    // ========================================================

    encodeButtons(
        state.buttons()
    );


    // ========================================================
    // Left Stick
    // ========================================================

    encodeStick(
        6,
        state.leftStick()
    );


    // ========================================================
    // Right Stick
    // ========================================================

    encodeStick(
        9,
        state.rightStick()
    );


    // ========================================================
    // Vibration status
    // ========================================================

    m_buffer[12] = 0x00;


    // ========================================================
    // IMU
    // ========================================================
    //
    // Todavía no tenemos IMU.
    //
    // Los bytes restantes permanecen en cero.
    //
    // Más adelante implementaremos acelerómetro y giroscopio.
    //
    // ========================================================


    // ========================================================
    // Report size
    // ========================================================

    m_size = ReportSize;

    return m_size;
}


// ============================================================
// encodeButtons()
// ============================================================

void
SwitchProInputReport::encodeButtons(
    const SwitchProControllerState::Buttons& buttons
)
{
    // ========================================================
    // Byte 3
    //
    // Y X B A SR SL R ZR
    // ========================================================

    if (buttons.Y)
        m_buffer[3] |= (1 << 0);

    if (buttons.X)
        m_buffer[3] |= (1 << 1);

    if (buttons.B)
        m_buffer[3] |= (1 << 2);

    if (buttons.A)
        m_buffer[3] |= (1 << 3);

    if (buttons.R)
        m_buffer[3] |= (1 << 6);

    if (buttons.ZR)
        m_buffer[3] |= (1 << 7);


    // ========================================================
    // Byte 4
    //
    // Minus Plus RStick LStick Home Capture
    // ========================================================

    if (buttons.Minus)
        m_buffer[4] |= (1 << 0);

    if (buttons.Plus)
        m_buffer[4] |= (1 << 1);

    if (buttons.RStick)
        m_buffer[4] |= (1 << 2);

    if (buttons.LStick)
        m_buffer[4] |= (1 << 3);

    if (buttons.Home)
        m_buffer[4] |= (1 << 4);

    if (buttons.Capture)
        m_buffer[4] |= (1 << 5);


    // ========================================================
    // Byte 5
    //
    // Down Up Right Left SR SL L ZL
    // ========================================================

    // Por ahora los botones direccionales no forman parte de
    // ControllerState.
    //
    // Los agregaremos posteriormente.

    if (buttons.L)
        m_buffer[5] |= (1 << 6);

    if (buttons.ZL)
        m_buffer[5] |= (1 << 7);
}


// ============================================================
// encodeStick()
// ============================================================

void
SwitchProInputReport::encodeStick(
    size_t offset,
    const SwitchProControllerState::Stick& stick
)
{
    // ========================================================
    // Convertir rango lógico
    //
    // Nuestro ControllerState utiliza:
    //
    //     -2048 ... +2047
    //
    // El Pro Controller utiliza:
    //
    //     0 ... 4095
    //
    // ========================================================

    int32_t x =
        static_cast<int32_t>(stick.x) + 2048;

    int32_t y =
        static_cast<int32_t>(stick.y) + 2048;


    // ========================================================
    // Limitar
    // ========================================================

    if (x < 0)
        x = 0;

    if (x > 4095)
        x = 4095;

    if (y < 0)
        y = 0;

    if (y > 4095)
        y = 4095;


    // ========================================================
    // Empaquetado Nintendo 12-bit
    // ========================================================
    //
    // Byte 0:
    //     X bits 0-7
    //
    // Byte 1:
    //     X bits 8-11
    //     Y bits 0-3
    //
    // Byte 2:
    //     Y bits 4-11
    //
    // ========================================================

    m_buffer[offset + 0] =
        static_cast<uint8_t>(
            x & 0xFF
        );

    m_buffer[offset + 1] =
        static_cast<uint8_t>(
            ((x >> 8) & 0x0F) |
            ((y & 0x0F) << 4)
        );

    m_buffer[offset + 2] =
        static_cast<uint8_t>(
            (y >> 4) & 0xFF
        );
}


// ============================================================
// data()
// ============================================================

const uint8_t*
SwitchProInputReport::data() const
{
    return m_buffer;
}


// ============================================================
// size()
// ============================================================

size_t
SwitchProInputReport::size() const
{
    return m_size;
}