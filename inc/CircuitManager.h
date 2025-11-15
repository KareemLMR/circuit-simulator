#ifndef CIRCUITMANAGER_H_
#define CIRCUITMANAGER_H_

#include "TwoTerminal.h"
#include "Resistor.h"
#include "Capacitor.h"
#include "Inductor.h"
#include "VoltageSource.h"
#include "CurrentSource.h"
#include <map>
#include <memory>
#include <utility>
#include <set>
#include <eigen3/Eigen/Dense>
#include <numeric> 
#include "ICircuitManager.h"

class CircuitManager : public ICircuitManager
{
    public:
        CircuitManager();
        CircuitManager(const CircuitManager& other) = delete;
        CircuitManager(const CircuitManager&& other) = delete;

        CircuitManager& operator=(const CircuitManager& other) = delete;
        CircuitManager& operator=(const CircuitManager&& other) = delete;

        static CircuitManager& getInstance(void);
        static int getDeviceTypeTerminals(DeviceType type);

        bool createDevice(DeviceType type,
                          const std::pair<std::string, std::vector<double>>& deviceCharacteristics,
                          const std::vector<std::shared_ptr<Node>>& pins = {}) override;

        bool connect(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) override;
        bool isConnected(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) override;
        std::vector<std::shared_ptr<Device>> getAdjacentDevices(const std::shared_ptr<Node>& node);
        std::set<std::shared_ptr<Node>> getUniqueNodes(void);
        void initializeCircuitMatrix(std::map<std::shared_ptr<Node>, std::vector<double>>& circuitMatrix);
        std::vector<std::shared_ptr<Node>> findAllNodesConnected(const std::shared_ptr<Node>& node);
        std::shared_ptr<Node> findWhichNodeConnected(const std::shared_ptr<Node>& node, const Device& device);
        std::shared_ptr<Node> findWhichNodeConnected(const std::shared_ptr<Node>& node, const std::set<std::shared_ptr<Node>>& nodes);
        std::shared_ptr<Node> findWhichNodeConnected(const std::shared_ptr<Node>& node);
        int getDeviceNodeIndex(const std::shared_ptr<Node>& node, const Device& device);

        std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> checkSourcesConnected(const std::shared_ptr<Node>& node,
                                                                                                        std::vector<std::shared_ptr<Device>>& adjacentDevices,
                                                                                                        const std::set<std::shared_ptr<Node>>& nodes);
        std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> checkSourcesConnected(const std::shared_ptr<Node>& node,
                                                                                                        std::vector<std::shared_ptr<Device>>& adjacentDevices);
        
        void calculateCircuitMatrix(double deltaT);

        Eigen::MatrixXd convert2DVectorToMatrix(const std::vector<std::vector<double>>& vect);
        std::vector<std::vector<double>> convertMatrixTo2DVector(const Eigen::MatrixXd& matrix);

        void solveCircuit(double deltaT) override;

        std::vector<std::vector<double>>& getCircuitMatrix() { return m_circuitMatrix; }
        std::vector<double>& getVoltages() { return m_voltages; }
        
        std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> queryDeviceVoltages(std::string deviceName) override;
        std::map<std::shared_ptr<Node>, double> queryDeviceCurrents(std::string deviceName) override;

        void applyKCL(void);
        void arrangeSources(std::map<std::shared_ptr<Device>, std::shared_ptr<Node>>& outerSources);

        ~CircuitManager();

    private:
        std::map<std::shared_ptr<Node>, std::shared_ptr<Device>> m_pinOf;
        std::map<std::shared_ptr<Node>, std::shared_ptr<Node>> m_connected;
        std::set<std::shared_ptr<Node>> m_uniqueNodes;
        static std::unique_ptr<CircuitManager> m_instance;
        std::map<std::string, std::shared_ptr<Device>> m_devices;
        std::vector<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>> m_sourceNodes;
        std::vector<std::vector<double>> m_circuitMatrix;
        std::vector<double> m_voltages;
        std::shared_ptr<Node> m_groundNode;
};

#endif