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

bool Orchestrator::init(double userInterfaceClockFrequency, double circuitRefreshFrequency, ICircuitManager* circuitManager)
{
    if (m_isInitialized)
    {
        std::cout << "Orchestrator is already initialized!" << std::endl;
    }
    else
    {
        if (circuitManager == nullptr)
        {
            std::cout << "Circuit Manager is not initialized!" << std::endl;
            return false;
        }
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
        m_circuitManager = circuitManager;
        m_isInitialized = true;
    }
    

    return true;
}

bool Orchestrator::start(void)
{
    if (!m_isInitialized)
    {
        std::cout << "Orchestrator is not initialized!" << std::endl;
        return false;
    }
    m_userInterfaceThread = std::thread((std::bind(&Orchestrator::orchestrate, this)));
    m_userInterfaceThread.detach();
    return true;
}

void Orchestrator::orchestrate(void)
{
    while (true)
    {
        m_circuitManager->solveCircuit(1.0 / m_circuitRefreshFrequency);
        for (auto& node : m_circuitManager->queryDeviceVoltages("c1").second)
        {
            std::cout << node->getVolt() << std::endl;
        }

        // for (auto& path : m_circuitManager->queryDeviceCurrents("V"))
        // {
        //     for (double current : path)
        //     {
        //         std::cout << current << " ";
        //     }
        //     std::cout << std::endl;
        // }
        std::chrono::duration<double> sleep_time(1.0 / m_userInterfaceClockFrequency);
        std::this_thread::sleep_for(sleep_time);
    }

}

Orchestrator::~Orchestrator()
{
    
}

