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


if(EXISTS ${CMAKE_SOURCE_DIR}/external/libcia402/CMakeLists.txt)
    add_subdirectory(${CMAKE_SOURCE_DIR}/external/libcia402)
else()
    CPMAddPackage(
        NAME libcia402
        GITHUB_REPOSITORY KaiStaud/libcia402
        GIT_TAG main
        SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/libcia402)
endif()

add_subdirectory(Core/Config)

add_library(ugui STATIC
    external/UGUI/ugui.c
    external/UGUI/ugui_button.c
    external/UGUI/ugui_checkbox.c
    external/UGUI/ugui_image.c
    external/UGUI/ugui_textbox.c
    external/UGUI/ugui_progress.c
    external/UGUI/Fonts/arial_6X6.c
    external/UGUI/Fonts/arial_6X6_cyrillic.c
    external/UGUI/Fonts/arial_9X10.c
    external/UGUI/Fonts/arial_9X10_cyrillic.c
    external/UGUI/Fonts/arial_10X13.c
    external/UGUI/Fonts/arial_10X13_cyrillic.c
    external/UGUI/Fonts/arial_12X15.c
    external/UGUI/Fonts/arial_12X15_cyrillic.c
    external/UGUI/Fonts/arial_16X18.c
    external/UGUI/Fonts/arial_16X18_cyrillic.c
    external/UGUI/Fonts/arial_20X23.c
    external/UGUI/Fonts/arial_20X23_cyrillic.c
    external/UGUI/Fonts/arial_25X28.c
    external/UGUI/Fonts/arial_25X28_cyrillic.c
    external/UGUI/Fonts/arial_29X35.c
    external/UGUI/Fonts/arial_29X35_cyrillic.c
    external/UGUI/Fonts/arial_35X40.c
    external/UGUI/Fonts/arial_35X40_cyrillic.c
    external/UGUI/Fonts/arial_39X45.c
    external/UGUI/Fonts/arial_39X45_cyrillic.c
    external/UGUI/Fonts/arial_45X52.c
    external/UGUI/Fonts/arial_45X52_cyrillic.c
    external/UGUI/Fonts/arial_49X57.c
    external/UGUI/Fonts/arial_49X57_cyrillic.c
    external/UGUI/Fonts/font_4x6.c
    external/UGUI/Fonts/font_5x8.c
    external/UGUI/Fonts/font_5x12.c
    external/UGUI/Fonts/font_6x8.c
    external/UGUI/Fonts/font_6x10.c
    external/UGUI/Fonts/font_7x12.c
    external/UGUI/Fonts/font_8x8.c
    external/UGUI/Fonts/font_8x12.c
    external/UGUI/Fonts/font_8x12_cyrillic.c
    external/UGUI/Fonts/font_8x14.c
    external/UGUI/Fonts/font_10x16.c
    external/UGUI/Fonts/font_12x16.c
    external/UGUI/Fonts/font_12x20.c
    external/UGUI/Fonts/font_16x26.c
    external/UGUI/Fonts/font_22x36.c
    external/UGUI/Fonts/font_24x40.c
    external/UGUI/Fonts/font_32x53.c)
target_include_directories(ugui PUBLIC
    external/UGUI)

add_library(display STATIC
    external/LCD/lcd.c
    #external/LCD/images.c
    )
target_include_directories(display PUBLIC
    external/LCD)
target_link_libraries(display PUBLIC
    ugui
    stm32cubemx)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME}
    Core/Src/cli_handlers.c)

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)
add_subdirectory(external/libsht4x)
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
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/FreeRTOS_CLI.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Src/app_cli.c
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Config/FRAM.cpp
    )

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
    ${LELY_INCLUDE_DIRS}
    external/libmotion/LinearProfiles
    external/libmotion/TrapezoidalRamp
    external/libmotion/MotionManager
    external/libcia402/Statemachine
    external/libsht4x
    Core/Config
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
    cia402
    motion_linear_profiles
    motion_manager
    motion_linear_profiles
    motion_homing
    display
    ugui
    sht4x
#    motion_constant_velocity
    )
