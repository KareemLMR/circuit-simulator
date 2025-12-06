#include "CircuitManager.h"
#include <QDebug>

const char* DEVICE_CREATOR_ALIAS = "create";
std::unique_ptr<CircuitManager> CircuitManager::m_instance = nullptr;


CircuitManager::CircuitManager()
{
}

CircuitManager& CircuitManager::getInstance(void)
{
    if (m_instance == nullptr)
    {
        m_instance = std::make_unique<CircuitManager>();
    }
    return *m_instance;
}

bool CircuitManager::createDevice(std::string type,
                                  const std::pair<std::string, std::vector<double>>& deviceCharacteristics,
                                  const std::vector<std::shared_ptr<Node>>& pins)
{
    if (m_devices.find(deviceCharacteristics.first) != m_devices.end())
    {
        qDebug() << "Device already exists" ;
        return false;
    }

    boost::filesystem::path libraryPath("../devices/" + type + "/lib" + type + ".so");
    std::unique_ptr<Device, DeviceLibraryDeleter<Device>> device = loadDevice<Device>(libraryPath);
    if (device)
    {
        qDebug() << "Loading of plugin succeeded!" ;
    }
    else
    {
        qDebug() << "Loading of plugin failed!" ;
    }
    device->setName(deviceCharacteristics.first);
    device->setDeviceParameters(deviceCharacteristics.second);
    m_devices[deviceCharacteristics.first] = std::move(device);

    int index = 0;
    for (auto& node : m_devices[deviceCharacteristics.first]->getPins())
    {
        if (pins.size() != 0)
        {
            node = pins[index];
            auto& deviceCurrents = m_devices[deviceCharacteristics.first]->getCurrents();
            deviceCurrents.clear();
        }
        m_pinOf[node] = m_devices[deviceCharacteristics.first];
        m_connected[node] = node;
        index++;
    }
    m_devices[deviceCharacteristics.first]->updateDeviceState();
    return true;
}

bool CircuitManager::connect(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2)
{
    if (m_connected.find(node1) == m_connected.end() || m_connected.find(node2) == m_connected.end())
    {
        qDebug() << "Requested to connect invalid nodes!" ;
        return false;
    }

    std::shared_ptr<Node> root2 = m_connected[node2];
    std::shared_ptr<Node> root1 = m_connected[node1];
    for (auto& node : m_connected)
    {
        if (node.second == root1)
        {
            node.second = root2;
        }
    }
    
    return true;
}

bool CircuitManager::isConnected(const std::shared_ptr<Node>& node1, const std::shared_ptr<Node>& node2)
{
    return m_connected[node1] == m_connected[node2];
}

std::vector<std::shared_ptr<Device>> CircuitManager::getAdjacentDevices(const std::shared_ptr<Node>& node)
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

std::set<std::shared_ptr<Node>> CircuitManager::getUniqueNodes(void)
{
    std::set<std::shared_ptr<Node>> uniqueNodes;
    for (auto& node : m_connected)
    {
        // if (!m_pinOf[node.first]->isVoltageSupply())
        // {
            uniqueNodes.insert(node.second);
        // }
    }
    return uniqueNodes;
}

void CircuitManager::initializeCircuitMatrix(std::map<std::shared_ptr<Node>, std::vector<double>>& circuitMatrix)
{
    m_uniqueNodes = getUniqueNodes();
    int numOfSources = 0;
    for (auto& device : m_devices)
    {
        if (device.second->isVoltageSupply())
        {
            numOfSources++;
        }
    }
    for (auto& node : m_uniqueNodes)
    {
        for (int i = 0 ; i < m_uniqueNodes.size() + numOfSources ; i++)
        {
            circuitMatrix[node].push_back(0.0);
        }
    }
    for (int i = 0 ; i < m_uniqueNodes.size(); i++)
    {
        m_results.push_back(0.0);
    }
}

