#ifndef ICIRCUITMANAGER_H_
#define ICIRCUITMANAGER_H_

#include <memory>
#include "Node.h"
#include <vector>
#include "Device.h"

class ICircuitManager
{
    public:
        ICircuitManager(){};
        ~ICircuitManager(){};

        virtual bool createDevice(std::string type,
                                  const std::pair<std::string, std::vector<double>>& deviceCharacteristics,
                                  const std::vector<std::shared_ptr<Node>>& pins = {}) = 0;

        virtual bool connect(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) = 0;
        virtual bool isConnected(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) = 0;
        virtual void solveCircuit(double deltaT) = 0;
        virtual std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> queryDeviceVoltages(std::string deviceName) = 0;
        virtual std::map<std::shared_ptr<Node>, double> queryDeviceCurrents(std::string deviceName) = 0;

};

#endif