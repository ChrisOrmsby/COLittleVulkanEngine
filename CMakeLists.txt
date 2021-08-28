# Project information
cmake_minimum_required(VERSION 3.18)
project(COLittleVulkan)
# C++ configuration
set(CMAKE_CXX_STANDARD 20)
# TODO I don't really like throwing all the shaders in one folder
#  I'd prefer if they were bundled with the code that uses them, but the project isn't big enough
#  to warran those types of changes yet; plus this is easier.
function(add_shader TARGET SHADER)
    # Find GLSLC for building sprv shaders
    find_program(GLSLC glslc)
    set(current-shader-path ${CMAKE_CURRENT_SOURCE_DIR}/shaders/${SHADER})
    set(current-output-path ${CMAKE_BINARY_DIR}/shaders/${SHADER}.spv)
    # Add a custom command to compile GLSL to SPIR-V.
    get_filename_component(current-output-dir ${current-output-path} DIRECTORY)
    file(MAKE_DIRECTORY ${current-output-dir})
    add_custom_command(
           OUTPUT ${current-output-path}
           COMMAND ${GLSLC} -o ${current-output-path} ${current-shader-path}
           DEPENDS ${current-shader-path}
           IMPLICIT_DEPENDS CXX ${current-shader-path}
           VERBATIM)
    # Make sure our build depends on this output.
    set_source_files_properties(${current-output-path} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${current-output-path})
endfunction(add_shader)
find_package(glfw3 REQUIRED)
# External C++ packages
find_package(Threads REQUIRED)
find_package(Vulkan REQUIRED)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_executable(main ${SOURCES})
# TODO maybe glob these instead
# Add shaders to the build system
add_shader(main triangle.vert)
add_shader(main triangle.frag)
# Link all external libs to main
target_link_libraries(main glfw)
target_link_libraries(main Threads::Threads)
target_link_libraries(main Vulkan::Vulkan)