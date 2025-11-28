#include "CurrentSource.h"

DEVICE_REGISTER_PLUGIN_CREATOR_METHOD(CurrentSource::create)

extern "C" 
{
    __attribute__((visibility("default"))) 
    Device* CurrentSource::create()
    {
        return new CurrentSource();
    }
}

void CurrentSource::destroy(Device* device)
{
    delete device;
}

CurrentSource::CurrentSource()
{
    setCurrent(0.0);
}

CurrentSource::CurrentSource(double i)
{
    setCurrent(i);
}

CurrentSource::CurrentSource(const CurrentSource& CurrentSource) : TwoTerminal(CurrentSource)
{
    setCurrent(CurrentSource.getCurrent());
}

CurrentSource::CurrentSource(const CurrentSource&& CurrentSource) : TwoTerminal(std::move(CurrentSource))
{
    setCurrent(CurrentSource.getCurrent());
    CurrentSource.setCurrent(0.0);
}

CurrentSource& CurrentSource::operator=(const CurrentSource& CurrentSource)
{
    if (this != &CurrentSource)
    {
        TwoTerminal::operator=(CurrentSource);
        m_i = CurrentSource.getCurrent();
    }
    return *this;
}

CurrentSource& CurrentSource::operator=(const CurrentSource&& CurrentSource)
{
    TwoTerminal::operator=(std::move(CurrentSource));
    m_i = CurrentSource.getCurrent();
    CurrentSource.setCurrent(0.0);
    return *this;
}

bool CurrentSource::receiveDeviceParameters(void)
{
    setCurrent(m_parameters[0]);
    return true;
}

CurrentSource::~CurrentSource()
{
    setCurrent(0.0);
}