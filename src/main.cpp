#include <iostream>
#include "Node.h"
#include <map>
#include "CircuitManager.h"
#include "Resistor.h"

#include <eigen3/Eigen/Dense>

int main()
{
    CircuitManager& cm = CircuitManager::getInstance();
    Node R11 = Node("R11");
    Node R12 = Node("R12");
    Node R21 = Node("R21");
    Node R22 = Node("R22");
    Node R31 = Node("R31");
    Node R32 = Node("R32");
    Node R41 = Node("R41");
    Node R42 = Node("R42");
    Node R51 = Node("R51");
    Node R52 = Node("R52");
    
    Node VB1 = Node("VB1");
    Node VB2 = Node("VB2");

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

    cm.calculateCircuitMatrix();

    
    std::vector<double> A_flat;
    for (const auto& row : cm.getCircuitMatrix())
    {
        A_flat.insert(A_flat.end(), row.begin(), row.end());
    }

    // Step 2: Get rows and columns
    size_t rows = cm.getCircuitMatrix().size();
    size_t cols = (rows > 0) ? cm.getCircuitMatrix()[0].size() : 0;

    // Step 3: Map into Eigen::MatrixXd (RowMajor for correct layout)
    Eigen::MatrixXd A = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(A_flat.data(), rows, cols);
    
    Eigen::VectorXd b = Eigen::Map<Eigen::VectorXd>(cm.getVoltages().data(), cm.getVoltages().size());

    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
    
    // for (auto coefficients : cm.getCircuitMatrix())
    // {
    //     for (auto coefficient : coefficients)
    //     {
    //         std::cout << coefficient << " ";
    //     }
    //     std::cout << std::endl;
    // }
    for (auto voltage : x)
    {
        std::cout << voltage << " ";
    }
    std::cout << "Exiting" << std::endl;
    
    return 0;
}