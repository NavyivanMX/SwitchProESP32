// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProHID.h
// Función : Capa de Bluetooth / HID del controlador.
//
// ETAPA 5.4
//   Inicialización de Bluetooth Classic.
//
// En esta etapa:
//
//   - Inicializamos el controlador Bluetooth.
//   - Habilitamos Bluetooth Classic.
//   - Inicializamos Bluedroid.
//   - Habilitamos Bluedroid.
//
// Todavía NO:
//
//   - Inicializamos HID.
//   - Creamos descriptor HID.
//   - Enviamos reportes.
//   - Intentamos conectar con Nintendo Switch.
// ============================================================

#pragma once


class SwitchProHID
{
public:

    // --------------------------------------------------------
    // Inicialización del sistema Bluetooth
    // --------------------------------------------------------

    bool begin();


    // --------------------------------------------------------
    // Actualización
    // --------------------------------------------------------

    void update();


private:

    // --------------------------------------------------------
    // Inicialización del controlador Bluetooth
    // --------------------------------------------------------

    bool initBluetoothController();


    // --------------------------------------------------------
    // Inicialización de Bluedroid
    // --------------------------------------------------------

    bool initBluedroid();
};