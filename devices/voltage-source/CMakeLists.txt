add_library(voltage-source SHARED VoltageSource.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(voltage-source PRIVATE
    ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Concurrent
    Eigen3::Eigen
)
