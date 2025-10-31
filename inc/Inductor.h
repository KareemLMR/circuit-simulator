#ifndef Inductor_H_
#define Inductor_H_

#include "TwoTerminal.h"
#include <iostream>

class Inductor : public TwoTerminal
{
    public:
        Inductor();
        Inductor(double l);

        Inductor(const Inductor& l);
        Inductor(const Inductor&& l);

        Inductor& operator=(const Inductor& l);
        Inductor& operator=(const Inductor&& l);

        double getInductance(void);
        double getEquivalentConductance(void);
        double getTimestamp(void);

        void setInductance(double l);
        void setEquivalentConductance(double eqG);
        void setTimestamp(double timestamp);

        void calculateCurrent(double deltaT) override;
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override;
        bool isSource() override { return false; }
        double getVoltage(const std::shared_ptr<Node>& node) override { return getV1() - getV2(); }

        ~Inductor();
    private:
        double m_l;
        double m_eqG;
        double m_timestamp;
        double m_volt;
};
#endif