void CircuitManager::initializeCurrentMatrix(std::map<std::shared_ptr<Device>, std::vector<double>>& currenttMatrix)
{
    m_uniqueNodes = getUniqueNodes();
    int numOfSources = 0;
    for (auto& device : m_devices)
    {
        if (device.second->isVoltageSupply())
        {
            numOfSources++;
        }
    }
    for (auto& device : m_devices)
    {
        if (device.second->isVoltageSupply())
        {
            for (int i = 0 ; i < m_uniqueNodes.size() + numOfSources ; i++)
            {
                currenttMatrix[device.second].push_back(0.0);
            }
            m_results.push_back(0.0);
        }
    }
}

std::vector<std::shared_ptr<Node>> CircuitManager::findAllNodesConnected(const std::shared_ptr<Node>& node)
{
    std::vector<std::shared_ptr<Node>> allConnectedNodes;

    for (auto& n : m_pinOf)
    {
        if (isConnected(node, n.first))
        {
            allConnectedNodes.push_back(n.first);
        }
    }
    return allConnectedNodes;
}

std::shared_ptr<Node> CircuitManager::findWhichNodeConnected(const std::shared_ptr<Node>& node, const Device& device)
{
    for (auto& n : device.getPins())
    {
        if (isConnected(node, n))
        {
            return n;
        }
    }
    return nullptr;
}

std::shared_ptr<Node> CircuitManager::findWhichNodeConnected(const std::shared_ptr<Node>& node, const std::set<std::shared_ptr<Node>>& nodes)
{
    for (auto& n : nodes)
    {
        if (isConnected(node, n))
        {
            return n;
        }
    }
    return nullptr;
}

std::shared_ptr<Node> CircuitManager::findWhichNodeConnected(const std::shared_ptr<Node>& node)
{
    for (auto& n : m_uniqueNodes)
    {
        if (isConnected(node, n))
        {
            return n;
        }
    }
    qDebug() << "No node connected to " << QString::fromStdString(node->getName());
    return nullptr;
}

int CircuitManager::getDeviceNodeIndex(const std::shared_ptr<Node>& node, const Device& device)
{
    for (int i = 0 ; i < device.getPins().size() ; i++)
    {
        if (node == device.getPins()[i])
        {
            return i;
        }
    }
    return -1;
}

std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> CircuitManager::checkSourcesConnected(const std::shared_ptr<Node>& node,
                                                                                                                std::vector<std::shared_ptr<Device>>& adjacentDevices,
                                                                                                                const std::set<std::shared_ptr<Node>>& nodes)
{
    std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> sources;
    for (auto& device : adjacentDevices)
    {
        if (device->isVoltageSupply())
        {
            std::shared_ptr<Node> sourceNode1 = findWhichNodeConnected(node, *device);
            std::shared_ptr<Node> sourceNode2;
            if (sourceNode1 == device->getPins()[0])
            {
                sourceNode2 = device->getPins()[1];
            }
            else
            {
                sourceNode2 = device->getPins()[0];
            }
            auto otherConnectedSourceNode = findWhichNodeConnected(sourceNode2, nodes);
            sources[{node, otherConnectedSourceNode}] = device->getVoltage(sourceNode1);
        }
    }
    return sources;
}

std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> CircuitManager::checkSourcesConnected(const std::shared_ptr<Node>& node,
                                                                                                                std::vector<std::shared_ptr<Device>>& adjacentDevices)
{
    std::map<std::pair<std::shared_ptr<Node>, std::shared_ptr<Node>>, double> sources;
    for (auto& device : adjacentDevices)
    {
        if (device->isVoltageSupply())
        {
            std::shared_ptr<Node> sourceNode1 = findWhichNodeConnected(node, *device);
            std::shared_ptr<Node> sourceNode2;
            if (sourceNode1 == device->getPins()[0])
            {
                sourceNode2 = device->getPins()[1];
            }
            else
            {
                sourceNode2 = device->getPins()[0];
            }
            auto otherConnectedSourceNode = findWhichNodeConnected(sourceNode2);
            sources[{node, otherConnectedSourceNode}] = device->getVoltage(sourceNode1);
        }
    }
    return sources;
}

