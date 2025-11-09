#include <iostream>
#include "Node.h"
#include <map>
#include "CircuitManager.h"
#include "Orchestrator.h"
#include "Resistor.h"

int main()
{
    CircuitManager& cm = CircuitManager::getInstance();
    // std::shared_ptr<Node> R11 = std::make_shared<Node>(Node("R11"));
    // std::shared_ptr<Node> R12 = std::make_shared<Node>(Node("R12"));
    // std::shared_ptr<Node> R21 = std::make_shared<Node>(Node("R21"));
    // std::shared_ptr<Node> R22 = std::make_shared<Node>(Node("R22"));
    // std::shared_ptr<Node> R31 = std::make_shared<Node>(Node("R31"));
    // std::shared_ptr<Node> R32 = std::make_shared<Node>(Node("R32"));
    // std::shared_ptr<Node> R41 = std::make_shared<Node>(Node("R41"));
    // std::shared_ptr<Node> R42 = std::make_shared<Node>(Node("R42"));
    // std::shared_ptr<Node> R51 = std::make_shared<Node>(Node("R51"));
    // std::shared_ptr<Node> R52 = std::make_shared<Node>(Node("R52"));
    
    // std::shared_ptr<Node> VB1 = std::make_shared<Node>(Node("VB1"));
    // std::shared_ptr<Node> VB2 = std::make_shared<Node>(Node("VB2"));

    // cm.createDevice(DeviceType::VOLTAGE_SOURCE, std::make_pair<std::string, std::vector<double>>("V", {100}), {VB1, VB2});
    
    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r1", {5}), {R11, R12});
    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r2", {10}), {R21, R22});
    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r3", {30}), {R31, R32});
    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r4", {10}), {R41, R42});
    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r5", {20}), {R51, R52});    

    // cm.connect(VB1, R11);
    // cm.connect(VB2, R32);
    // cm.connect(R12, R21);
    // cm.connect(R21, R31);
    // cm.connect(R22, R41);
    // cm.connect(R41, R51);
    // cm.connect(R42, R52);
    // cm.connect(R32, R52);

    // std::shared_ptr<Node> I1 = std::make_shared<Node>(Node("I1"));
    // std::shared_ptr<Node> I2 = std::make_shared<Node>(Node("I2"));
    // std::shared_ptr<Node> R1 = std::make_shared<Node>(Node("R1"));
    // std::shared_ptr<Node> R2 = std::make_shared<Node>(Node("R2"));

    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r1", {1e3}), {R1, R2});
    // cm.createDevice(DeviceType::CURRENT_SOURCE, std::make_pair<std::string, std::vector<double>>("i1", {1e-3}), {I1, I2});

    // cm.connect(I1, R1);
    // cm.connect(I2, R2);

    // std::shared_ptr<Node> R1 = std::make_shared<Node>(Node("R1"));
    // std::shared_ptr<Node> R2 = std::make_shared<Node>(Node("R2"));
    // std::shared_ptr<Node> C1 = std::make_shared<Node>(Node("C1"));
    // std::shared_ptr<Node> C2 = std::make_shared<Node>(Node("C2"));
    // std::shared_ptr<Node> V1 = std::make_shared<Node>(Node("V1"));
    // std::shared_ptr<Node> V2 = std::make_shared<Node>(Node("V2"));

    // cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r1", {1e3}), {R1, R2});
    // cm.createDevice(DeviceType::CAPACITOR, std::make_pair<std::string, std::vector<double>>("c1", {1e-6}), {C1, C2});
    // cm.createDevice(DeviceType::CURRENT_SOURCE, std::make_pair<std::string, std::vector<double>>("V", {1e-3}), {V1, V2});

    // cm.connect(V1, R1);
    // cm.connect(R2, C1);
    // cm.connect(C2, V2);

    std::shared_ptr<Node> R1 = std::make_shared<Node>(Node("R1"));
    std::shared_ptr<Node> R2 = std::make_shared<Node>(Node("R2"));
    std::shared_ptr<Node> L1 = std::make_shared<Node>(Node("L1"));
    std::shared_ptr<Node> L2 = std::make_shared<Node>(Node("L2"));
    std::shared_ptr<Node> V1 = std::make_shared<Node>(Node("V1"));
    std::shared_ptr<Node> V2 = std::make_shared<Node>(Node("V2"));

    cm.createDevice(DeviceType::RESISTOR, std::make_pair<std::string, std::vector<double>>("r1", {1}), {R1, R2});
    cm.createDevice(DeviceType::INDUCTOR, std::make_pair<std::string, std::vector<double>>("l1", {1e-3}), {L1, L2});
    cm.createDevice(DeviceType::VOLTAGE_SOURCE, std::make_pair<std::string, std::vector<double>>("V", {100}), {V1, V2});

    cm.connect(V2, R1);
    cm.connect(R2, L1);
    cm.connect(L2, V1);


    Orchestrator& oc = Orchestrator::getInstance();
    oc.init(1000.0, 1000000.0, &cm);
    oc.start();

    while (1)
    {

    }
    return 0;
}