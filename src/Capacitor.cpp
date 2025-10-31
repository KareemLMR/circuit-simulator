#include "Capacitor.h"

Capacitor::Capacitor() : TwoTerminal(), m_c(0), m_eqR(0), m_timestamp(0)
{
    m_type = DeviceType::CAPACITOR;
}

Capacitor::Capacitor(double c) : TwoTerminal(), m_c(c), m_eqR(0), m_timestamp(0)
{
    m_type = DeviceType::CAPACITOR;
}

Capacitor::Capacitor(const Capacitor& c) : TwoTerminal(c), m_c(c.getCapacitance())
{
    m_type = DeviceType::CAPACITOR;
}

Capacitor::Capacitor(const Capacitor&& c) : TwoTerminal(std::move(c)), m_c(c.getCapacitance())
{
    m_type = DeviceType::CAPACITOR;
    c.setCapacitance(0);
}

Capacitor& Capacitor::operator=(const Capacitor& c)
{
    if (this != &c)
    {
        TwoTerminal::operator=(c);
        m_c = c.getCapacitance();
        m_type = DeviceType::CAPACITOR;
    }
    return *this;
}

Capacitor& Capacitor::operator=(const Capacitor&& c)
{
    TwoTerminal::operator=(std::move(c));
    m_c = c.getCapacitance();
    c.setCapacitance(0);
    m_type = DeviceType::CAPACITOR;
    return *this;
}

double Capacitor::getCapacitance(void)
{
    return m_c;
}

double Capacitor::getEquivalentResistance(void)
{
    return m_eqR;
}

double Capacitor::getTimestamp(void)
{
    return m_timestamp;
}

void Capacitor::setCapacitance(double c)
{
    m_c = c;
}

void Capacitor::setEquivalentResistance(double eqR)
{
    m_eqR = eqR;
}

void Capacitor::setTimestamp(double timestamp)
{
    m_timestamp = timestamp;
}

double Capacitor::getVoltage(const std::shared_ptr<Node>& node)
{
    double ret = 0.0;
    if (node == getPins()[0])
    {
        ret = m_v;
    }
    else if (node == getPins()[1])
    {
        ret = -m_v;
    }
    else
    {
        std::cout << "getVoltage: Invalid node" << std::endl;
        ret = 0.0;
    }

    return ret;
}


void Capacitor::calculateCurrent(double deltaT)
{
    auto& currents = getCurrents();
    double i = currents[getPins()[0]];

    m_v += i * deltaT / m_c;
}

std::map<std::shared_ptr<Node>, double> Capacitor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    return currentCoefficients;
}

Capacitor::~Capacitor()
{
    setCapacitance(0);
    m_eqR = 0.0;
}