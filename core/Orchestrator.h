#ifndef ORCHESTRATOR_H_
#define ORCHESTRATOR_H_

#include <memory>
#include <iostream>
#include <thread>
#include <atomic>
#include "ICircuitManager.h"
#include <functional>
#include <QDebug>

class Orchestrator
{
    public:
        Orchestrator();

        Orchestrator(const Orchestrator& other) = delete;
        Orchestrator(const Orchestrator&& other) = delete;

        Orchestrator& operator=(const Orchestrator& other) = delete;
        Orchestrator& operator=(const Orchestrator&& other) = delete;

        bool init(double userInterfaceClockFrequency, double circuitRefreshFrequency, ICircuitManager* circuitManager);
        bool start(void);
        void orchestrate(void);

        static Orchestrator& getInstance(void);

        ~Orchestrator();

    private:
        static std::unique_ptr<Orchestrator> m_instance;
        double m_userInterfaceClockFrequency;
        double m_circuitRefreshFrequency;
        std::thread m_userInterfaceThread;
        ICircuitManager* m_circuitManager;
        std::atomic<bool> m_isInitialized;
};

#endif
