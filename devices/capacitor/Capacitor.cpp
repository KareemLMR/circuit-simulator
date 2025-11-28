#include "Capacitor.h"

DEVICE_REGISTER_PLUGIN_CREATOR_METHOD(Capacitor::create)

extern "C" 
{
    __attribute__((visibility("default"))) 
    Device* Capacitor::create()
    {
        return new Capacitor();
    }
}

void Capacitor::destroy(Device* device)
{
    delete device;
}

Capacitor::Capacitor() : m_c(0), m_eqR(0), m_timestamp(0)
{
}

Capacitor::Capacitor(double c) : m_c(c), m_eqR(0), m_timestamp(0)
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

void Capacitor::prepareForNextStep(double deltaT)
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

bool Capacitor::receiveDeviceParameters(void)
{
    if (m_parameters[0] > 0)
    {
        setCapacitance(m_parameters[0]);
        return true;
    }
    else
    {
        std::cout << "Failed to set device parameter, less than 0 value provided for capacitance!" << std::endl;
        return false;
    }
}

Capacitor::~Capacitor()
{
    setCapacitance(0);
    m_eqR = 0.0;
}