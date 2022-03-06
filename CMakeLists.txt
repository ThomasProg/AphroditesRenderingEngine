cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB SOURCES src/*)
set(LibName "AphroditesRenderingEngine")
project(${LibName})

# Add glm
add_subdirectory(ThirdParties/glm)

# Add vulkan
add_subdirectory(ThirdParties/vulkan)
include_directories(AFTER ${VULKAN_INCLUDE_PATH})
link_directories(${VULKAN_LINK_PATH})

# Create library
add_library(${LibName} ${SOURCES})
target_link_libraries(${LibName} glm ${VULKAN_LIB_LIST})
target_include_directories(${LibName} PUBLIC "${PROJECT_SOURCE_DIR}/include/" "${PROJECT_SOURCE_DIR}/ThirdParties/glm")

