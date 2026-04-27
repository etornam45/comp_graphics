cmake_minimum_required(VERSION 4.0)

project(live_coding VERSION 1.0.0)

# list(APPEND CMAKE_PREFIX_PATH "/opt/homebrew")

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)


add_executable(${PROJECT_NAME} 
	src/main.cpp
	# src/pipeline.cpp
	src/glad.c
)

target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)