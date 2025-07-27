include(${CMAKE_CURRENT_LIST_DIR}/p7.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/nlohmann_json.cmake)

set(INSTALLED_LIBRARIES
        P7::Client
        nlohmann_json::nlohmann_json
)
