#ifndef NODE_H_
#define NODE_H_

#include "string"

class Node
{
    public:
        Node();
        Node(std::string name);
        Node(std::string name, double m_volt);

        Node(const Node& node);
        Node(const Node&& node);

        Node& operator=(const Node& node);
        Node& operator=(const Node&& node);

        bool operator<(const Node& node);

        std::string getName(void);
        double getVolt(void);

        void setName(std::string name);
        void setVolt(double volt);

        ~Node();

    private:
        std::string m_name;
        double m_volt;
        static int m_counter;
};

#endif