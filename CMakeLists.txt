cmake_minimum_required(VERSION 3.21.0)

project(Game VERSION 1.0.0)
`
# SFML linking

# set(SFML_STATIC_LIBRARIES TRUE)
set(SFML_DIR "C:/SFML-2.5.1/lib/cmake/SFML")
find_package(SFML COMPONENTS graphics audio REQUIRED)

# OpenGL linking
find_package(OpenGL REQUIRED)

# GLEW linking
set(GLEW_INCLUDE_DIRS "C:/glew-2.1.0/include")
set(GLEW_LIBRARIES "C:/glew-2.1.0/lib/Release/Win32")
set(GLEW_ROOT "C:/glew-2.1.0")
find_package(GLEW REQUIRED)

# GLM linking
set(glm_DIR "C:/glm/cmake/glm")
find_package(glm REQUIRED)

add_library(gl-shader STATIC
	src/tools/GLShader.cpp
	src/tools/GLShader.hpp
)

target_link_libraries(gl-shader OpenGL::GL GLEW::GLEW)

set(GLOB SOURCES 
	src/*.hpp
	src/*.cpp
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)
add_executable(Game src/main.cpp src/entities.cpp src/tools/utility.cpp)

target_include_directories(Game PRIVATE src)

target_link_libraries(Game sfml-graphics sfml-audio OpenGL::GL GLEW::GLEW gl-shader glm::glm)


