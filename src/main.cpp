#include <iostream>
#include "Node.h"
#include <map>
#include "CircuitManager.h"
#include "Resistor.h"

int main()
{
    CircuitManager& cm = CircuitManager::getInstance();
    std::shared_ptr<Node> R11 = std::make_shared<Node>(Node("R11"));
    std::shared_ptr<Node> R12 = std::make_shared<Node>(Node("R12"));
    std::shared_ptr<Node> R21 = std::make_shared<Node>(Node("R21"));
    std::shared_ptr<Node> R22 = std::make_shared<Node>(Node("R22"));
    std::shared_ptr<Node> R31 = std::make_shared<Node>(Node("R31"));
    std::shared_ptr<Node> R32 = std::make_shared<Node>(Node("R32"));
    std::shared_ptr<Node> R41 = std::make_shared<Node>(Node("R41"));
    std::shared_ptr<Node> R42 = std::make_shared<Node>(Node("R42"));
    std::shared_ptr<Node> R51 = std::make_shared<Node>(Node("R51"));
    std::shared_ptr<Node> R52 = std::make_shared<Node>(Node("R52"));
    
    std::shared_ptr<Node> VB1 = std::make_shared<Node>(Node("VB1"));
    std::shared_ptr<Node> VB2 = std::make_shared<Node>(Node("VB2"));

    cm.createDevice(DeviceType::VOLTAGE_SOURCE, std::make_pair<std::string, std::vector<double>>("V", {100}), {VB1, VB2});
    
    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r1", {5}), {R11, R12});
    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r2", {10}), {R21, R22});
    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r3", {30}), {R31, R32});
    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r4", {10}), {R41, R42});
    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r5", {20}), {R51, R52});    

    cm.connect(VB1, R11);
    cm.connect(VB2, R32);
    cm.connect(R12, R21);
    cm.connect(R21, R31);
    cm.connect(R22, R41);
    cm.connect(R41, R51);
    cm.connect(R42, R52);
    cm.connect(R32, R52);

    cm.solveCircuit();

    for (auto& node : cm.queryDeviceVoltages("r1").second)
    {
        std::cout << node->getVolt() << std::endl;
    }

    for (auto& path : cm.queryDeviceCurrents("r1"))
    {
        for (double current : path)
        {
            std::cout << current << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}