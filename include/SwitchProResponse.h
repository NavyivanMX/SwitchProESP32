// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProResponse.h
// Función : Construcción de respuestas Nintendo.
//
// ETAPA 5.8.4
//
// Este módulo construye Output/Input Reports utilizados como
// respuesta a los Subcommands del protocolo.
//
// IMPORTANTE:
//
// Esta etapa solamente construye la estructura del paquete.
// Todavía NO lo transmite por Bluetooth.
//
// ============================================================

#pragma once

#include <stdint.h>
#include <stddef.h>


class SwitchProResponse
{
public:

    // ========================================================
    // Constantes
    // ========================================================

    static constexpr size_t ReportSize = 64;


    // ========================================================
    // Constructor
    // ========================================================

    SwitchProResponse();


    // ========================================================
    // Reset
    // ========================================================

    void reset();


    // ========================================================
    // Crear respuesta
    // ========================================================
    //
    // Construye un reporte de respuesta 0x21.
    //
    // subcommand:
    //     comando al que estamos respondiendo.
    //
    // data:
    //     datos adicionales de la respuesta.
    //
    // dataLength:
    //     cantidad de datos adicionales.
    //
    // return:
    //     cantidad total de bytes generados.
    //
    // ========================================================

    size_t build(
        uint8_t subcommand,
        const uint8_t* data,
        size_t dataLength
    );


    // ========================================================
    // Información
    // ========================================================

    const uint8_t* data() const;

    size_t size() const;


private:

    uint8_t m_buffer[ReportSize];

    size_t m_size;
};