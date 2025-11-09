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

        void prepareForNextStep(double deltaT) override;
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override;
        bool isVoltageSupply() override { return false; }
        bool isCurrentSupply() override { return false; }

        ~Resistor();
    private:
        double m_r;
};
#endif