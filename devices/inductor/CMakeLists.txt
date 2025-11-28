add_library(inductor SHARED Inductor.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(inductor ${Boost_LIBRARIES})
