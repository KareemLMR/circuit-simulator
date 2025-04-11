#include "CircuitManager.h"
#include <iostream>

std::unique_ptr<CircuitManager> CircuitManager::m_instance = nullptr;


CircuitManager::CircuitManager()
{
}

CircuitManager& CircuitManager::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = std::make_unique<CircuitManager>();
    }
    return *m_instance;
}

int CircuitManager::getDeviceTypeTerminals(DeviceType type)
{
    switch (type)
    {
        case DeviceType::RESISTOR:
        case DeviceType::CAPACITOR:
        case DeviceType::INDUCTOR:
        case DeviceType::DIODE:
        case DeviceType::VOLTAGE_SOURCE:
        case DeviceType::CURRENT_SOURCE:
            return 2;
        case DeviceType::TRANSISTOR:
            return 3;
    }
    return 0;
}


bool CircuitManager::createDevice(DeviceType type, std::pair<std::string, std::vector<double>> deviceCharacteristics, std::vector<Node> pins)
{
    if (m_devices.find(deviceCharacteristics.first) != m_devices.end())
    {
        std::cout << "Device already exists" << std::endl;
        return false;
    }
    if (pins.size() != 0 && pins.size() != getDeviceTypeTerminals(type))
    {
        std::cout << "Wrong number of pins" << std::endl;
        return false;
    }
    switch (type)
    {
        case DeviceType::RESISTOR:
        {
            std::cout << "Creating a resistor" << std::endl;
            std::shared_ptr<Resistor> r = std::make_shared<Resistor>();
            r->setName(deviceCharacteristics.first);
            r->setResistance(deviceCharacteristics.second[0]);
            m_devices[deviceCharacteristics.first] = std::move(r);
            break;
        }
        
        case DeviceType::VOLTAGE_SOURCE:
        {
            std::cout << "Creating a voltage source" << std::endl;
            std::shared_ptr<VoltageSource> vs = std::make_shared<VoltageSource>();
            vs->setName(deviceCharacteristics.first);
            vs->setVoltage(deviceCharacteristics.second[0]);
            m_devices[deviceCharacteristics.first] = std::move(vs);
            break;
        }
            
        default:
            std::cout << "Invalid device type" << std::endl;
            return false;
    }
    int index = 0;
    for (auto& node : m_devices[deviceCharacteristics.first]->getPins())
    {
        if (pins.size() != 0)
        {
            node = pins[index];
        }
        m_pinOf[node] = m_devices[deviceCharacteristics.first];
        m_connected[node] = node;
        index++;
    }
    return true;
}

bool CircuitManager::connect(const Node& node1, const Node& node2)
{
    std::cout << "Try connecting" << std::endl;
    if (m_connected.find(node1) == m_connected.end() || m_connected.find(node2) == m_connected.end())
    {
        std::cout << "Requested to connect invalid nodes!" << std::endl;
        return false;
    }
    if (m_pinOf[node1]->isSource() || m_pinOf[node2]->isSource())
    {
        std::cout << "Delay connecting voltage source" << std::endl;
        std::pair<Node, Node> toBeConnected({node1, node2});
        m_sourceNodes.push_back(toBeConnected);
    }
    else
    {
        Node root2 = m_connected[node2];
        Node root1 = m_connected[node1];
        for (auto& node : m_connected)
        {
            if (node.second == root1)
            {
                node.second = root2;
            }
        }
    }
    
    return true;
}

bool CircuitManager::isConnected(const Node& node1, const Node& node2)
{
    return m_connected[node1] == m_connected[node2];
}

std::vector<std::shared_ptr<Device>> CircuitManager::getAdjacentDevices(const Node& node)
{
    std::vector<std::shared_ptr<Device>> adjDevices;
    for (auto& currNode : m_connected)
    {
        if (isConnected(node, currNode.first))
        {
            adjDevices.push_back(m_pinOf[currNode.first]);
        }
    }
    return adjDevices;
}

std::set<Node> CircuitManager::getUniqueNodes()
{
    std::set<Node> uniqueNodes;
    for (auto& node : m_connected)
    {
        if (!m_pinOf[node.first]->isSource())
        {
            uniqueNodes.insert(node.second);
        }
    }
    return uniqueNodes;
}

void CircuitManager::initializeCircuitMatrix(std::map<Node, std::vector<double>>& circuitMatrix)
{
    std::set<Node> uniqueNodes = getUniqueNodes();
    for (auto& node : uniqueNodes)
    {
        for (int i = 0 ; i < uniqueNodes.size() ; i++)
        {
            circuitMatrix[node].push_back(0.0);
        }
    }
}

std::unique_ptr<Node> CircuitManager::findWhichNodeConnected(const Node& node, const Device& device)
{
    std::unique_ptr<Node> foundNode;
    for (auto& n : device.getPins())
    {
        if (isConnected(node, n))
        {
            foundNode = std::make_unique<Node>(n);
            return foundNode;
        }
    }
    return nullptr;
}

