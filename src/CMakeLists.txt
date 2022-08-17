file(GLOB SOURCES *.c)
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