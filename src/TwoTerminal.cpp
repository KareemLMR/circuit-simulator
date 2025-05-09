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
    std::vector<std::vector<double>>& currents = getCurrents();
    currents[0][1] = m_i;
    currents[1][0] = -1.0 * m_i;
}

void TwoTerminal::forwardDeviceState()
{
    std::vector<std::shared_ptr<Node>>& nodes = getPins();
    setV1(nodes[0]->getVolt());
    setV2(nodes[1]->getVolt());
}

TwoTerminal::~TwoTerminal()
{
    m_transferFunction = nullptr;
    m_v1 = 0.0;
    m_v2 = 0.0;
    m_i = 0.0;
}