#include "InventoryManager.h"
#include <cstring>

std::unique_ptr<InventoryManager> InventoryManager::m_instance = nullptr;

InventoryManager::InventoryManager()
{
    m_isInitialized.store(false);
}

InventoryManager& InventoryManager::getInstance(void)
{
    if (m_instance == nullptr)
    {
        m_instance = std::make_unique<InventoryManager>();
    }
    return *m_instance;
}

bool InventoryManager::init(std::string devicesPath)
{
    bool ret = false;
    struct dirent *entry;
    DIR* dir = opendir(devicesPath.c_str());
    if (m_isInitialized)
    {
        qDebug() << "InventoryManager is already initialized!";
        m_supportedDevices.clear();
    }
    if (dir)
    {
        while ((entry = readdir(dir)) != NULL)
        {
            // Skip "." (current directory) and ".." (parent directory)
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }

            // Check if the entry is a directory
            // d_type is a non-standard field in the dirent struct, but commonly available on POSIX systems.
            // If d_type is not available, you would need to use stat() on the full path to check the file type.
            if (entry->d_type == DT_DIR)
            {
                FILE *file;
                // Try to open the file in read mode ("r")
                std::string lib = devicesPath + entry->d_name + "/lib" + entry->d_name + ".so";
                if ((file = fopen(lib.c_str(), "r")) != NULL)
                {
                    fclose(file);
                    m_supportedDevices[entry->d_name]++;
                }
            }
        }
        closedir(dir);
        ret = true;;
    }
    else
    {
        ret = false;
    }
    m_isInitialized.store(ret);
    return ret;
}

bool InventoryManager::isInitialized(void)
{
    return m_isInitialized.load();
}

std::map<std::string, int>& InventoryManager::getSupportedDevices(void)
{
    return m_supportedDevices;
}

InventoryManager::~InventoryManager()
{
    m_supportedDevices.clear();
    m_isInitialized.store(false);
}
