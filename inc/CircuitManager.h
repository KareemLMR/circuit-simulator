#ifndef CIRCUITMANAGER_H_
#define CIRCUITMANAGER_H_

#include "TwoTerminal.h"
#include "Resistor.h"
#include "VoltageSource.h"
#include <map>
#include <memory>
#include <set>

enum class DeviceType
{
    RESISTOR,
    CAPACITOR,
    INDUCTOR,
    DIODE,
    TRANSISTOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE
};


class CircuitManager
{
    public:
        CircuitManager();
        CircuitManager(const CircuitManager& other) = delete;
        CircuitManager(const CircuitManager&& other) = delete;

        CircuitManager& operator=(const CircuitManager& other) = delete;
        CircuitManager& operator=(const CircuitManager&& other) = delete;

        static CircuitManager& getInstance();
        static int getDeviceTypeTerminals(DeviceType type);

        bool createDevice(DeviceType type, std::pair<std::string, std::vector<double>> deviceCharacteristics, std::vector<Node> pins = {});

        bool connect(const Node& node1, const Node& node2);
        bool isConnected(const Node& node1, const Node& node2);
        std::vector<std::shared_ptr<Device>> getAdjacentDevices(const Node& node);
        std::set<Node> getUniqueNodes();
        void initializeCircuitMatrix(std::map<Node, std::vector<double>>& circuitMatrix);
        std::unique_ptr<Node> findWhichNodeConnected(const Node& node, const Device& device);
        std::unique_ptr<Node> findWhichNodeConnected(const Node& node, const std::set<Node>& nodes);
        std::map<std::pair<Node, Node>, double> checkSourcesConnected(const Node& node,
                                                                      std::vector<std::shared_ptr<Device>>& adjacentDevices,
                                                                      const std::set<Node>& nodes);
        void calculateCircuitMatrix();

        std::vector<std::vector<double>>& getCircuitMatrix() { return m_circuitMatrix; }
        std::vector<double>& getVoltages() { return m_voltages; }
        

        ~CircuitManager();

    private:
        std::map<Node, std::shared_ptr<Device>> m_pinOf;
        std::map<Node, Node> m_connected;
        static std::unique_ptr<CircuitManager> m_instance;
        std::map<std::string, std::shared_ptr<Device>> m_devices;
        std::vector<std::pair<Node, Node>> m_sourceNodes;
        std::vector<std::vector<double>> m_circuitMatrix;
        std::vector<double> m_voltages;
};

#endif