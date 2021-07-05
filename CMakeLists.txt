cmake_minimum_required(VERSION 3.10)

add_subdirectory("vendor/SFML")
add_subdirectory("vendor/glm")

project(Pendulum)

set(CMAKE_CXX_STANDARD 17)

link_directories("vendor/SFML/lib")

file(GLOB Psrc "*.h" "*.cpp")
add_executable(Pendulum ${Psrc})

target_link_libraries(Pendulum debug sfml-graphics-d.lib optimized sfml-graphics.lib)
target_link_libraries(Pendulum debug sfml-window-d.lib optimized sfml-window.lib)
target_link_libraries(Pendulum debug sfml-system-d.lib optimized sfml-system.lib)

target_include_directories(Pendulum PRIVATE "vendor/glm")
target_include_directories(Pendulum PRIVATE "vendor/SFML/include")

configure_file(vendor/SFML/lib/Debug/sfml-graphics-d-2.dll COPYONLY Debug/sfml-graphics-d-2.dll)
configure_file(vendor/SFML/lib/Debug/sfml-window-d-2.dll COPYONLY Debug/sfml-window-d-2.dll)
configure_file(vendor/SFML/lib/Debug/sfml-system-d-2.dll COPYONLY Debug/sfml-system-d-2.dll)

configure_file(vendor/SFML/lib/Release/sfml-graphics-2.dll COPYONLY Release/sfml-graphics-2.dll)
configure_file(vendor/SFML/lib/Release/sfml-window-2.dll COPYONLY Release/sfml-window-2.dll)
configure_file(vendor/SFML/lib/Release/sfml-system-2.dll COPYONLY Release/sfml-system-2.dll)

file(COPY vendor/SFML/lib/Debug/sfml-graphics-d-2.dll DESTINATION Debug/)
file(COPY vendor/SFML/lib/Debug/sfml-window-d-2.dll DESTINATION Debug/)
file(COPY vendor/SFML/lib/Debug/sfml-system-d-2.dll DESTINATION Debug/)

file(COPY vendor/SFML/lib/Release/sfml-graphics-2.dll DESTINATION Release)
file(COPY vendor/SFML/lib/Release/sfml-window-2.dll DESTINATION Release)
file(COPY vendor/SFML/lib/Release/sfml-system-2.dll DESTINATION Release)