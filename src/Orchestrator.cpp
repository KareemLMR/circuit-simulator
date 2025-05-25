#include "Orchestrator.h"

constexpr double MAX_USER_INTERFACE_FREQUENCY_SUPPORTED = 1000.0;

std::unique_ptr<Orchestrator> Orchestrator::m_instance = nullptr;

Orchestrator::Orchestrator()
{
}

Orchestrator& Orchestrator::getInstance(void)
{
    if (m_instance == nullptr)
    {
        m_instance = std::make_unique<Orchestrator>();
    }
    return *m_instance;
}

bool Orchestrator::init(double userInterfaceClockFrequency, double circuitRefreshFrequency)
{
    if (userInterfaceClockFrequency > circuitRefreshFrequency)
    {
        std::cout << "circuitRefreshFrequency must be bigger than userInterfaceClockFrequency" << std::endl;
        return false;
    }
    if (userInterfaceClockFrequency <= 0 || circuitRefreshFrequency <= 0)
    {
        std::cout << "userInterfaceClockFrequency and circuitRefreshFrequency must be positive integers" << std::endl;
        return false;
    }
    if (userInterfaceClockFrequency > MAX_USER_INTERFACE_FREQUENCY_SUPPORTED)
    {
        std::cout << "userInterfaceClockFrequency provided is bigger than supported maximum value of " << MAX_USER_INTERFACE_FREQUENCY_SUPPORTED << std::endl;
        return false;
    }
    m_userInterfaceClockFrequency = userInterfaceClockFrequency;
    m_circuitRefreshFrequency = circuitRefreshFrequency;

    return true;
}

