// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDeviceInfo.h
// Función : Información básica del controlador.
//
// ETAPA 5.8.6
//
// Este módulo mantiene los datos de identificación que serán
// utilizados por el protocolo Switch Pro Controller.
//
// IMPORTANTE:
//
// Aquí solamente definimos los datos.
// La transmisión Bluetooth se implementará posteriormente.
//
// ============================================================

#pragma once

#include <stdint.h>
#include <stddef.h>


class SwitchProDeviceInfo
{
public:

    // ========================================================
    // Identidad del dispositivo
    // ========================================================

    static constexpr uint16_t VendorId  = 0x057E;
    static constexpr uint16_t ProductId = 0x2009;


    // ========================================================
    // Versiones
    // ========================================================

    static constexpr uint8_t HardwareVersionMajor = 1;
    static constexpr uint8_t HardwareVersionMinor = 0;

    static constexpr uint8_t FirmwareVersionMajor = 1;
    static constexpr uint8_t FirmwareVersionMinor = 0;


    // ========================================================
    // Nombre
    // ========================================================

    static const char* deviceName();


    // ========================================================
    // Serial
    // ========================================================

    static const char* serialNumber();


    // ========================================================
    // Construcción de información
    // ========================================================

    static size_t build(
        uint8_t* buffer,
        size_t bufferSize
    );
};