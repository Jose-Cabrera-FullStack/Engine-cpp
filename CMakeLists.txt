cmake_minimum_required(VERSION 3.8)
project(EngineCpp)

# Set compiler and flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wfatal-errors -Wno-macro-redefined")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find packages
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(Lua REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${SDL2_MIXER_INCLUDE_DIRS} ${LUA_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/lib/glm)

# Source files
file(GLOB SRC_FILES src/*.cpp src/Game/*.cpp src/Logger/*.cpp src/ECS/*.cpp src/AssetStore/*.cpp)

# Add executable
add_executable(gameengine ${SRC_FILES})

# Link libraries
target_link_libraries(gameengine SDL2_image ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${LUA_LIBRARIES})

# Ensure the build directory exists

file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/build)