void CircuitManager::calculateCircuitMatrix(double deltaT)
{
    std::map<std::shared_ptr<Node>, std::vector<double>> circuitMatrix;
    std::map<std::shared_ptr<Device>, std::vector<double>> currentMatrix;
    initializeCircuitMatrix(circuitMatrix);
    initializeCurrentMatrix(currentMatrix);
    std::map<std::shared_ptr<Node>, bool> handledNodes;
    int index = 0;
    for (auto& node : m_uniqueNodes)
    {
        if (handledNodes.find(node) == handledNodes.end())
        {
            auto adjacentDevices = getAdjacentDevices(node);
            auto sourcesMap = checkSourcesConnected(node, adjacentDevices);
            if (sourcesMap.empty())
            {
                double knownIndependentCurrents = 0.0;
                for (auto& device : adjacentDevices)
                {
                    std::shared_ptr<Node> deviceNode = findWhichNodeConnected(node, *device);
                    if (device->isCurrentSupply())
                    {
                        knownIndependentCurrents += device->getCurrent(deviceNode);
                    }
                    else
                    {
                        std::map<std::shared_ptr<Node>, double> currentCoefficients = device->getCurrentCoefficients(deviceNode, deltaT);
                        for (auto& n : currentCoefficients)
                        {
                            std::shared_ptr<Node> foundNode = findWhichNodeConnected(n.first);
                            if (foundNode != nullptr)
                            {
                                circuitMatrix[foundNode][index] += n.second;
                            }
                            else
                            {
                                qDebug() << "Invalid node found! " << QString::fromStdString(n.first->getName());
                            }
                        }
                    }
                }
                m_results[index] = knownIndependentCurrents;
            }
            else
            {
                double knownIndependentCurrents = 0.0;
                for (auto& device : adjacentDevices)
                {
                    std::shared_ptr<Node> deviceNode = findWhichNodeConnected(node, *device);
                    if (device->isCurrentSupply())
                    {
                        knownIndependentCurrents += device->getCurrent(deviceNode);
                    }
                    if (device->isVoltageSupply())
                    {
                        if (node == device->getPins()[0])
                        {
                            currentMatrix[device][index] = 1;
                        }
                        else
                        {
                            currentMatrix[device][index] = -1;
                        }
                    }
                    else
                    {
                        std::map<std::shared_ptr<Node>, double> currentCoefficients = device->getCurrentCoefficients(deviceNode, deltaT);
                        for (auto& n : currentCoefficients)
                        {
                            std::shared_ptr<Node> foundNode = findWhichNodeConnected(n.first);
                            if (foundNode != nullptr)
                            {
                                circuitMatrix[foundNode][index] += n.second;
                            }
                            else
                            {
                                qDebug() << "Invalid node found! " << QString::fromStdString(n.first->getName());
                            }
                        }
                    }
                }
                m_results[index] = knownIndependentCurrents;
                index++;
                for (auto& source : sourcesMap)
                {
                    circuitMatrix[source.first.first][index] = 1.0;
                    circuitMatrix[source.first.second][index] = -1.0;
                    m_results[index] = source.second;
                    handledNodes[source.first.first] = true;
                    handledNodes[source.first.second] = true;
                    index++;
                }
            }
            index++;
        }
        else
        {
            m_results[index] = 0.0;
        }
        m_groundNode = node;
    }

    m_circuitMatrix.resize(circuitMatrix.size() + currentMatrix.size());
    index = 0;
    for (int i = 0 ; i < circuitMatrix.size() +  currentMatrix.size() ; i++)
    {
        for (const auto& node : m_uniqueNodes)
        {
            m_circuitMatrix[index].push_back(circuitMatrix[node][i]);
        }
        for (const auto& device : m_devices)
        {
            if (device.second->isVoltageSupply())
            {
                m_circuitMatrix[index].push_back(currentMatrix[device.second][i]);
            }
           
        }
        index++;
    }

    // Convert the map to a vector of vectors
    std::vector<double> groundVector;
    for (int i = 0 ; i < m_uniqueNodes.size() + currentMatrix.size() ; i++)
    {
        groundVector.push_back(0.0);
    }
    groundVector[m_uniqueNodes.size() - 1] = 1.0;
    m_circuitMatrix.push_back(groundVector);
    m_results.push_back(0.0);
}

