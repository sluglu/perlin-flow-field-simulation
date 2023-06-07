cmake_minimum_required(VERSION 3.5)
project(opengl-perlin-flow-field)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_TOOLCHAIN_FILE "C:/Program Files/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

find_package(freeglut REQUIRED)
include_directories(${FREEGLUT_INCLUDE_DIRS})


# Include directories
include_directories(include)

# Add all header files in the include folder
file(GLOB HEADER_FILES include/*.h)

# Add all source files in the src folder
file(GLOB SOURCE_FILES src/*.cpp)


# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE ${FREEGLUT_LIBRARIES})