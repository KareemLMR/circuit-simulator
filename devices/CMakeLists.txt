# Find required Qt packages
find_package(Qt5 REQUIRED COMPONENTS
    Core
    Widgets
    Gui
    Concurrent
)

add_subdirectory(resistor)
add_subdirectory(inductor)
add_subdirectory(capacitor)
add_subdirectory(voltage-source)
add_subdirectory(current-source)
