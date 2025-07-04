#ifndef ICIRCUITMANAGER_H_
#define ICIRCUITMANAGER_H_

#include <memory>
#include "Node.h"
#include <vector>
#include "Device.h"

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

class ICircuitManager
{
    public:
        ICircuitManager(){};
        ~ICircuitManager(){};

        virtual bool createDevice(DeviceType type,
                                  const std::pair<std::string, std::vector<double>>& deviceCharacteristics,
                                  const std::vector<std::shared_ptr<Node>>& pins = {}) = 0;

        virtual bool connect(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) = 0;
        virtual bool isConnected(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2) = 0;
        virtual void solveCircuit(void) = 0;
        virtual std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> queryDeviceVoltages(std::string deviceName) = 0;
        virtual std::vector<std::vector<double>> queryDeviceCurrents(std::string deviceName) = 0;

};

#endif