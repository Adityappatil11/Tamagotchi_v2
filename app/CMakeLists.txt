add_library(app_logic 
    src/PetLogic.cpp 
    src/CommandParser.cpp 
)

#define include directories
target_include_directories(app_logic PUBLIC include)

#if building shared, need to handle symbol visibility
if(BUILD_SHARED_LIBS)
    set_target_properties(app_logic PROPERTIES C_VISIBILITY_PRESET hidden VISIBILITY_INLINES_HIDDEN 1)
endif()