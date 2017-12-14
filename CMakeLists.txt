cmake_minimum_required(VERSION 3.8)

project(CoinBill C CXX)

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost COMPONENTS system)

set(COINBILL_TOP_DIR ${CMAKE_SOURCE_DIR})
set(COINBILL_LIB_DIR ${COINBILL_TOP_DIR}/lib/)
set(COINBILL_INC_DIR ${COINBILL_TOP_DIR}/include/)
set(COINBILL_OUT_DIR ${COINBILL_TOP_DIR}/output/)
set(COINBILL_CMAKE_DIR ${COINBILL_TOP_DIR}/cmake/)

include(${COINBILL_CMAKE_DIR}/CoinBill-Lib.cmake)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/bin)

if(WIN32)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_WINDOWS)
elseif(APPLE)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_MAC_OS)
elseif(UNIX)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_UNIX_OS)
endif()

include_directories(${COINBILL_INC_DIR})

if(Boost_FOUND)
    option(USE_BOOST_ASIO "Use socket library for boost asio." TRUE)
    if(USE_BOOST_ASIO)
        message(STATUS "CoinBill : Boost found. Using Boost ASIO.")
        include_directories(${Boost_INCLUDE_DIRS})
        list(APPEND COINBILL_TARGET_LIBRARIES           ${Boost_LIBRARIES})
        list(APPEND COINBILL_TARGET_LIBRARY_DIRS        ${Boost_LIBRARY_DIRS})
        list(APPEND COINBILL_TARGET_DEFINES             -DCOINBILL_USE_BOOST_ASIO)
    endif()
endif()

option(USE_AVX2_INSTRUCTIONS "Use AVX2 SIMD for better performance" TRUE)
if(USE_AVX2_INSTRUCTIONS)
    message(STATUS "CoinBill : AVX2 SIMD Enable.")
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_USE_AVX2)
endif()

set(BOTAN_ROOT_DIRECTORY "C:/botan" CACHE STRING "Botan Library Root Directory")
set(BOTAN_LIB_DIR ${BOTAN_ROOT_DIRECTORY}/lib/)
set(BOTAN_INC_DIR ${BOTAN_ROOT_DIRECTORY}/include/botan-2)

find_library(Botan
    NAMES botan
    PATHS ${BOTAN_ROOT_DIRECTORY}/lib
)

if((NOT EXISTS ${BOTAN_LIB_DIR}) OR (NOT EXISTS ${BOTAN_INC_DIR}) OR (NOT EXISTS ${Botan}))
    message(STATUS ${BOTAN_LIB_DIR})
    message(STATUS ${BOTAN_INC_DIR})
    message(FATAL_ERROR "CoinBill : Could not found Botan library")
endif()

message(STATUS "CoinBill : Found Botan library")
message(STATUS "  - ${Botan}")
message(STATUS "  - ${BOTAN_LIB_DIR}")
message(STATUS "  - ${BOTAN_INC_DIR}")

list(APPEND COINBILL_TARGET_LIBRARIES       ${Botan})
list(APPEND COINBILL_TARGET_LIBRARY_DIR     ${BOTAN_LIB_DIR})
include_directories(${BOTAN_INC_DIR})

add_definitions(${COINBILL_TARGET_DEFINES})
add_subdirectory(${COINBILL_LIB_DIR}/Network/)
add_subdirectory(${COINBILL_LIB_DIR}/Support/)
add_subdirectory(${COINBILL_LIB_DIR}/User/)

add_executable(CoinBillExec ${COINBILL_LIB_DIR}/CoinBill.cpp)
target_link_libraries(CoinBillExec
    CoinBillNetwork
    CoinBillSupport
    CoinBillUser
    ${COINBILL_TARGET_LIBRARIES})