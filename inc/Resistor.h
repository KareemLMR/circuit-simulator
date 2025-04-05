#ifndef RESISTOR_H_
#define RESISTOR_H_

#include "TwoTerminal.h"

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

        ~Resistor();
    private:
        double m_r;
};
#endif