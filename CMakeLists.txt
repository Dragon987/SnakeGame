cmake_minimum_required(VERSION 3.14)
project(SnakeGame VERSION 1.0)

add_executable(
	app
	main.cpp
)

target_link_libraries(
	app
	-lengine
	-lMath
)
