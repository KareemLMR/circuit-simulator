#ifndef DEVICE_H_
#define DEVICE_H_

#include "Node.h"
#include "vector"
#include <map>
#include <memory>

class Device
{
    public:
        Device();
        Device(std::string name);
        Device(int terminals);
        Device(std::string name, int terminals);

        Device(const Device& device);
        Device(const Device&& device);

        Device& operator=(const Device& device);
        Device& operator=(const Device&& device);

        void setName(std::string name);
        void setNumOfTerminals(int terminals);
        void setPins(const std::vector<std::shared_ptr<Node>>& pins);
        void setPins(std::vector<std::shared_ptr<Node>>&& pins);
        void setCurrents(const std::vector<std::vector<double>>& currents);
        void setCurrents(std::vector<std::vector<double>>&& currents);

        std::string getName(void);
        int getNumOfTerminals(void);
        std::vector<std::shared_ptr<Node>> & getPins(void);
        std::vector<std::vector<double>>& getCurrents(void);

        virtual void updateDeviceState() = 0;
        virtual void forwardDeviceState() = 0;
        virtual std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node) = 0;
        virtual bool isSource() = 0;
        virtual double getVoltage(const std::shared_ptr<Node>& node) = 0;
        virtual void calculateCurrent() = 0;

        virtual ~Device();

    private:
        std::string m_name;
        int m_terminals;
        std::vector<std::shared_ptr<Node>> m_pins;
        std::vector<std::vector<double>> m_currents;
        static int m_counter;
};

#endif