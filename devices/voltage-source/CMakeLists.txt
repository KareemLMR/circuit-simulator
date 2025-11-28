add_library(voltage-source SHARED VoltageSource.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(voltage-source ${Boost_LIBRARIES})
