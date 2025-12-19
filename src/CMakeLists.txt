add_executable(${PROJECT_NAME}
    main.cpp ${CORE_BASE}/CircuitManager.cpp
    ${DEVICE_BASE}/Device.cpp
    ${CORE_BASE}/Node.cpp
    ${DEVICE_BASE}/TwoTerminal.cpp
    ${CORE_BASE}/Orchestrator.cpp
    ${CORE_BASE}/InventoryManager.cpp
    ${DISPLAY_BASE}/MainWindow.cpp
    ${DISPLAY_BASE}/MainWindow.ui
    ${DISPLAY_BASE}/inventory.cpp
    ${DISPLAY_BASE}/inventory.ui)

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Concurrent
    Eigen3::Eigen
)
