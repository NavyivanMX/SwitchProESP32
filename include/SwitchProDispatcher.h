// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProDispatcher.h
// Función : Distribución de Subcommands.
//
// ETAPA 5.8.5
//
// Recibe un Subcommand ya interpretado por
// SwitchProProtocol y decide qué respuesta debe construir.
//
// IMPORTANTE:
//
// En esta etapa NO implementamos todavía las respuestas
// específicas del Pro Controller.
//
// ============================================================

#pragma once

#include <stdint.h>
#include <stddef.h>

#include "SwitchProProtocol.h"
#include "SwitchProResponse.h"


class SwitchProDispatcher
{
public:

    // ========================================================
    // Constructor
    // ========================================================

    SwitchProDispatcher();


    // ========================================================
    // Reset
    // ========================================================

    void reset();


    // ========================================================
    // Procesar reporte
    // ========================================================
    //
    // El Dispatcher recibe el reporte completo.
    //
    // Primero lo entrega al protocolo para su interpretación.
    // Después prepara la respuesta correspondiente.
    //
    // return:
    //     true  = reporte reconocido
    //     false = reporte no reconocido
    //
    // ========================================================

    bool process(
        const uint8_t* data,
        size_t length
    );


    // ========================================================
    // Respuesta disponible
    // ========================================================

    bool hasResponse() const;


    // ========================================================
    // Obtener respuesta
    // ========================================================

    const SwitchProResponse& response() const;


private:

    // ========================================================
    // Componentes
    // ========================================================

    SwitchProProtocol m_protocol;

    SwitchProResponse m_response;


    // ========================================================
    // Estado
    // ========================================================

    bool m_hasResponse;
};