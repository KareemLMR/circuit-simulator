#include "VoltageSource.h"

DEVICE_REGISTER_PLUGIN_CREATOR_METHOD(VoltageSource::create)

extern "C" 
{
    __attribute__((visibility("default"))) 
    Device* VoltageSource::create()
    {
        return new VoltageSource();
    }
}

void VoltageSource::destroy(Device* device)
{
    delete device;
}

VoltageSource::VoltageSource()
{
}

VoltageSource::VoltageSource(double v)
{
    setVoltage(v);
}

VoltageSource::VoltageSource(const VoltageSource& voltageSource) : TwoTerminal(voltageSource)
{
    setVoltage(voltageSource.getVoltage());
}

VoltageSource::VoltageSource(const VoltageSource&& voltageSource) : TwoTerminal(std::move(voltageSource))
{
    setVoltage(voltageSource.getVoltage());
    voltageSource.setVoltage(0.0);
}

VoltageSource& VoltageSource::operator=(const VoltageSource& voltageSource)
{
    if (this != &voltageSource)
    {
        TwoTerminal::operator=(voltageSource);
        m_v = voltageSource.getVoltage();
    }
    return *this;
}

VoltageSource& VoltageSource::operator=(const VoltageSource&& voltageSource)
{
    TwoTerminal::operator=(std::move(voltageSource));
    m_v = voltageSource.getVoltage();
    voltageSource.setVoltage(0.0);
    return *this;
}

bool VoltageSource::receiveDeviceParameters(void)
{
    setVoltage(m_parameters[0]);
    return true;
}

VoltageSource::~VoltageSource()
{
    setVoltage(0.0);
}