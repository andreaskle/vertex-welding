cmake_minimum_required(VERSION 3.14)
include(FetchContent)
project(vertex-welding)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)

if(OPENGL_FOUND)
  include_directories(  
    ${OPENGL_INCLUDE_DIR} 
  )
  link_directories(
    ${OPENGL_LIBRARY_DIRS}
  )   
endif() 

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 0.9.9.7
)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.3.2
)

FetchContent_MakeAvailable(glm glfw)

include_directories(external/stb)
include_directories(${vertex-welding_SOURCE_DIR})
add_subdirectory(external)
add_subdirectory(opengl)
add_subdirectory(vertex-welding)
