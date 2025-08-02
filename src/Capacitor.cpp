#include "Capacitor.h"

Capacitor::Capacitor() : TwoTerminal(), m_c(0), m_eqR(0), m_timestamp(0)
{

}

Capacitor::Capacitor(double c) : TwoTerminal(), m_c(c), m_eqR(0), m_timestamp(0)
{
    
}

Capacitor::Capacitor(const Capacitor& c) : TwoTerminal(c), m_c(c.getCapacitance())
{

}

Capacitor::Capacitor(const Capacitor&& c) : TwoTerminal(std::move(c)), m_c(c.getCapacitance())
{
    c.setCapacitance(0);
}

Capacitor& Capacitor::operator=(const Capacitor& c)
{
    if (this != &c)
    {
        TwoTerminal::operator=(c);
        m_c = c.getCapacitance();
    }
    return *this;
}

Capacitor& Capacitor::operator=(const Capacitor&& c)
{
    TwoTerminal::operator=(std::move(c));
    m_c = c.getCapacitance();
    c.setCapacitance(0);
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

void Capacitor::calculateCurrent()
{
    double v1 = getV1();
    double v2 = getV2();

    double i = (v1 - v2) / m_eqR;
    setCurrent(i);
}

std::map<std::shared_ptr<Node>, double> Capacitor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    m_eqR += deltaT / m_c;
    m_timestamp += deltaT;
    if (m_eqR != 0)
    {
        currentCoefficients[node] = 1.0 / m_eqR;
        if (node == getPins()[0])
        {
            currentCoefficients[getPins()[1]] = -1.0 / m_eqR;
        }
        else if (node == getPins()[1])
        {
            currentCoefficients[getPins()[0]] = -1.0 / m_eqR;
        }
        else
        {
            std::cout << "Invalid node" << std::endl;
            return {};
        }
    }
    else
    {
        
    }
    
    return currentCoefficients;
}

Capacitor::~Capacitor()
{
    setCapacitance(0);
    m_eqR = 0.0;
}