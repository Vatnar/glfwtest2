add_library(Logger STATIC src/Logger.cpp)
target_include_directories(Logger PUBLIC include)
target_link_libraries(Logger PUBLIC fmt::fmt)