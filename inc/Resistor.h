#ifndef RESISTOR_H_
#define RESISTOR_H_

#include "TwoTerminal.h"
#include <iostream>

class Resistor : public TwoTerminal
{
    public:
        Resistor();
        Resistor(double r);

        Resistor(const Resistor& r);
        Resistor(const Resistor&& r);

        Resistor& operator=(const Resistor& r);
        Resistor& operator=(const Resistor&& r);

        double getResistance(void);
        void setResistance(double r);

        void calculateCurrent() override;
        std::map<Node, double> getCurrentCoefficients(const Node& node) override;
        bool isSource() override { return false; }
        double getVoltage(const Node& node) override { return getV1() - getV2(); }
        

        ~Resistor();
    private:
        double m_r;
};
#endif