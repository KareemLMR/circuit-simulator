#include "Orchestrator.h"

constexpr double MAX_USER_INTERFACE_FREQUENCY_SUPPORTED = 1000.0;

std::unique_ptr<Orchestrator> Orchestrator::m_instance = nullptr;

Orchestrator::Orchestrator()
{
    m_userInterfaceClockFrequency = 1.0;
    m_userInterfaceClockFrequency = 1.0;
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
//    if (m_isInitialized)
//    {
//        qDebug() << "Orchestrator is already initialized!" ;
//    }
//    else
//    {
        if (circuitManager == nullptr)
        {
            qDebug() << "Circuit Manager is not initialized!" ;
            return false;
        }
        if (userInterfaceClockFrequency > circuitRefreshFrequency)
        {
            qDebug() << "circuitRefreshFrequency must be bigger than userInterfaceClockFrequency" ;
            return false;
        }
        if (userInterfaceClockFrequency <= 0 || circuitRefreshFrequency <= 0)
        {
            qDebug() << "userInterfaceClockFrequency and circuitRefreshFrequency must be positive integers" ;
            return false;
        }
        if (userInterfaceClockFrequency > MAX_USER_INTERFACE_FREQUENCY_SUPPORTED)
        {
            qDebug() << "userInterfaceClockFrequency provided is bigger than supported maximum value of " << MAX_USER_INTERFACE_FREQUENCY_SUPPORTED ;
            return false;
        }

        m_userInterfaceClockFrequency = userInterfaceClockFrequency;
        m_circuitRefreshFrequency = circuitRefreshFrequency;
        m_circuitManager = circuitManager;
        m_isInitialized = true;
//    }
    

    return true;
}

bool Orchestrator::start(void)
{
    if (!m_isInitialized)
    {
        qDebug() << "Orchestrator is not initialized!" ;
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
        for (auto& node : m_circuitManager->queryDeviceVoltages("resistor1").second)
        {
            qDebug() << node->getVolt() ;
        }

        for (auto& path : m_circuitManager->queryDeviceCurrents("resistor1"))
        {
            qDebug() << "Node " << QString::fromStdString(path.first->getName()) << " current = " << path.second ;
        }
        std::chrono::duration<double> sleep_time(1.0 / m_userInterfaceClockFrequency);
        std::this_thread::sleep_for(sleep_time);
    }

}

Orchestrator::~Orchestrator()
{
    
}
