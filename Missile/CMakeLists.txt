cmake_minimum_required(VERSION 3.0)

project(missile)

add_subdirectory(source)

set_target_properties(missile
	PROPERTIES COMPILE_FLAGS "-std=c++11 -Wall")
