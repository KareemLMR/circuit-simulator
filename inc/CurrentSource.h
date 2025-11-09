#ifndef CURRENT_SOURCE_H_
#define CURRENT_SOURCE_H_

#include "TwoTerminal.h"
#include <iostream>



class CurrentSource : public TwoTerminal
{
    public:
        CurrentSource();
        CurrentSource(double i);        

        CurrentSource(const CurrentSource& CurrentSource);
        CurrentSource(const CurrentSource&& CurrentSource);

        CurrentSource& operator=(const CurrentSource& CurrentSource);
        CurrentSource& operator=(const CurrentSource&& CurrentSource);

        bool isCurrentSupply() override { return true; }
        bool isVoltageSupply() override { return false; }
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override { return {}; };
        void prepareForNextStep(double deltaT) override {};

        ~CurrentSource();
};

#endif
