#ifndef CAPACITOR_H_
#define CAPACITOR_H_

#include "TwoTerminal.h"
#include <iostream>

class Capacitor : public TwoTerminal
{
    public:
        Capacitor();
        Capacitor(double c);

        Capacitor(const Capacitor& c);
        Capacitor(const Capacitor&& c);

        Capacitor& operator=(const Capacitor& c);
        Capacitor& operator=(const Capacitor&& c);

        double getCapacitance(void);
        double getEquivalentResistance(void);
        double getTimestamp(void);

        void setCapacitance(double c);
        void setEquivalentResistance(double eqR);
        void setTimestamp(double timestamp);

        void calculateCurrent() override;
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override;
        bool isSource() override { return false; }
        double getVoltage(const std::shared_ptr<Node>& node) override { return getV1() - getV2(); }

        ~Capacitor();
    private:
        double m_c;
        double m_eqR;
        double m_timestamp;
};
#endif