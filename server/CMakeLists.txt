cmake_minimum_required(VERSION 3.22)
project(server)

SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Coverage" CACHE STRING "Доступные типы сборки" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra -Werror" CACHE STRING "Флаги отладки")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG" CACHE STRING "Флаги релиза")
set(CMAKE_CXX_FLAGS_COVERAGE "-O0 -g --coverage" CACHE STRING "Флаги покрытия кода")

set(CMAKE_EXE_LINKER_FLAGS_DEBUG "-fsanitize=address" CACHE STRING "Флаги линковки отладки")
set(CMAKE_EXE_LINKER_FLAGS_COVERAGE "--coverage" CACHE STRING "Флаги линковки покрытия")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_CURRENT_SOURCE_DIR}/bin/debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_CURRENT_SOURCE_DIR}/bin/release")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_COVERAGE "${CMAKE_CURRENT_SOURCE_DIR}/bin/coverage")

include(dependencies/install_all.cmake)

SET(SOURCE_FILES
        source/main.cpp

        source/logger/ILogger.h
        source/logger/P7Logger.h
        source/logger/P7Logger.cpp
        source/logger/LoggerFactory.cpp
        source/logger/LoggerFactory.h

)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:${CMAKE_CXX_FLAGS_DEBUG}>
        $<$<CONFIG:Release>:${CMAKE_CXX_FLAGS_RELEASE}>
        $<$<CONFIG:Coverage>:${CMAKE_CXX_FLAGS_COVERAGE}>
)

target_link_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:${CMAKE_EXE_LINKER_FLAGS_DEBUG}>
        $<$<CONFIG:Coverage>:${CMAKE_EXE_LINKER_FLAGS_COVERAGE}>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Debug>:DEBUG_MODE=1>
        $<$<CONFIG:Release>:RELEASE_MODE=1>
        $<$<CONFIG:Coverage>:COVERAGE_MODE=1>
)

foreach (library IN LISTS INSTALLED_LIBRARIES)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${library})
endforeach ()
