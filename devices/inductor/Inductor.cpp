#include "Inductor.h"

DEVICE_REGISTER_PLUGIN_CREATOR_METHOD(Inductor::create)

extern "C" 
{
    __attribute__((visibility("default"))) 
    Device* Inductor::create()
    {
        return new Inductor();
    }
}

void Inductor::destroy(Device* device)
{
    delete device;
}

Inductor::Inductor() : m_l(0), m_eqG(0), m_timestamp(0)
{
}

Inductor::Inductor(double l) : m_l(l), m_eqG(0), m_timestamp(0)
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

void Inductor::prepareForNextStep(double deltaT)
{
    double v1 = getV1();
    m_i -= v1 * deltaT / m_l;
    std::map<std::shared_ptr<Node>, double>& currents = getCurrents();
    std::vector<std::shared_ptr<Node>>& nodes = getPins();
    currents[nodes[0]] = m_i;
    currents[nodes[1]] = m_i;
}

std::map<std::shared_ptr<Node>, double> Inductor::getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT)
{
    std::map<std::shared_ptr<Node>, double> currentCoefficients;
    return currentCoefficients;
}

bool Inductor::receiveDeviceParameters(void)
{
    if (m_parameters[0] > 0)
    {
        setInductance(m_parameters[0]);
        return true;
    }
    else
    {
        std::cout << "Failed to set device parameter, less than 0 value provided for inductance!" << std::endl;
        return false;
    }
}

Inductor::~Inductor()
{
    setInductance(0);
    m_eqG = 0.0;
}