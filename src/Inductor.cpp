#include "Inductor.h"

Inductor::Inductor() : TwoTerminal(), m_l(0), m_eqG(0), m_timestamp(0)
{

}

Inductor::Inductor(double l) : TwoTerminal(), m_l(l), m_eqG(0), m_timestamp(0)
{
    
}

Inductor::Inductor(const Inductor& l) : TwoTerminal(l), m_l(l.getInductance())
{

}

Inductor::Inductor(const Inductor&& l) : TwoTerminal(std::move(l)), m_l(l.getInductance())
{
    l.setInductance(0);
}

Inductor& Inductor::operator=(const Inductor& l)
{
    if (this != &l)
    {
        TwoTerminal::operator=(l);
        m_l = l.getInductance();
    }
    return *this;
}

Inductor& Inductor::operator=(const Inductor&& l)
{
    TwoTerminal::operator=(std::move(l));
    m_l = l.getInductance();
    l.setInductance(0);
    return *this;
}

double Inductor::getInductance(void)
{
    return m_l;
}

double Inductor::getEquivalentConductance(void)
{
    return m_eqG;
}

double Inductor::getTimestamp(void)
{
    return m_timestamp;
}

void Inductor::setInductance(double l)
{
    m_l = l;
}

void Inductor::setEquivalentConductance(double eqG)
{
    m_eqG = eqG;
}

void Inductor::setTimestamp(double timestamp)
{
    m_timestamp = timestamp;
}

void Inductor::calculateCurrent()
{
    double v1 = getV1();
    double v2 = getV2();

    double i = (v1 - v2) / m_eqG;
    setCurrent(i);
}

std::map<std::shared_ptr<Node>, double> Inductor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    if (std::abs(m_volt) <= std::abs(getV1() - getV2()))
    {
        m_eqG += deltaT / m_l;
    }
    else
    {
        m_eqG -= deltaT / m_l;
    }
    m_volt = getV1() - getV2();
    
    m_timestamp += deltaT;
    if (m_eqG != 0)
    {
        currentCoefficients[node] = m_eqG;
        if (node == getPins()[0])
        {
            currentCoefficients[getPins()[1]] = -m_eqG;
        }
        else if (node == getPins()[1])
        {
            currentCoefficients[getPins()[0]] = -m_eqG;
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

Inductor::~Inductor()
{
    setInductance(0);
    m_eqG = 0.0;
}