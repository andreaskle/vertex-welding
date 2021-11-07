PROJECT(vertex-welding)
add_executable(vertex-welding main.cpp scene.h scene.cpp camera.h camera.cpp vertex.h vertex_welder.h vertex_welder.cpp)
target_link_libraries(vertex-welding opengl glad glfw glm )
