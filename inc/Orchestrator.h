#ifndef ORCHESTRATOR_H_
#define ORCHESTRATOR_H_

#include <memory>
#include <iostream>
#include <thread>

class Orchestrator
{
    public:
        Orchestrator();

        Orchestrator(const Orchestrator& other) = delete;
        Orchestrator(const Orchestrator&& other) = delete;

        Orchestrator& operator=(const Orchestrator& other) = delete;
        Orchestrator& operator=(const Orchestrator&& other) = delete;

        bool init(double userInterfaceClockFrequency, double circuitRefreshFrequency);

        static Orchestrator& getInstance(void);

        ~Orchestrator();

    private:
        static std::unique_ptr<Orchestrator> m_instance;
        double m_userInterfaceClockFrequency;
        double m_circuitRefreshFrequency;
        std::thread m_userInterfaceThread;
};

#endif