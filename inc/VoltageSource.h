#ifndef VOLTAGE_SOURCE_H_
#define VOLTAGE_SOURCE_H_

#include "TwoTerminal.h"
#include <iostream>



class VoltageSource : public TwoTerminal
{
    public:
        VoltageSource();
        VoltageSource(double v);        

        VoltageSource(const VoltageSource& voltageSource);
        VoltageSource(const VoltageSource&& voltageSource);

        VoltageSource& operator=(const VoltageSource& voltageSource);
        VoltageSource& operator=(const VoltageSource&& voltageSource);

        bool isSource() override { return true; }
        std::map<Node, double> getCurrentCoefficients(const Node& node) override { return {}; };
        void calculateCurrent() override {};

        double getVoltage(const Node& node) override;
        void setVoltage(double v);

        ~VoltageSource();
        
    private:
        double getVoltage(void);
        double m_v;
};

#endif

