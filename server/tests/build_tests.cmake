SET(CMAKE_TLS_VERIFY 0)

include_directories(./source)
include_directories(./tests)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)
enable_testing()


SET(PROJECT_SOURCE_FILES
        source/logger/ILogger.h
        source/logger/P7Logger.h
        source/logger/P7Logger.cpp
        source/logger/LoggerFactory.h
        source/logger/LoggerFactory.cpp

        source/IServer.h
        source/Server.h
        source/Server.cpp

        source/socket_server/UDPServer.h
        source/socket_server/UDPServer.cpp

        source/http_server/HTTPlibServer.h
        source/http_server/HTTPlibServer.cpp

        source/session_manager/ISessionManager.h
        source/session_manager/SessionManager.h
        source/session_manager/SessionManager.cpp

        source/cdr_writer/CdrWriter.h
        source/cdr_writer/CdrWriter.cpp
        source/cdr_writer/FileHandler.h
        source/cdr_writer/FileHandler.cpp

        source/models/IJsonModelSerializer.h
        source/models/IJsonModelDeserializer.h
        source/models/http/HTTPExceptionResponse.h
        source/models/http/HTTPExceptionResponse.cpp
        source/models/Config.h
        source/models/Config.cpp
)

SET(SOURCE_TEST_FILES
        tests/SessionManagerTest.cpp
)

add_executable(test_${PROJECT_NAME} ${PROJECT_SOURCE_FILES} ${SOURCE_TEST_FILES})
target_link_libraries(test_${PROJECT_NAME} PRIVATE GTest::gtest_main GTest::gmock_main)

include(GoogleTest)
gtest_discover_tests(test_${PROJECT_NAME})

set_target_properties(test_${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin"
)

foreach (library IN LISTS INSTALLED_LIBRARIES)
    target_link_libraries(test_${PROJECT_NAME} PRIVATE ${library})
endforeach ()
