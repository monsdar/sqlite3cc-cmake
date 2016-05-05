PROJECT(sqlite3cc)
cmake_minimum_required(VERSION 2.8)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

file(GLOB HEADERS ${CMAKE_SOURCE_DIR}/sqlite3cc-0.1.1/include/*.h)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/sqlite3cc-0.1.1/src/*.cc)

include_directories(${CMAKE_SOURCE_DIR}/sqlite3cc-0.1.1/include ${CONAN_INCLUDE_DIRS})
add_library(sqlite3cc STATIC ${SOURCES})
target_link_libraries(sqlite3cc ${CONAN_LIBS})

install(FILES ${HEADERS} DESTINATION include)
install(TARGETS sqlite3cc LIBRARY DESTINATION lib ARCHIVE DESTINATION lib)
