#include "Resistor.h"

Resistor::Resistor() : TwoTerminal(), m_r(0)
{
    m_type = DeviceType::RESISTOR;
}

Resistor::Resistor(double r) : TwoTerminal(), m_r(r)
{
    m_type = DeviceType::RESISTOR;
}

Resistor::Resistor(const Resistor& r) : TwoTerminal(r), m_r(r.getResistance())
{
    m_type = DeviceType::RESISTOR;
}

Resistor::Resistor(const Resistor&& r) : TwoTerminal(std::move(r)), m_r(r.getResistance())
{
    m_type = DeviceType::RESISTOR;
    r.setResistance(0);
}

Resistor& Resistor::operator=(const Resistor& r)
{
    if (this != &r)
    {
        TwoTerminal::operator=(r);
        m_r = r.getResistance();
        m_type = DeviceType::RESISTOR;
    }
    return *this;
}

Resistor& Resistor::operator=(const Resistor&& r)
{
    TwoTerminal::operator=(std::move(r));
    m_r = r.getResistance();
    r.setResistance(0);
    m_type = DeviceType::RESISTOR;
    return *this;
}

double Resistor::getResistance(void)
{
    return m_r;
}

void Resistor::setResistance(double r)
{
    m_r = r;
}

void Resistor::prepareForNextStep(double deltaT)
{
    std::cout << "Calculating current for resistor" << std::endl;
    double v1 = getV1();
    double v2 = getV2();

    double i = (v1 - v2) / m_r;
    setCurrent(i);
}

std::map<std::shared_ptr<Node>, double> Resistor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    currentCoefficients[node] = 1.0 / m_r;
    if (node == getPins()[0])
    {
        currentCoefficients[getPins()[1]] = -1.0 / m_r;
    }
    else if (node == getPins()[1])
    {
        currentCoefficients[getPins()[0]] = -1.0 / m_r;
    }
    else
    {
        std::cout << "Invalid node" << std::endl;
        return {};
    }
    return currentCoefficients;
}

Resistor::~Resistor()
{
    setResistance(0);
}