#ifndef TWO_TERMINAL_H_
#define TWO_TERMINAL_H_

#include "Device.h"
#include <iostream>
#include <functional>

class TwoTerminal : public Device
{
    public:
        TwoTerminal();

        TwoTerminal(const TwoTerminal& twoTerminal);
        TwoTerminal(const TwoTerminal&& twoTerminal);

        TwoTerminal& operator=(const TwoTerminal& twoTerminal);
        TwoTerminal& operator=(const TwoTerminal&& twoTerminal);

        void setTransferFunction(std::function<double(double, double)> tf);
        void setCurrent(double i);
        void setV1(double v1);
        void setV2(double v2);

        double getCurrent(void);
        double getV1(void);
        double getV2(void);
        std::function<double(double, double)>  getTransferFunction(void);

        void updateDeviceState() override;
        void forwardDeviceState() override;
        void routeCurrents(std::shared_ptr<Node> node) override;
        double getVoltage(const std::shared_ptr<Node>& node) override;
        double getVoltage(void);
        void setVoltage(double v);
        double getCurrent(const std::shared_ptr<Node>& node) override;
        bool setDeviceParameters(const std::vector<double>& parameters) override;
        virtual bool receiveDeviceParameters(void) = 0;

        virtual ~TwoTerminal();

    protected:
        double m_v, m_i;
        std::vector<double> m_parameters;

    private:
        double m_v1, m_v2;
        std::function<double(double, double)> m_transferFunction;
};

#endif