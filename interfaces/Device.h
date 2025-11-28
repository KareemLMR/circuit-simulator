#ifndef DEVICE_H_
#define DEVICE_H_

#include "Node.h"
#include "vector"
#include <map>
#include <memory>
#include <boost/dll/alias.hpp>

#define DEVICE_REGISTER_PLUGIN_CREATOR_METHOD(creatorMethod) BOOST_DLL_ALIAS(creatorMethod, create)

// Export macro
#ifdef _WIN32
    #ifdef DEVICE_EXPORTS
        #define DEVICE_API __declspec(dllexport)
    #else
        #define DEVICE_API __declspec(dllimport)
    #endif
#else
    #define DEVICE_API __attribute__((visibility("default")))
#endif

class DEVICE_API Device
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
        void setCurrents(const std::map<std::shared_ptr<Node>, double>& currents);
        void setCurrents(std::map<std::shared_ptr<Node>, double>&& currents);

        std::string getName(void);
        int getNumOfTerminals(void);
        std::vector<std::shared_ptr<Node>>& getPins(void);
        std::map<std::shared_ptr<Node>, double>& getCurrents(void);

        virtual void updateDeviceState() = 0;
        virtual void forwardDeviceState() = 0;
        virtual std::map<std::shared_ptr<Node>, double> getCurrentCoefficients(const std::shared_ptr<Node>& node, double deltaT) = 0;
        virtual bool isVoltageSupply() = 0;
        virtual bool isCurrentSupply() = 0;
        virtual double getVoltage(const std::shared_ptr<Node>& node) = 0;
        virtual double getCurrent(const std::shared_ptr<Node>& node) = 0;
        virtual void prepareForNextStep(double deltaT) = 0;
        virtual void routeCurrents(std::shared_ptr<Node> node) = 0;
        virtual bool setDeviceParameters(const std::vector<double>& parameters) = 0;

        virtual ~Device();

    private:
        std::string m_name;
        int m_terminals;
        std::vector<std::shared_ptr<Node>> m_pins;
        std::map<std::shared_ptr<Node>, double> m_currents;
        static int m_counter;
};

#endif