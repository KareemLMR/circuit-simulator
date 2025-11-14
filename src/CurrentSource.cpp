#include "CurrentSource.h"

CurrentSource::CurrentSource()
{
    setCurrent(0.0);
    m_type = DeviceType::CURRENT_SOURCE;
}

CurrentSource::CurrentSource(double i)
{
    setCurrent(i);
    m_type = DeviceType::CURRENT_SOURCE;
}

CurrentSource::CurrentSource(const CurrentSource& CurrentSource) : TwoTerminal(CurrentSource)
{
    setCurrent(CurrentSource.getCurrent());
    m_type = DeviceType::CURRENT_SOURCE;
}

CurrentSource::CurrentSource(const CurrentSource&& CurrentSource) : TwoTerminal(std::move(CurrentSource))
{
    setCurrent(CurrentSource.getCurrent());
    m_type = DeviceType::CURRENT_SOURCE;
    CurrentSource.setCurrent(0.0);
}

CurrentSource& CurrentSource::operator=(const CurrentSource& CurrentSource)
{
    if (this != &CurrentSource)
    {
        TwoTerminal::operator=(CurrentSource);
        m_i = CurrentSource.getCurrent();
        m_type = DeviceType::CURRENT_SOURCE;
    }
    return *this;
}

CurrentSource& CurrentSource::operator=(const CurrentSource&& CurrentSource)
{
    TwoTerminal::operator=(std::move(CurrentSource));
    m_i = CurrentSource.getCurrent();
    CurrentSource.setCurrent(0.0);
    m_type = DeviceType::CURRENT_SOURCE;
    return *this;
}

CurrentSource::~CurrentSource()
{
    setCurrent(0.0);
}