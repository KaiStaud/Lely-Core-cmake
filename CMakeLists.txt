cmake_minimum_required(VERSION 3.22)

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Set the project name
set(CMAKE_PROJECT_NAME Lely-Core-cmake)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LELY REQUIRED liblely-coapp)

# Core project settings)
project(${CMAKE_PROJECT_NAME}  VERSION 0.0.1)
message("Build type: " ${CMAKE_BUILD_TYPE})

include(cmake/version.cmake)
include(cmake/cpm.cmake)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

if(EXISTS ${CMAKE_SOURCE_DIR}/external/libeasyspin/CMakeLists.txt)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/libeasyspin)
else()
    CPMAddPackage(
        NAME libeasyspin
        GITHUB_REPOSITORY KaiStaud/libeasyspin
        GIT_TAG main
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/libeasyspin
    )
endif()

if(EXISTS ${CMAKE_SOURCE_DIR}/external/libmotion/CMakeLists.txt)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/libmotion)
else()
    CPMAddPackage(
        NAME libmotion
        GITHUB_REPOSITORY KaiStaud/libmotion
        GIT_TAG main
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/libmotion)
endif()
# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Add user sources
add_subdirectory(external/libcia402)

target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE ${LELY_CFLAGS_OTHER})


# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
    ${CMAKE_CURRENT_SOURCE_DIR}/lely-core/install/usr/local/lib/
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
    Core/bsp/can.c
    Core/bsp/ring_buffer.c
    Core/Src/stm32_time.c
    Core/Src/sdev.c
    Core/shell/src/sys_command_line.c 
    Core/shell/src/sys_queue.c 
    Core/Src/CLinearProfile.cpp
    Core/Src/CConstantVelocity.cpp
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${LELY_INCLUDE_DIRS}
    external/libmotion/constant_velocity
    external/libmotion/trapezoidal_ramp
    external
    )

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    lely-coapp  
    lely-io2  
    lely-ev  
    lely-co  
    lely-can  
    lely-util  
    lely-libc  
    libcia402
    motion_tp_ramp
    motion_constant_velocity
    )
