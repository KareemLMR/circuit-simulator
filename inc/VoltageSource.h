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
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override { return {}; };
        void calculateCurrent() override {};

        double getVoltage(const std::shared_ptr<Node>& node) override;
        void setVoltage(double v);

        ~VoltageSource();
        
    private:
        double getVoltage(void);
        double m_v;
};

#endif

