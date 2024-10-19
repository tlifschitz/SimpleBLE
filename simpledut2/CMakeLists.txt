cmake_minimum_required(VERSION 3.21)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/prelude.cmake)

project(
    simpledut
    VERSION 1.0.0
    LANGUAGES CXX
)

include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/epilogue.cmake)

include(GenerateExportHeader)
include(GNUInstallDirs)


set(SIMPLEBLE_PRIVATE_INCLUDES
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include)


add_executable(simpledut
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

set_target_properties(simpledut PROPERTIES
    CXX_VISIBILITY_PRESET hidden
    VISIBILITY_INLINES_HIDDEN YES
    CXX_STANDARD 17
    POSITION_INDEPENDENT_CODE ON
    WINDOWS_EXPORT_ALL_SYMBOLS ON)

target_include_directories(simpledut PRIVATE ${SIMPLEBLE_PRIVATE_INCLUDES})


list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEBLE_LOG_LEVEL=SIMPLEBLE_LOG_LEVEL_${SIMPLEBLE_LOG_LEVEL})
list(APPEND PRIVATE_COMPILE_DEFINITIONS SIMPLEBLE_VERSION="${PROJECT_VERSION}")

# Detect the operating system and load the necessary dependencies
if(PLATFORM STREQUAL "plain")
    message(STATUS "Building SimpleDut Plain")
    target_include_directories(simpledut PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port/plain)

elseif (PLATFORM STREQUAL "esp32")
    message(STATUS "Building SimpleDut ESP32")
    target_include_directories(simpledut PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/port/esp32)
    include(${CMAKE_CURRENT_SOURCE_DIR}/src/port/esp32/port.cmake)
else()
    message(FATAL_ERROR "Unknown platform: ${PLATFORM}")
endif()

apply_build_options(simpledut
    "${PRIVATE_COMPILE_DEFINITIONS}"
    "${PRIVATE_COMPILE_OPTIONS}"
    "${PRIVATE_LINK_OPTIONS}"
    "${PUBLIC_LINK_OPTIONS}")
