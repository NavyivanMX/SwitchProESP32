// ============================================================
// SwitchProESP32
// ------------------------------------------------------------
// Archivo : SwitchProController.cpp
// Función : Orquestador principal del controlador.
// Etapa   : 5.8.12
// ============================================================

#include "SwitchProController.h"

#include "esp_log.h"
#include "esp_timer.h"


// ============================================================
// TAG
// ============================================================

static const char* TAG = "SwitchProController";


// ============================================================
// Constructor
// ============================================================

SwitchProController::SwitchProController()
    : m_state(),
      m_lastReportTime(0),
      m_hid(nullptr)
{
}


// ============================================================
// begin()
// ============================================================

bool
SwitchProController::begin()
{
    ESP_LOGI(
        TAG,
        "========================================"
    );

    ESP_LOGI(
        TAG,
        "SwitchProController"
    );

    ESP_LOGI(
        TAG,
        "Initializing controller..."
    );

    ESP_LOGI(
        TAG,
        "========================================"
    );


    // ========================================================
    // Crear instancia HID
    // ========================================================

    m_hid = new SwitchProHID();


    if (m_hid == nullptr)
    {
        ESP_LOGE(
            TAG,
            "Failed to create SwitchProHID"
        );

        return false;
    }


    // ========================================================
    // Inicializar HID
    // ========================================================

    if (!m_hid->begin())
    {
        ESP_LOGE(
            TAG,
            "SwitchProHID initialization failed"
        );

        delete m_hid;
        m_hid = nullptr;

        return false;
    }


    // ========================================================
    // Inicializar temporizador de reportes
    // ========================================================

    m_lastReportTime =
        static_cast<uint32_t>(
            esp_timer_get_time() / 1000ULL
        );


    ESP_LOGI(
        TAG,
        "SwitchProController: READY"
    );


    return true;
}


// ============================================================
// update()
// ============================================================

void
SwitchProController::update()
{
    // ========================================================
    // Seguridad
    // ========================================================

    if (m_hid == nullptr)
    {
        return;
    }


    // ========================================================
    // Tiempo actual
    // ========================================================

    const uint32_t now = getMillis();


    // ========================================================
    // ¿Ya toca generar un reporte?
    // ========================================================

    if (!reportIntervalElapsed(now))
    {
        return;
    }


    m_lastReportTime = now;


    // ========================================================
    // Generar reporte
    // ========================================================

    updateState();
    updateReport();
}


    // ============================================================
    // updateReport()
    // ============================================================
    //
    // Genera el Input Report utilizando el estado actual.
    //
    // ============================================================

    void
    SwitchProController::updateReport()
    {
        if (m_hid == nullptr)
        {
            return;
        }


        if (!m_hid->sendInputReport(m_state))
        {
            ESP_LOGW(
                TAG,
                "Failed to build HID input report"
            );
        }
    }

// ============================================================
// state()
// ============================================================

const SwitchProControllerState&
SwitchProController::state() const
{
    return m_state;
}

// ============================================================
// reportIntervalElapsed()
// ============================================================

bool
SwitchProController::reportIntervalElapsed(uint32_t now) const
{
    return
        (now - m_lastReportTime) >= ReportIntervalMs;
}

// ============================================================
// getMillis()
// ============================================================
//
// Obtiene el tiempo transcurrido desde el arranque en
// milisegundos.
//
// ============================================================

uint32_t
SwitchProController::getMillis() const
{
    return static_cast<uint32_t>(
        esp_timer_get_time() / 1000ULL
    );
}

// ============================================================
// updateState()
// ============================================================
//
// Actualiza el estado lógico del controlador.
//
// Por ahora no existen entradas físicas conectadas,
// por lo que el estado permanece en su condición actual.
//
// ============================================================

void
SwitchProController::updateState()
{
    // --------------------------------------------------------
    // Etapa actual:
    //
    // No hay entradas físicas todavía.
    //
    // Los botones y joysticks serán conectados aquí
    // posteriormente.
    // --------------------------------------------------------
}

