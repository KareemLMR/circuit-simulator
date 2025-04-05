#include "Resistor.h"

Resistor::Resistor() : TwoTerminal(), m_r(0)
{

}

Resistor::Resistor(double r) : TwoTerminal(), m_r(r)
{
    
}

Resistor::Resistor(const Resistor& r) : TwoTerminal(r), m_r(r.getResistance())
{

}

Resistor::Resistor(const Resistor&& r) : TwoTerminal(std::move(r)), m_r(r.getResistance())
{
    r.setResistance(0);
}

Resistor& Resistor::operator=(const Resistor& r)
{
    if (this != &r)
    {
        TwoTerminal::operator=(r);
        m_r = r.getResistance();
    }
    return *this;
}

Resistor& Resistor::operator=(const Resistor&& r)
{
    TwoTerminal::operator=(std::move(r));
    m_r = r.getResistance();
    r.setResistance(0);
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

void Resistor::calculateCurrent()
{
    double v1 = getV1();
    double v2 = getV2();

    double i = (v1 - v2) / m_r;
    setCurrent(i);
}

Resistor::~Resistor()
{
    setResistance(0);
}