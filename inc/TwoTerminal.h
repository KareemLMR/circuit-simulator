#ifndef TWO_TERMINAL_H_
#define TWO_TERMINAL_H_

#include "Device.h"
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

        virtual void calculateCurrent() = 0;

        virtual ~TwoTerminal();

    private:
        double m_v1, m_v2, m_i;
        std::function<double(double, double)> m_transferFunction;
};

#endif