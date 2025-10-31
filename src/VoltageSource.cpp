#include "VoltageSource.h"

VoltageSource::VoltageSource() : TwoTerminal(), m_v(0.0)
{
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(double v) : TwoTerminal(), m_v(v)
{
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(const VoltageSource& voltageSource) : TwoTerminal(voltageSource), m_v(voltageSource.getVoltage())
{
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(const VoltageSource&& voltageSource) : TwoTerminal(std::move(voltageSource)), m_v(voltageSource.getVoltage())
{
    m_type = DeviceType::VOLTAGE_SOURCE;
    voltageSource.setVoltage(0.0);
}

VoltageSource& VoltageSource::operator=(const VoltageSource& voltageSource)
{
    if (this != &voltageSource)
    {
        TwoTerminal::operator=(voltageSource);
        m_v = voltageSource.getVoltage();
        m_type = DeviceType::VOLTAGE_SOURCE;
    }
    return *this;
}

VoltageSource& VoltageSource::operator=(const VoltageSource&& voltageSource)
{
    TwoTerminal::operator=(std::move(voltageSource));
    m_v = voltageSource.getVoltage();
    voltageSource.setVoltage(0.0);
    m_type = DeviceType::VOLTAGE_SOURCE;
    return *this;
}

double VoltageSource::getVoltage(const std::shared_ptr<Node>& node)
{
    static int counter = 0;
    // if ((counter / 100) % 2 == 0)
    // {
    //     counter++;
    //     return 0.0;
    // }
    if (node == getPins()[0])
    {
        counter++;
        return m_v;
    }
    else if (node == getPins()[1])
    {
        counter++;
        return -1.0 * m_v;
    }
    else
    {
        counter++;
        std::cout << "getVoltage: Invalid node" << std::endl;
        return 0.0;
    }
}

double VoltageSource::getVoltage(void)
{
    return m_v;
}


void VoltageSource::setVoltage(double v)
{
    m_v = v;
}

VoltageSource::~VoltageSource()
{
    setVoltage(0.0);
}