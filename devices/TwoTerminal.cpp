#include "TwoTerminal.h"

TwoTerminal::TwoTerminal() : Device(2), m_v1(0.0), m_v2(0.0), m_i(0.0)
{

}

TwoTerminal::TwoTerminal(const TwoTerminal& ttl) : Device(ttl)
{
    if (this != &ttl)
    {
        m_transferFunction = ttl.getTransferFunction();
        m_v1 = ttl.getV1();
        m_v2 = ttl.getV2();
        m_i = ttl.getCurrent();
    }
}

TwoTerminal::TwoTerminal(const TwoTerminal&& ttl) : Device(std::move(ttl))
{
    m_transferFunction = ttl.getTransferFunction();
    m_v1 = ttl.getV1();
    m_v2 = ttl.getV2();
    m_i = ttl.getCurrent();

    ttl.setV1(0.0);
    ttl.setV2(0.0);
    ttl.setCurrent(0.0);
    ttl.setTransferFunction(nullptr);
}

TwoTerminal& TwoTerminal::operator=(const TwoTerminal& ttl)
{
    if (this != &ttl)
    {
        Device::operator=(ttl);
        m_transferFunction = ttl.getTransferFunction();
        m_v1 = ttl.getV1();
        m_v2 = ttl.getV2();
        m_i = ttl.getCurrent();
    }

    return *this;
}

TwoTerminal& TwoTerminal::operator=(const TwoTerminal&& ttl)
{
    Device::operator=(std::move(ttl));
    m_transferFunction = ttl.getTransferFunction();
    m_v1 = ttl.getV1();
    m_v2 = ttl.getV2();
    m_i = ttl.getCurrent();

    ttl.setV1(0.0);
    ttl.setV2(0.0);
    ttl.setCurrent(0.0);
    ttl.setTransferFunction(nullptr);

    return *this;
}

void TwoTerminal::setTransferFunction(std::function<double(double, double)> tf)
{
    m_transferFunction = tf;
}

void TwoTerminal::setCurrent(double i)
{
    m_i = i;
    updateDeviceState();
}

void TwoTerminal::setV1(double v1)
{
    m_v1 = v1;
}

void TwoTerminal::setV2(double v2)
{
    m_v2 = v2;
}

double TwoTerminal::getCurrent(void)
{
    return m_i;
}

double TwoTerminal::getV1(void)
{
    return m_v1;
}

double TwoTerminal::getV2(void)
{
    return m_v2;
}

std::function<double(double, double)>  TwoTerminal::getTransferFunction(void)
{
    return m_transferFunction;
}

void TwoTerminal::updateDeviceState()
{
    std::map<std::shared_ptr<Node>, double>& currents = getCurrents();
    currents[getPins()[0]] = m_i;
    currents[getPins()[1]] = m_i;
}

void TwoTerminal::forwardDeviceState()
{
    std::vector<std::shared_ptr<Node>>& nodes = getPins();
    setV1(nodes[0]->getVolt());
    setV2(nodes[1]->getVolt());
}

void TwoTerminal::routeCurrents(std::shared_ptr<Node> node)
{
    std::map<std::shared_ptr<Node>, double>& currents = getCurrents();
    std::vector<std::shared_ptr<Node>>& nodes = getPins();
    if (node == nodes[0])
    {
        currents[nodes[1]] = currents[nodes[0]];
    }
    else if (node == nodes[1])
    {
        currents[nodes[0]] = currents[nodes[1]];
    }
    else
    {
        qDebug() << "Invalid node!" ;
    }
}

double TwoTerminal::getVoltage(const std::shared_ptr<Node>& node)
{
    if (node == getPins()[0])
    {
        return m_v;
    }
    else if (node == getPins()[1])
    {
        return -m_v;
    }
    else
    {
        qDebug() << "getVoltage: Invalid node" ;
        return 0.0;
    }
}

double TwoTerminal::getVoltage()
{
    return m_v;
}

void TwoTerminal::setVoltage(double v)
{
    m_v = v;
}

double TwoTerminal::getCurrent(const std::shared_ptr<Node>& node)
{
    if (node == getPins()[0])
    {
        return m_i;
    }
    else if (node == getPins()[1])
    {
        return -m_i;
    }
    else
    {
        qDebug() << "getCurrent: Invalid node" ;
        return 0.0;
    }
}

bool TwoTerminal::setDeviceParameters(const std::vector<double>& parameters)
{
    if (parameters.empty())
    {
        qDebug() << "Failed to set parameters of " << QString::fromStdString(getName()) << ", parameters list empty" ;
        return false;
    }
    m_parameters = parameters;
    return receiveDeviceParameters();
}

TwoTerminal::~TwoTerminal()
{
    m_transferFunction = nullptr;
    m_v1 = 0.0;
    m_v2 = 0.0;
    m_i = 0.0;
}
