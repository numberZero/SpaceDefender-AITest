set(sources
	basicworld/missile.cpp
	basicworld/object.cpp
	basicworld/world.cpp
	simulation/simulation.cpp
	main.cpp
	draw.cpp
	system.cpp
)

find_path(Eigen NAMES signature_of_eigen3_matrix_library Eigen)
find_library(SDL SDL2)
find_library(OpenGL NAMES GL opengl32 NAMES_PER_DIR)

set(includes
	.
	${Eigen}
)

set(libraries
	${SDL}
	${OpenGL}
)

include_directories(${includes})

add_executable(missile ${sources})
target_link_libraries(missile ${libraries})
