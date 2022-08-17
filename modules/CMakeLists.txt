#[[ 
########################
# New module template #
#######################    

    add_library(<module_name>
        SHARED
            <module_source>

            # you must add sources below
            ${PROJECT_SOURCE_DIR}/src/hashmap.c
            ${PROJECT_SOURCE_DIR}/src/logger.c
        )
    set_property(TARGET <module_name> PROPERTY POSITION_INDEPENDENT_CODE ON)
    target_include_directories(<module_name> PRIVATE ${PROJECT_SOURCE_DIR}/include)

    # you can add another sources or libraries as you need
]]

# default module
add_library(default_module 
    SHARED 
        default_module.c
        ${PROJECT_SOURCE_DIR}/src/hashmap.c
        ${PROJECT_SOURCE_DIR}/src/logger.c
    )
set_property(TARGET default_module PROPERTY POSITION_INDEPENDENT_CODE ON)
target_include_directories(default_module PRIVATE ${PROJECT_SOURCE_DIR}/include)