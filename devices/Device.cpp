#include "Device.h"

int Device::m_counter = 0;

Device::Device() : m_name("dev" + std::to_string(m_counter++))
{

}

Device::Device(std::string name) : m_name(name)
{

}

Device::Device(int terminals) : m_name("dev" + std::to_string(m_counter++)), m_terminals(terminals)
{
    for (int i = 0 ; i < terminals ; i++)
    {
        m_pins.push_back(std::make_shared<Node>());
    }
}

Device::Device(std::string name, int terminals) : m_name(name), m_terminals(terminals)
{
    for (int i = 0 ; i < terminals ; i++)
    {
        m_pins.push_back(std::make_shared<Node>());
    }
}

Device::Device(const Device& device)
{
    if (this != &device)
    {
        m_terminals = device.getNumOfTerminals();
        m_name = device.getName();
        m_pins = device.getPins();
        m_currents = device.getCurrents();
    }
}

Device::Device(const Device&& device)
{
    m_terminals = device.getNumOfTerminals();
    m_name = device.getName();
    m_pins = std::move(device.getPins());
    m_currents = std::move(device.getCurrents());

    device.setName("");
    device.setNumOfTerminals(0);
    device.getCurrents().clear();
    device.getPins().clear();
}

Device& Device::operator=(const Device& device)
{
    if (this != &device)
    {
        m_terminals = device.getNumOfTerminals();
        m_name = device.getName();
        m_pins = device.getPins();
        m_currents = device.getCurrents();
    }

    return *this;
}

Device& Device::operator=(const Device&& device)
{
    m_terminals = device.getNumOfTerminals();
    m_name = device.getName();
    m_pins = std::move(device.getPins());
    m_currents = std::move(device.getCurrents());

    device.setName("");
    device.setNumOfTerminals(0);
    device.getCurrents().clear();
    device.getPins().clear();

    return *this;
}

void Device::setName(std::string name)
{
    m_name = name;
}

void Device::setNumOfTerminals(int terminals)
{
    m_terminals = terminals;
}

void Device::setPins(const std::vector<std::shared_ptr<Node>>& pins)
{
    m_pins = pins;
}

void Device::setPins(std::vector<std::shared_ptr<Node>> && pins)
{
    m_pins = std::move(pins);
}

void Device::setCurrents(const std::map<std::shared_ptr<Node>, double>& currents)
{
    m_currents = currents;
}

void Device::setCurrents(std::map<std::shared_ptr<Node>, double>&& currents)
{
    m_currents = std::move(currents);
}

std::string Device::getName(void)
{
    return m_name;
}

int Device::getNumOfTerminals(void)
{
    return m_terminals;
}

std::vector<std::shared_ptr<Node>>& Device::getPins(void)
{
    return m_pins;
}

std::map<std::shared_ptr<Node>, double>& Device::getCurrents(void)
{
    return m_currents;
}

Device::~Device()
{
    m_name.clear();
    m_terminals = 0;
    m_pins.clear();
    m_currents.clear();
}