#ifndef CAPACITOR_H_
#define CAPACITOR_H_

#include "TwoTerminal.h"
#include <iostream>
#include <math.h>

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

        void calculateCurrent(double deltaT) override;
        std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) override;
        bool isSource() override { return true; }
        double getVoltage(const std::shared_ptr<Node>& node) override;

        ~Capacitor();
    private:
        double m_c;
        double m_eqR;
        double m_timestamp;
        double m_v;
};
#endif