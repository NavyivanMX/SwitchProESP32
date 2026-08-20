// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDescriptor.cpp
// Función : Implementación del descriptor HID.
//
// ETAPA 5.6
//
// Este descriptor es TEMPORAL.
// Su función es proporcionar una base HID válida mientras
// construimos el descriptor específico del Pro Controller.
// ============================================================

#include "SwitchProDescriptor.h"


namespace SwitchProDescriptor
{

    // ========================================================
    // HID REPORT MAP
    // ========================================================
    //
    // Descriptor HID mínimo de Game Pad.
    //
    // Todavía NO representa al Nintendo Switch Pro Controller.
    //
    // ========================================================

    const uint8_t ReportMap[] =
    {
        // ----------------------------------------------------
        // Usage Page: Generic Desktop
        // ----------------------------------------------------

        0x05, 0x01,

        // ----------------------------------------------------
        // Usage: Game Pad
        // ----------------------------------------------------

        0x09, 0x05,

        // ----------------------------------------------------
        // Application Collection
        // ----------------------------------------------------

        0xA1, 0x01,

        // ----------------------------------------------------
        // End Collection
        // ----------------------------------------------------

        0xC0
    };


    // ========================================================
    // REPORT MAP SIZE
    // ========================================================

    const uint16_t ReportMapSize =
        sizeof(ReportMap);

}