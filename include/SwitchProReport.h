// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProReport.h
// Función : Representación de los datos de entrada del
//           controlador.
//
// ETAPA 5.7.1
//
// En esta etapa:
//
//   - No usamos hardware.
//   - No usamos joysticks físicos.
//   - No enviamos todavía el reporte a la Switch.
//
// Creamos únicamente la estructura lógica que posteriormente
// será convertida en un HID Input Report.
//
// Referencia conceptual:
//   Report ID 0x30 = Standard Full Mode.
// ============================================================

#pragma once

#include <stdint.h>


class SwitchProReport
{
public:

    // --------------------------------------------------------
    // Constructor
    // --------------------------------------------------------

    SwitchProReport();


    // --------------------------------------------------------
    // Reset
    // --------------------------------------------------------
    //
    // Coloca todos los controles en estado neutral.
    //

    void reset();


    // --------------------------------------------------------
    // Botones
    // --------------------------------------------------------

    void setButtons(uint32_t buttons);

    uint32_t getButtons() const;


    // --------------------------------------------------------
    // D-Pad
    // --------------------------------------------------------

    void setDPad(uint8_t value);

    uint8_t getDPad() const;


    // --------------------------------------------------------
    // Joystick izquierdo
    // --------------------------------------------------------

    void setLeftStick(
        uint16_t x,
        uint16_t y
    );

    uint16_t getLeftStickX() const;
    uint16_t getLeftStickY() const;


    // --------------------------------------------------------
    // Joystick derecho
    // --------------------------------------------------------

    void setRightStick(
        uint16_t x,
        uint16_t y
    );

    uint16_t getRightStickX() const;
    uint16_t getRightStickY() const;


    // --------------------------------------------------------
    // Serialización
    // --------------------------------------------------------
    //
    // Convierte el estado lógico en un reporte HID.
    //
    // Por ahora solamente construimos la estructura básica.
    //

    static constexpr uint8_t ReportId = 0x30;

    static constexpr size_t ReportSize = 13;

    void serialize(uint8_t* buffer) const;


private:

    uint32_t m_buttons;

    uint8_t m_dPad;

    uint16_t m_leftStickX;
    uint16_t m_leftStickY;

    uint16_t m_rightStickX;
    uint16_t m_rightStickY;
};