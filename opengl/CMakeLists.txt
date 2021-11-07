project(opengl)
add_library(opengl utils.h utils.cpp)
target_link_libraries(opengl glad)