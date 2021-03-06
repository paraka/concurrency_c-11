cmake_minimum_required (VERSION 2.8.11)
project(Concurrency)
find_package (Threads)

# We need C++ 11
if (${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION} GREATER 3.0)
  # CMake 3.1 has built-in CXX standard checks.
  set(CMAKE_CXX_STANDARD 11)
  set(CMAKE_CXX_STANDARD_REQUIRED on)
else ()
  if (CMAKE_COMPILER_IS_GNUCXX)
    message ("Assuming that GNU CXX uses -std=c++11 flag for C++11 compatibility.")
    list(APPEND CMAKE_CXX_FLAGS "-std=c++11")
  else()
    message ("No CMAKE C++11 check. If the build breaks, you're on your own.")
  endif()
endif ()

set(Concurrency_SRCS main.cpp)
add_executable(concurrency_test ${Concurrency_SRCS})
target_link_libraries(concurrency_test ${CMAKE_THREAD_LIBS_INIT})
install(TARGETS concurrency_test DESTINATION bin)
