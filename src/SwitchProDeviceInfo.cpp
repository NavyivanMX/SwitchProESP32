// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDeviceInfo.cpp
// Función : Implementación de información del dispositivo.
//
// ETAPA 5.8.6
// ============================================================

#include "SwitchProDeviceInfo.h"

#include <string.h>


// ============================================================
// Información estática
// ============================================================

namespace
{
    constexpr const char* DeviceName =
        "SwitchProESP32";

    constexpr const char* SerialNumber =
        "ESP32-SWITCH-PRO";
}


// ============================================================
// deviceName()
// ============================================================

const char*
SwitchProDeviceInfo::deviceName()
{
    return DeviceName;
}


// ============================================================
// serialNumber()
// ============================================================

const char*
SwitchProDeviceInfo::serialNumber()
{
    return SerialNumber;
}


// ============================================================
// build()
// ============================================================

size_t
SwitchProDeviceInfo::build(
    uint8_t* buffer,
    size_t bufferSize
)
{
    // --------------------------------------------------------
    // Validación
    // --------------------------------------------------------

    if (
        buffer == nullptr ||
        bufferSize == 0
    )
    {
        return 0;
    }


    // --------------------------------------------------------
    // Limpiar buffer
    // --------------------------------------------------------

    memset(
        buffer,
        0,
        bufferSize
    );


    // ========================================================
    // Construcción de información
    // ========================================================
    //
    // Esta estructura es deliberadamente pequeña en esta
    // etapa.
    //
    // La respuesta exacta del Subcommand será construida por
    // el handler correspondiente en una etapa posterior.
    //
    // ========================================================

    size_t offset = 0;


    // --------------------------------------------------------
    // Vendor ID
    // --------------------------------------------------------

    if (offset + 2 <= bufferSize)
    {
        buffer[offset++] =
            static_cast<uint8_t>(
                VendorId & 0xFF
            );

        buffer[offset++] =
            static_cast<uint8_t>(
                (VendorId >> 8) & 0xFF
            );
    }


    // --------------------------------------------------------
    // Product ID
    // --------------------------------------------------------

    if (offset + 2 <= bufferSize)
    {
        buffer[offset++] =
            static_cast<uint8_t>(
                ProductId & 0xFF
            );

        buffer[offset++] =
            static_cast<uint8_t>(
                (ProductId >> 8) & 0xFF
            );
    }


    // --------------------------------------------------------
    // Hardware version
    // --------------------------------------------------------

    if (offset + 2 <= bufferSize)
    {
        buffer[offset++] =
            HardwareVersionMajor;

        buffer[offset++] =
            HardwareVersionMinor;
    }


    // --------------------------------------------------------
    // Firmware version
    // --------------------------------------------------------

    if (offset + 2 <= bufferSize)
    {
        buffer[offset++] =
            FirmwareVersionMajor;

        buffer[offset++] =
            FirmwareVersionMinor;
    }


    return offset;
}