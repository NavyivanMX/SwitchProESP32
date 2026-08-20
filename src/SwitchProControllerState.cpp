// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProControllerState.cpp
// Función : Implementación del estado del controlador.
//
// ETAPA 5.8.8
// ============================================================

#include "SwitchProControllerState.h"


// ============================================================
// Constructor
// ============================================================

SwitchProControllerState::SwitchProControllerState()
{
    reset();
}


// ============================================================
// reset()
// ============================================================

void SwitchProControllerState::reset()
{
    // --------------------------------------------------------
    // Botones
    // --------------------------------------------------------

    m_buttons.A = false;
    m_buttons.B = false;
    m_buttons.X = false;
    m_buttons.Y = false;

    m_buttons.L = false;
    m_buttons.R = false;

    m_buttons.ZL = false;
    m_buttons.ZR = false;

    m_buttons.Plus = false;
    m_buttons.Minus = false;

    m_buttons.Home = false;
    m_buttons.Capture = false;

    m_buttons.LStick = false;
    m_buttons.RStick = false;


    // --------------------------------------------------------
    // Left Stick
    // --------------------------------------------------------

    m_leftStick.x = 0;
    m_leftStick.y = 0;


    // --------------------------------------------------------
    // Right Stick
    // --------------------------------------------------------

    m_rightStick.x = 0;
    m_rightStick.y = 0;


    // --------------------------------------------------------
    // Battery
    // --------------------------------------------------------

    m_batteryLevel = 100;
}


// ============================================================
// buttons()
// ============================================================

const SwitchProControllerState::Buttons&
SwitchProControllerState::buttons() const
{
    return m_buttons;
}


// ============================================================
// buttons()
// ============================================================

SwitchProControllerState::Buttons&
SwitchProControllerState::buttons()
{
    return m_buttons;
}


// ============================================================
// leftStick()
// ============================================================

const SwitchProControllerState::Stick&
SwitchProControllerState::leftStick() const
{
    return m_leftStick;
}


// ============================================================
// rightStick()
// ============================================================

const SwitchProControllerState::Stick&
SwitchProControllerState::rightStick() const
{
    return m_rightStick;
}


// ============================================================
// leftStick()
// ============================================================

SwitchProControllerState::Stick&
SwitchProControllerState::leftStick()
{
    return m_leftStick;
}


// ============================================================
// rightStick()
// ============================================================

SwitchProControllerState::Stick&
SwitchProControllerState::rightStick()
{
    return m_rightStick;
}


// ============================================================
// batteryLevel()
// ============================================================

uint8_t
SwitchProControllerState::batteryLevel() const
{
    return m_batteryLevel;
}


// ============================================================
// setBatteryLevel()
// ============================================================

void
SwitchProControllerState::setBatteryLevel(
    uint8_t level
)
{
    if (level > 100)
    {
        level = 100;
    }

    m_batteryLevel = level;
}