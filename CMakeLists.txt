cmake_minimum_required(VERSION 3.8)
project(EngineCpp)

# Set compiler and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wno-macro-redefined")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL2 and its components
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Lua REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${LUA_INCLUDE_DIR})

# Source files
file(GLOB SRC_FILES src/*.cpp)

# Add executable
add_executable(gameengine ${SRC_FILES})

# Link libraries
target_link_libraries(gameengine ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${LUA_LIBRARIES})

# Ensure the build directory exists
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)

# Copy shaders to build directory
file(COPY src/shaders DESTINATION ${CMAKE_BINARY_DIR}/build)