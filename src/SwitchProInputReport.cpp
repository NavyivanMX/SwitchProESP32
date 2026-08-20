// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProInputReport.cpp
// Función : Implementación del HID Input Report.
//
// ETAPA 5.8.10
//
// Formato Nintendo Pro Controller 0x30.
//
// ============================================================

#include "SwitchProInputReport.h"
#include "esp_log.h"
#include <string.h>


static const char* TAG_INPUT_REPORT = "SwitchProInputReport";
// ============================================================
// Constructor
// ============================================================

SwitchProInputReport::SwitchProInputReport()
: m_data{},
m_size(0)
{
}

// ============================================================
// CLEAR
// ============================================================

void SwitchProInputReport::clear()
{
memset(
m_data,
0,
sizeof(m_data)
);

m_size = 0;

}

// ============================================================
// BUILD
// ============================================================

size_t SwitchProInputReport::build(
    const SwitchProControllerState& state
)
{
    (void)state;

    clear();

    m_data[0] = ReportId;

    m_data[1] = 0x00;

    m_data[2] = 0x90;

    // Botones
    m_data[3] = 0x00;
    m_data[4] = 0x00;
    m_data[5] = 0x00;

    // Stick izquierdo centrado
    const uint16_t leftX = 0x800;
    const uint16_t leftY = 0x800;

    m_data[6] =
        static_cast<uint8_t>(leftX & 0xFF);

    m_data[7] =
        static_cast<uint8_t>(
            ((leftX >> 8) & 0x0F) |
            ((leftY & 0x0F) << 4)
        );

    m_data[8] =
        static_cast<uint8_t>(
            (leftY >> 4) & 0xFF
        );

    // Stick derecho centrado
    const uint16_t rightX = 0x800;
    const uint16_t rightY = 0x800;

    m_data[9] =
        static_cast<uint8_t>(rightX & 0xFF);

    m_data[10] =
        static_cast<uint8_t>(
            ((rightX >> 8) & 0x0F) |
            ((rightY & 0x0F) << 4)
        );

    m_data[11] =
        static_cast<uint8_t>(
            (rightY >> 4) & 0xFF
        );

    // Reporte completo
    m_size = ReportSize;

ESP_LOGI(
    TAG_INPUT_REPORT,
    "BUILD OK: m_size=%u ReportSize=%u",
    static_cast<unsigned>(m_size),
    static_cast<unsigned>(ReportSize)
);

return m_size;
}

// ============================================================
// ENCODE BUTTONS
// ============================================================

void SwitchProInputReport::encodeButtons(
const SwitchProControllerState& state
)
{
// --------------------------------------------------------
// IMPORTANTE
//
// Esta sección depende de los nombres exactos que tenga
// SwitchProControllerState.
//
// Para evitar romper la compilación si todavía estamos
// desarrollando esa estructura, inicialmente dejamos
// los bytes en cero.
//
// La estructura HID ya queda lista para que después
// conectemos los botones reales.
// --------------------------------------------------------

(void)state;

m_data[3] = 0x00;
m_data[4] = 0x00;
m_data[5] = 0x00;

}

// ============================================================
// ENCODE STICKS
// ============================================================

void SwitchProInputReport::encodeSticks(
const SwitchProControllerState& state
)
{
// --------------------------------------------------------
// Igual que con los botones:
//
// primero queremos conseguir que la Switch acepte
// el dispositivo.
//
// Los sticks se conectarán después con los nombres
// reales de SwitchProControllerState.
// --------------------------------------------------------

(void)state;


// --------------------------------------------------------
// Stick izquierdo
//
// Centro aproximado:
//   X = 0x800
//   Y = 0x800
//
// 12 bits por eje.
// --------------------------------------------------------

const uint16_t leftX = 0x800;
const uint16_t leftY = 0x800;


m_data[6] =
    static_cast<uint8_t>(
        leftX & 0xFF
    );

m_data[7] =
    static_cast<uint8_t>(
        ((leftX >> 8) & 0x0F) |
        ((leftY & 0x0F) << 4)
    );

m_data[8] =
    static_cast<uint8_t>(
        (leftY >> 4) & 0xFF
    );


// --------------------------------------------------------
// Stick derecho
// --------------------------------------------------------

const uint16_t rightX = 0x800;
const uint16_t rightY = 0x800;


m_data[9] =
    static_cast<uint8_t>(
        rightX & 0xFF
    );

m_data[10] =
    static_cast<uint8_t>(
        ((rightX >> 8) & 0x0F) |
        ((rightY & 0x0F) << 4)
    );

m_data[11] =
    static_cast<uint8_t>(
        (rightY >> 4) & 0xFF
    );

}

// ============================================================
// DATA
// ============================================================

const uint8_t* SwitchProInputReport::data() const
{
return m_data;
}

// ============================================================
// MUTABLE DATA
// ============================================================

uint8_t* SwitchProInputReport::mutableData()
{
return m_data;
}

// ============================================================
// SIZE
// ============================================================

size_t SwitchProInputReport::size() const
{
return m_size;
}
