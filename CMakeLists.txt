cmake_minimum_required(VERSION 3.5)
project(opengl-perlin-flow-field)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)

# Include directories
include_directories(include freeglut/include)

# Add all header files in the include folder
file(GLOB HEADER_FILES include/*.h)

# Add all source files in the src folder
file(GLOB SOURCE_FILES src/*.cpp)


# Add an executable target
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

#target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/freeglut/lib/freeglut.lib)

add_subdirectory(barebone-engine)

target_link_libraries(${PROJECT_NAME} barebone-engine)
