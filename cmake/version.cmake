string(TIMESTAMP BUILD_TIME "%Y-%m-%d %H:%M:%S")
execute_process(
        COMMAND git rev-parse --short HEAD
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        OUTPUT_VARIABLE APP_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/version.h.in
    ${CMAKE_CURRENT_SOURCE_DIR}/Core/Inc/version.h
    @ONLY
)

message("Git-Hash ${APP_GIT_HASH}")
message("Version ${PROJECT_VERSION}")