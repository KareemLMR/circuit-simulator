#ifndef VOLTAGE_SOURCE_H_
#define VOLTAGE_SOURCE_H_

#include "TwoTerminal.h"
#include <iostream>


class DEVICE_API VoltageSource : public TwoTerminal
{
    public:
        static Device* create();
        static void destroy(Device* device);

        VoltageSource();
        VoltageSource(double v);        

        VoltageSource(const VoltageSource& voltageSource);
        VoltageSource(const VoltageSource&& voltageSource);

        VoltageSource& operator=(const VoltageSource& voltageSource);
        VoltageSource& operator=(const VoltageSource&& voltageSource);

        bool isVoltageSupply() override { return true; }
        bool isCurrentSupply() override { return false; }
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override { return {}; };
        void prepareForNextStep(double deltaT) override {};
        bool receiveDeviceParameters(void) override;

        ~VoltageSource();
};

#endif
