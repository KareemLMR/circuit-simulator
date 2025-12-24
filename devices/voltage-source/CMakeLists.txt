add_library(voltage-source SHARED VoltageSource.cpp ${CORE_BASE}/Node.cpp ${DEVICE_BASE}/TwoTerminal.cpp ${DEVICE_BASE}/Device.cpp)
target_link_libraries(voltage-source PRIVATE
    ${Boost_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    Qt5::Gui
    Qt5::Concurrent
    Eigen3::Eigen
)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/devices/voltage-source)
configure_file(
    ${DEVICE_BASE}/voltage-source/voltage-source.png
    ${CMAKE_CURRENT_BINARY_DIR}/voltage-source.png
    COPYONLY
)
