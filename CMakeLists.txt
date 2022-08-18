### Build tinysv ###
cmake_minimum_required(VERSION 3.21)
project(tinysv 
    VERSION 0.2.0
    DESCRIPTION "A minimal extensive webserver"
    LANGUAGES C)

set(VERSION ${CMAKE_PROJECT_VERSION})
configure_file(${PROJECT_SOURCE_DIR}/include/version.h.in ${PROJECT_SOURCE_DIR}/include/version.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tinysv)

# copy config & templates
file(COPY ${CMAKE_SOURCE_DIR}/sample/www DESTINATION ${CMAKE_BINARY_DIR}/tinysv)
file(COPY ${CMAKE_SOURCE_DIR}/sample/template DESTINATION ${CMAKE_BINARY_DIR}/tinysv)
file(COPY_FILE ${CMAKE_SOURCE_DIR}/config/default.xml ${CMAKE_BINARY_DIR}/tinysv/config.xml)

# collect source files
file(GLOB SOURCES src/*.c)
add_executable(tinysv ${SOURCES})
target_include_directories(tinysv PUBLIC ${PROJECT_SOURCE_DIR}/include)

# expat
find_package(EXPAT REQUIRED)
if(EXPAT_FOUND)
    target_link_libraries(tinysv PRIVATE EXPAT::EXPAT)
    target_include_directories(tinysv PRIVATE ${EXPAT_INCLUDE_DIRS})
endif()

# pthread
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(tinysv PRIVATE Threads::Threads)

# dynamic link lib
target_link_libraries(tinysv PRIVATE dl)

# subdirectory
add_subdirectory(modules)

