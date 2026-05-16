include(FetchContent)

#FetchContent_Declare(
#    pugixml
#    GIT_REPOSITORY https://github.com/zeux/pugixml.git
#    GIT_TAG v1.15
#)
#FetchContent_MakeAvailable(pugixml)

add_library(motion_config INTERFACE)
target_include_directories(motion_config
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/Backends
        ${json_SOURCE_DIR}/include
)
#target_link_libraries(motion_config INTERFACE pugixml)
