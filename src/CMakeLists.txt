find_package(Boost REQUIRED COMPONENTS system filesystem)

add_executable(${PROJECT_NAME} main.cpp ${CORE_BASE}/CircuitManager.cpp ${DEVICE_BASE}/Device.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${CORE_BASE}/Orchestrator.cpp)
include_directories(${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})