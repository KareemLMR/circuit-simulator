cmake_minimum_required(VERSION 3.16)
project(circuit-simulator VERSION 1.0.0 LANGUAGES CXX)

# Required for Qt Creator to recognize as Qt project
set(CMAKE_AUTOMOC ON)      # Auto-generate moc files
set(CMAKE_AUTORCC ON)      # Auto-compile Qt resource files
set(CMAKE_AUTOUIC ON)      # Auto-generate UI header files

# Required for cross-platform compilation
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Don't use GNU extensions (for MSVC compatibility)

# Optional: Better debugging in Qt Creator
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # For clangd/code completion

# Find required Qt packages
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Concurrent
)

# Alternative: If using Qt5
# find_package(Qt5 5.15 REQUIRED COMPONENTS Core Widgets Gui ...)

# Find other dependencies
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
find_package(Boost 1.74 REQUIRED COMPONENTS system filesystem)

# For Boost.DLL, might need to set this:
add_definitions(-DBOOST_DLL_USE_STD_FS)

set(DEVICE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/devices)
set(CORE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/core)
set(INTERFACE_BASE ${CMAKE_CURRENT_SOURCE_DIR}/interfaces)

include_directories(${DEVICE_BASE})
include_directories(${CORE_BASE})
include_directories(${INTERFACE_BASE})

add_compile_options(-fpermissive)
add_subdirectory(src)
add_subdirectory(devices)
