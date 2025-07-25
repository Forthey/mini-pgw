execute_process(
        COMMAND bash "${CMAKE_CURRENT_SOURCE_DIR}/dependencies/build_p7.bash" "${CMAKE_BINARY_DIR}"
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
        RESULT_VARIABLE P7_BUILD_RESULT
)

include(FetchContent)

if(NOT P7_BUILD_RESULT EQUAL 0)
    message(FATAL_ERROR "Failed to build P7 library")
endif()

FetchContent_Declare(
        libp7
        URL https://baical.net/files/libP7Client_v5.6.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
        SOURCE_DIR ${CMAKE_BINARY_DIR}/p7_source
        BINARY_DIR ${CMAKE_BINARY_DIR}/p7_build
        INSTALL_DIR ${CMAKE_BINARY_DIR}/p7_install
        CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/p7_install
)

FetchContent_MakeAvailable(libp7)

add_library(P7::Headers INTERFACE IMPORTED)
set_target_properties(P7::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_BINARY_DIR}/_deps/p7_source/Headers"
)

add_library(P7::Client STATIC IMPORTED)
set_target_properties(P7::Client PROPERTIES
        IMPORTED_LOCATION "${CMAKE_BINARY_DIR}/_deps/p7_source/lib/libp7.a"
        INTERFACE_LINK_LIBRARIES P7::Headers
)

