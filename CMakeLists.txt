cmake_minimum_required(VERSION 3.8)

# Coinbill project, we are using C/C++.
project(CoinBill C CXX)

# We are using OpenSSL for cryption, Boost for socket.
find_package(CryptoPP   REQUIRED)
find_package(Boost      COMPONENTS system)

# Initialize default directories.
set(COINBILL_TOP_DIR ${CMAKE_SOURCE_DIR})
set(COINBILL_LIB_DIR ${COINBILL_TOP_DIR}/lib/)
set(COINBILL_INC_DIR ${COINBILL_TOP_DIR}/include/)
set(COINBILL_OUT_DIR ${COINBILL_TOP_DIR}/output/)
set(COINBILL_CMAKE_DIR ${COINBILL_TOP_DIR}/cmake/)

include(${COINBILL_CMAKE_DIR}/CoinBill-Lib.cmake)

# Initialize output directories.
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${COINBILL_OUT_DIR}/bin)

# Target type defines.
if(WIN32)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_WINDOWS)
elseif(APPLE)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_MAC_OS)
elseif(UNIX)
    list(APPEND COINBILL_TARGET_DEFINES -DCOINBILL_UNIX_OS)
endif()

include_directories(${COINBILL_INC_DIR})
list(APPEND COINBILL_TARGET_LIBRARIES cryptopp-static)

# Boost defines.
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

# Add and initialize subdirectories.
add_definitions(${COINBILL_TARGET_DEFINES})
add_subdirectory(${COINBILL_LIB_DIR}/Network/)
add_subdirectory(${COINBILL_LIB_DIR}/Support/)
add_subdirectory(${COINBILL_LIB_DIR}/User/)

# Now compile it.
add_executable(CoinBillExec ${COINBILL_LIB_DIR}/CoinBill.cpp)
target_link_libraries(CoinBillExec
    CoinBillNetwork 
    CoinBillSupport 
    CoinBillUser
    ${COINBILL_TARGET_LIBRARIES})