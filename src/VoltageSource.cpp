#include "VoltageSource.h"

VoltageSource::VoltageSource() : TwoTerminal()
{
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(double v) : TwoTerminal()
{
    setVoltage(v);
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(const VoltageSource& voltageSource) : TwoTerminal(voltageSource)
{
    setVoltage(voltageSource.getVoltage());
    m_type = DeviceType::VOLTAGE_SOURCE;
}

VoltageSource::VoltageSource(const VoltageSource&& voltageSource) : TwoTerminal(std::move(voltageSource))
{
    setVoltage(voltageSource.getVoltage());
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

VoltageSource::~VoltageSource()
{
    setVoltage(0.0);
}