cmake_minimum_required(VERSION 3.5)
project(opengl_tutorials)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
# Detect and add GLEW
find_package(GLEW REQUIRED)
if (!GLEW_FOUND)
    MESSAGE("GLEW NOT FOUND")
endif()
include_directories(${GLEW_INCLUDE_DIRS})

# Detect and add SFML
find_package(SFML 2 REQUIRED system window graphics network audio)
if (!SFML_FOUND)
    MESSAGE("SFML NOT FOUND")
endif()
include_directories(${SFML_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
if (!OPENGL_FOUND)
    MESSAGE("OPENGL NOT FOUND")
endif()
include_directories(${GLEW_INCLUDE_DIRS})

find_package(SOIL REQUIRED)
if (!SOIL_FOUND)
    MESSAGE("SOIL NOT FOUND")
endif()
include_directories(${SOIL_INCLUDE_DIRS})

find_package(assimp REQUIRED)
if (!ASSIMP_FOUND)
    MESSAGE("ASSIMP NOT FOUND")
endif()
include_directories(${ASSIMP_INCLUDE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLM DEFAULT_MSG GLM_INCLUDE_DIR)
# Define GLM_INCLUDE_DIRS
if (GLM_FOUND)
    set(GLM_INCLUDE_DIRS ${GLM_INCLUDE_DIR})
endif()

# Hide some variables
mark_as_advanced(GLM_INCLUDE_DIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(UTILS_DIR ${CMAKE_SOURCE_DIR}/utils)
include_directories(${UTILS_DIR})
add_subdirectory(1_hello_triangle)
add_subdirectory(2_shaders)
add_subdirectory(3_textures)
add_subdirectory(4_transformations)
add_subdirectory(5_coordinate_systems)
add_subdirectory(6_camera)
add_subdirectory(7_colors)
add_subdirectory(8_basic_lighting)
add_subdirectory(9_materials)
add_subdirectory(10_lighting_maps)
add_subdirectory(11_light_casters)
add_subdirectory(12_multiple_lights)
add_subdirectory(13_model_loading)
add_subdirectory(14_depth_testing)
add_subdirectory(15_stencil_testing)
add_subdirectory(bump_mapping)
