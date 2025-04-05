#ifndef CIRCUITMANAGER_H_
#define CIRCUITMANAGER_H_

#include "TwoTerminal.h"
#include <map>

class CircuitManager
{
    public:
        CircuitManager();
        CircuitManager(const CircuitManager& other) = delete;
        CircuitManager(const CircuitManager&& other) = delete;

        CircuitManager& operator=(const CircuitManager& other) = delete;
        CircuitManager& operator=(const CircuitManager&& other) = delete;

        static CircuitManager& getInstance();

        ~CircuitManager();

    private:
        std::map<Node, std::vector<Node>> m_neighbours;
};

#endif