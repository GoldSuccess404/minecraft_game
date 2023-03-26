cmake_minimum_required(VERSION 3.25)
project(minecraft)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(${CMAKE_SOURCE_DIR}/lib-vc2022)

set(TARHET_H
	global_variables.hpp
	stb_image.hpp
	glfw3.hpp
	)
set(TARGET_SRC
	main.cpp
	)
	
add_executable(minecraft WIN32 ${TARGET_SRC} ${TARGET_H})

target_link_libraries(minecraft
				PRIVATE
				 glfw3
				opengl32
			   )