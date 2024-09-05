file(GLOB_RECURSE CPP_SOURCES "*.cpp")

add_library(db ${CPP_SOURCES})

target_include_directories(db PUBLIC include)