std::unique_ptr<Node> CircuitManager::findWhichNodeConnected(const Node& node, const std::set<Node>& nodes)
{
    std::unique_ptr<Node> foundNode;
    for (auto& n : nodes)
    {
        if (isConnected(node, n))
        {
            foundNode = std::make_unique<Node>(n);
            return foundNode;
        }
    }
    return nullptr;
}

void CircuitManager::connectSources(std::map<Node, std::vector<double>>& circuitMatrix, std::map<Node, int>& indexMap)
{
    std::set<Node> uniqueNodes = getUniqueNodes();
    std::map<std::shared_ptr<Device>, std::vector<std::pair<Node, Node>>> rowsToBeAdded;
    for (auto& sourceNode : m_sourceNodes)
    {
        std::unique_ptr<Node> foundNode = nullptr;
        if (!m_pinOf[sourceNode.first]->isSource())
        {
            foundNode = findWhichNodeConnected(sourceNode.first, uniqueNodes);
            if (foundNode != nullptr)
            {
                rowsToBeAdded[m_pinOf[sourceNode.second]].push_back(std::pair<Node, Node>(sourceNode.second, *foundNode));
            }
            else
            {
                std::cout << "Invalid node found! " << sourceNode.first.getName() << std::endl;
            }
        }
        else if (!m_pinOf[sourceNode.second]->isSource())
        {
            foundNode = findWhichNodeConnected(sourceNode.second, uniqueNodes);
            if (foundNode != nullptr)
            {
                rowsToBeAdded[m_pinOf[sourceNode.first]].push_back(std::pair<Node, Node>(sourceNode.first, *foundNode));
            }
            else
            {
                std::cout << "Invalid node found! " << sourceNode.second.getName() << std::endl;
            }
        }
        else
        {
            std::cout << "Rejecting a source connection!" << std::endl;
        }
    }
    m_voltages.resize(uniqueNodes.size());
    for (auto& device : rowsToBeAdded)
    {
        //TODO: Check that line again!
        int maxIndex = std::max(indexMap[device.second[0].second], indexMap[device.second[1].second]);
        int minIndex = std::min(indexMap[device.second[0].second], indexMap[device.second[1].second]);
        auto maxNode = device.second[1];
        auto minNode = device.second[0];
        if (maxIndex == indexMap[device.second[0].second])
        {
            maxNode = device.second[0];
            minNode = device.second[1];
        }

        m_voltages[maxIndex - 1] = device.first->getVoltage(minNode.first);
        for (int i = 0 ; i < uniqueNodes.size() ; i++)
        {
            if (i == minIndex)
            {
                circuitMatrix[maxNode.second][i] = 1;
            }
            else if (i == maxIndex)
            {
                circuitMatrix[maxNode.second][i] = -1;
            }
            else
            {
                circuitMatrix[maxNode.second][i] = 0;
            }
        }
        circuitMatrix.erase(minNode.second);
    }
    // for (auto& v: voltagesVector)
    // {
    //     std::cout << v << " ";
    // }
    // std::cout << std::endl;
    // return voltagesVector;
}

void CircuitManager::calculateCircuitMatrix()
{
    std::map<Node, std::vector<double>> circuitMatrix;
    initializeCircuitMatrix(circuitMatrix);
    std::set<Node> uniqueNodes = getUniqueNodes();
    std::map<Node, int> indexMap;
    int index = 0;
    for (auto& node : uniqueNodes)
    {
        indexMap[node] = index;
    
        for (auto& device : getAdjacentDevices(node))
        {
            std::unique_ptr<Node> deviceNode = findWhichNodeConnected(node, *device);
            std::map<Node, double> currentCoefficients = device->getCurrentCoefficients(*deviceNode);
            for (auto& n : currentCoefficients)
            {
                std::unique_ptr<Node> foundNode = findWhichNodeConnected(n.first, uniqueNodes);
                if (foundNode != nullptr)
                {
                    circuitMatrix[*foundNode][index] += n.second;
                }
                else
                {
                    std::cout << "Invalid node found! " << n.first.getName() << std::endl;
                }
            }
        }
        index++;
    }
    std::cout << "Calling connectSources" << std::endl;
    connectSources(circuitMatrix, indexMap);
    std::cout << "connectSources returned" << std::endl;
    

    // Convert the map to a vector of vectors
    std::vector<double> groundVector;
    for (const auto& node : uniqueNodes)
    {
        groundVector.push_back(0.0);
        m_circuitMatrix.push_back(circuitMatrix[node]);
    }
    groundVector[uniqueNodes.size() - 1] = 1.0;
    m_circuitMatrix.push_back(groundVector);
    std::cout << "Circuit matrix:" << std::endl;
    for (const auto& row : m_circuitMatrix)
    {
        for (const auto& element : row)
        {
            std::cout << element << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "Voltages:" << std::endl;
    for (const auto& voltage : m_voltages)
    {
        std::cout << voltage << " ";
    }
    std::cout << std::endl;
}

CircuitManager::~CircuitManager()
{
    m_instance = nullptr;
}