Eigen::MatrixXd CircuitManager::convert2DVectorToMatrix(const std::vector<std::vector<double>>& vect)
{
    std::vector<double> AFlat;
    for (const auto& row : vect)
    {
        AFlat.insert(AFlat.end(), row.begin(), row.end());
    }

    // Step 2: Get rows and columns
    size_t rows = vect.size();
    size_t cols = (rows > 0) ? vect[0].size() : 0;

    // Step 3: Map into Eigen::MatrixXd (RowMajor for correct layout)
    Eigen::MatrixXd A = Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(AFlat.data(), rows, cols);
    
    return A;
}

std::vector<std::vector<double>> CircuitManager::convertMatrixTo2DVector(const Eigen::MatrixXd& matrix)
{
    std::vector<std::vector<double>> vect;
    vect.resize(matrix.rows());

    for (int i = 0; i < matrix.rows(); ++i)
    {
        vect[i].resize(matrix.cols());
        for (int j = 0; j < matrix.cols(); ++j)
        {
            vect[i][j] = matrix(i, j);
        }
    }
    return vect;
}

void CircuitManager::solveCircuit(double deltaT)
{
    // Step 1: Calculate the circuit matrix
    calculateCircuitMatrix(deltaT);

    for (auto& row : m_circuitMatrix)
    {
        for (auto e : row)
        {
            qDebug() << e << " ";
        }
        qDebug() ;
    }

    for (auto e : m_results)
    {
        qDebug() << e << " ";
    }
    qDebug() ;

    // Step 3: Map into Eigen::MatrixXd (RowMajor for correct layout)
    Eigen::MatrixXd A = convert2DVectorToMatrix(getCircuitMatrix());
    
    Eigen::VectorXd b = Eigen::Map<Eigen::VectorXd>(getVoltages().data(), getVoltages().size());

    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    int index = 0;
    for (auto& node : m_uniqueNodes)
    {
        node->setVolt(x[index]);
        for (auto& n : findAllNodesConnected(node))
        {
            n->setVolt(x[index]);
        }
        index++;
    }

    for (auto& device : m_devices)
    {
        if (device.second->isVoltageSupply())
        {
            auto& deviceCurrents = device.second->getCurrents();
            auto pins = device.second->getPins();
            deviceCurrents[pins[0]] = x[index];
            deviceCurrents[pins[1]] = x[index];
            index++;
        }
    }

    for (auto& device : m_devices)
    {
        device.second->forwardDeviceState();
        device.second->prepareForNextStep(deltaT);
    }

    m_circuitMatrix.clear();
    m_results.clear();
}

std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> CircuitManager::queryDeviceVoltages(std::string deviceName)
{
    std::pair<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>> deviceVoltages;
    if (m_devices.find(deviceName) != m_devices.end())
    {
        deviceVoltages.first = m_groundNode;
        deviceVoltages.second = m_devices[deviceName]->getPins();
        return deviceVoltages;
    }
    else
    {
        return {};
    }
}

std::map<std::shared_ptr<Node>, double> CircuitManager::queryDeviceCurrents(std::string deviceName)
{
    if (m_devices.find(deviceName) != m_devices.end())
    {
        return m_devices[deviceName]->getCurrents();
    }
    else
    {
        return {};
    }
}

CircuitManager::~CircuitManager()
{

}
