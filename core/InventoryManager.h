#ifndef INVENTORY_MANAGER_H_
#define INVENTORY_MANAGER_H_

#include <memory>
#include <vector>
#include <atomic>
#include <string>
#include <dirent.h>
#include <errno.h>
#include "qdebug.h"

class InventoryManager
{
public:
    InventoryManager();

    InventoryManager(const InventoryManager& other) = delete;
    InventoryManager(const InventoryManager&& other) = delete;

    InventoryManager& operator=(const InventoryManager& other) = delete;
    InventoryManager& operator=(const InventoryManager&& other) = delete;

    bool init(std::string devicesPath);
    std::vector<std::string> getSupportedDevices(void);

    static InventoryManager& getInstance(void);

    bool isInitialized(void);

    ~InventoryManager();

private:
    static std::unique_ptr<InventoryManager> m_instance;
    std::vector<std::string> m_supportedDevices;
    std::atomic<bool> m_isInitialized;
};

#endif
