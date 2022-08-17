### Build modules for tinysv ###
cmake_minimum_required(VERSION 3.10)
project(tinysv_modules
    DESCRIPTION "module SDK for tinysv"
    LANGUAGES C)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/modules)
else()
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tinysv/modules)
endif()

#[[ 
#########################
## New module template ##
#########################    

    add_library(<module_name>
        SHARED
            <module_source>

            # you must add sources below
            ${PROJECT_SOURCE_DIR}/src/hashmap.c
            ${PROJECT_SOURCE_DIR}/src/logger.c
        )
    set_property(TARGET <module_name> PROPERTY POSITION_INDEPENDENT_CODE ON)
    set_target_properties(<module_name> PROPERTIES PREFIX "")
    target_include_directories(<module_name> PRIVATE ${PROJECT_SOURCE_DIR}/include)

    # you can add another sources or libraries as you need
]]

# default module
add_library(default_module 
    SHARED 
        default_module.c
        ${PROJECT_SOURCE_DIR}/../src/hashmap.c
        ${PROJECT_SOURCE_DIR}/../src/logger.c
    )
set_property(TARGET default_module PROPERTY POSITION_INDEPENDENT_CODE ON)
set_target_properties(default_module PROPERTIES PREFIX "")
target_include_directories(default_module PRIVATE ${PROJECT_SOURCE_DIR}/../include)
