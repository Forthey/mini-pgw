include(FetchContent)

FetchContent_Declare(
        libp7
        URL https://baical.net/files/libP7Client_v5.6.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(libp7)

if("${libp7_SOURCE_DIR}" STREQUAL "" OR "${libp7_BINARY_DIR}" STREQUAL "")
    message(FATAL_ERROR "Could not download library libp7")
endif()

execute_process(
        COMMAND ${CMAKE_COMMAND} -S ${libp7_SOURCE_DIR} -B ${libp7_BINARY_DIR} -DCMAKE_INSTALL_PREFIX=${libp7_BINARY_DIR}
)

execute_process(
        COMMAND ${CMAKE_COMMAND} --build ${libp7_BINARY_DIR}
)

file(COPY ${libp7_SOURCE_DIR}/Headers DESTINATION ${libp7_BINARY_DIR}/Headers)

add_library(P7::Headers INTERFACE IMPORTED)
set_target_properties(P7::Headers PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${libp7_BINARY_DIR}/Headers"
)

add_library(P7::Client STATIC IMPORTED)
set_target_properties(P7::Client PROPERTIES
        IMPORTED_LOCATION "${libp7_BINARY_DIR}/Sources/libp7.a"
        INTERFACE_LINK_LIBRARIES P7::Headers
)