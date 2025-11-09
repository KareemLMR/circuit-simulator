#include "Inductor.h"

Inductor::Inductor() : TwoTerminal(), m_l(0), m_eqG(0), m_timestamp(0)
{
    m_type = DeviceType::INDUCTOR;
}

Inductor::Inductor(double l) : TwoTerminal(), m_l(l), m_eqG(0), m_timestamp(0)
{
    m_type = DeviceType::INDUCTOR;
}

Inductor::Inductor(const Inductor& l) : TwoTerminal(l), m_l(l.getInductance())
{
    m_type = DeviceType::INDUCTOR;
}

Inductor::Inductor(const Inductor&& l) : TwoTerminal(std::move(l)), m_l(l.getInductance())
{
    m_type = DeviceType::INDUCTOR;
    l.setInductance(0);
}

Inductor& Inductor::operator=(const Inductor& l)
{
    if (this != &l)
    {
        TwoTerminal::operator=(l);
        m_l = l.getInductance();
        m_type = DeviceType::INDUCTOR;
    }
    return *this;
}

Inductor& Inductor::operator=(const Inductor&& l)
{
    TwoTerminal::operator=(std::move(l));
    m_l = l.getInductance();
    l.setInductance(0);
    m_type = DeviceType::INDUCTOR;
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

void Inductor::prepareForNextStep(double deltaT)
{
    double v = getVoltage();
    m_i += v * deltaT / m_l;
}

std::map<std::shared_ptr<Node>, double> Inductor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    return currentCoefficients;
}

Inductor::~Inductor()
{
    setInductance(0);
    m_eqG = 0.0;
}