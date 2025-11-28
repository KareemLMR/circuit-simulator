add_library(current-source SHARED CurrentSource.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(current-source ${Boost_LIBRARIES})
