cmake_minimum_required(VERSION 3.5)
project(12_multiple_lights)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/12_multiple_lights)

set(UTILS_SOURCE_FILES ../utils/file_reader.cc ../utils/file_reader.hh
        ../utils/shader_compiler.cc ../utils/shader_compiler.hh ../utils/shader.cc ../utils/shader.hh ../utils/camera.hh ../utils/camera.cc)
set(TUTO_SOURCE_FILES tuto/main.cc)

add_executable(multiple_lights ${TUTO_SOURCE_FILES} ${UTILS_SOURCE_FILES})

target_link_libraries(multiple_lights; ${SFML_LIBRARIES}; ${GLEW_LIBRARIES}; ${SOIL_LIBRARIES}; GL)
