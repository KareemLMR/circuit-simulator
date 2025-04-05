#include "Node.h"

int Node::m_counter = 0;

Node::Node() : m_name("node" + std::to_string(m_counter++)), m_volt(0.0)
{

}

Node::Node(std::string name) : m_name(name), m_volt(0.0)
{

}

Node::Node(std::string name, double volt) : m_name(name), m_volt(volt)
{

}

Node::Node(const Node& node)
{
    if (this != &node)
    {
        m_name = node.getName();
        m_volt = node.getVolt();
    }
}

Node::Node(const Node&& node)
{
    m_name = node.getName();
    m_volt = node.getVolt();
    node.setName("");
    node.setVolt(0.0);
}

Node& Node::operator=(const Node& node)
{
    if (this != &node)
    {
        m_name = node.getName();
        m_volt = node.getVolt();
    }
    return *this;
}

Node& Node::operator=(const Node&& node)
{
    m_name = node.getName();
    m_volt = node.getVolt();
    node.setName("");
    node.setVolt(0.0);

    return *this;
}

bool Node::operator<(const Node& node)
{
    return m_name < node.getName();
}

std::string Node::getName(void)
{
    return m_name;
}

double Node::getVolt(void)
{
    return m_volt;
}

void Node::setName(std::string name)
{
    m_name = name;
}

void Node::setVolt(double volt)
{
    m_volt = volt;
}

Node::~Node()
{
    m_name.clear();
    m_volt = 0.0;
}