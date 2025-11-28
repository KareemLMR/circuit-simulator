add_library(resistor SHARED Resistor.cpp /${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(resistor ${Boost_LIBRARIES})
