// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProControllerState.h
// Función : Estado lógico del Nintendo Switch Pro Controller.
//
// ETAPA 5.8.8
//
// Este módulo representa el estado actual del controlador.
//
// IMPORTANTE:
//
// En esta etapa NO dependemos de hardware físico.
//
// Los botones y sticks son datos lógicos.
// Más adelante InputManager podrá actualizar este estado.
//
// ============================================================

#pragma once

#include <stdint.h>


class SwitchProControllerState
{
public:

    // ========================================================
    // Botones
    // ========================================================

    struct Buttons
    {
        bool A;
        bool B;
        bool X;
        bool Y;

        bool L;
        bool R;

        bool ZL;
        bool ZR;

        bool Plus;
        bool Minus;

        bool Home;
        bool Capture;

        bool LStick;
        bool RStick;
    };


    // ========================================================
    // Stick
    // ========================================================

    struct Stick
    {
        int16_t x;
        int16_t y;
    };


    // ========================================================
    // Constructor
    // ========================================================

    SwitchProControllerState();


    // ========================================================
    // Reset
    // ========================================================

    void reset();


    // ========================================================
    // Botones
    // ========================================================

    const Buttons& buttons() const;

    Buttons& buttons();


    // ========================================================
    // Sticks
    // ========================================================

    const Stick& leftStick() const;

    const Stick& rightStick() const;

    Stick& leftStick();

    Stick& rightStick();


    // ========================================================
    // Batería
    // ========================================================

    uint8_t batteryLevel() const;

    void setBatteryLevel(uint8_t level);


private:

    Buttons m_buttons;

    Stick m_leftStick;

    Stick m_rightStick;

    uint8_t m_batteryLevel;
};