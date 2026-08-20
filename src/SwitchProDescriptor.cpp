// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDescriptor.cpp
// Función : HID Report Descriptor
//
// ETAPA 5.7.5
//
// Implementado:
//
//   - Report ID 0x30
//   - 24 botones
//   - Left Stick  X/Y
//   - Right Stick X/Y
//   - Padding/reserva controlado
//
// IMPORTANTE:
//
// Este descriptor es todavía una etapa de construcción.
// La compatibilidad final con Nintendo dependerá también de
// los reportes y del protocolo específico del Pro Controller.
// ============================================================

#include "SwitchProDescriptor.h"


namespace SwitchProDescriptor
{

    const uint8_t ReportMap[] =
    {

        // ====================================================
        // GAME PAD APPLICATION
        // ====================================================

        0x05, 0x01,       // Usage Page: Generic Desktop
        0x09, 0x05,       // Usage: Game Pad
        0xA1, 0x01,       // Collection: Application


        // ====================================================
        // REPORT ID
        // ====================================================

        0x85, 0x30,


        // ====================================================
        // BUTTONS
        // ====================================================

        0x05, 0x09,       // Usage Page: Button

        0x19, 0x01,       // Usage Minimum: Button 1
        0x29, 0x18,       // Usage Maximum: Button 24

        0x15, 0x00,       // Logical Minimum: 0
        0x25, 0x01,       // Logical Maximum: 1

        0x75, 0x01,       // Report Size: 1 bit
        0x95, 0x18,       // Report Count: 24

        0x81, 0x02,       // Input: Data, Variable, Absolute


        // ====================================================
        // LEFT STICK
        // ====================================================

        0x05, 0x01,       // Usage Page: Generic Desktop

        0x09, 0x30,       // Usage: X
        0x09, 0x31,       // Usage: Y

        0x16, 0x00, 0x00, // Logical Minimum: 0
        0x26, 0xFF, 0x0F, // Logical Maximum: 4095

        0x75, 0x0C,       // Report Size: 12 bits
        0x95, 0x02,       // Report Count: 2

        0x81, 0x02,       // Input: Data, Variable, Absolute


        // ====================================================
        // RIGHT STICK
        // ====================================================

        0x09, 0x32,       // Usage: Z
        0x09, 0x35,       // Usage: Rz

        0x75, 0x0C,       // Report Size: 12 bits
        0x95, 0x02,       // Report Count: 2

        0x81, 0x02,       // Input: Data, Variable, Absolute


        // ====================================================
        // PADDING
        // ====================================================
        //
        // Reservamos 8 bits.
        //
        // Esto nos permite mantener una estructura alineada
        // mientras seguimos construyendo los campos posteriores.
        //
        // ----------------------------------------------------

        0x75, 0x08,       // Report Size: 8 bits
        0x95, 0x01,       // Report Count: 1

        0x81, 0x03,       // Input: Constant, Variable, Absolute


        // ====================================================
        // END COLLECTION
        // ====================================================

        0xC0
    };


    // ========================================================
    // REPORT MAP SIZE
    // ========================================================

    const uint16_t ReportMapSize =
        sizeof(ReportMap);

}