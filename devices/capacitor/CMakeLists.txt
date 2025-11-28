add_library(capacitor SHARED Capacitor.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(capacitor ${Boost_LIBRARIES})