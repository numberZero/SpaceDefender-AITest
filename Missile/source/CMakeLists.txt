set(sources
	basicworld/missile.cpp
	basicworld/object.cpp
	basicworld/world.cpp
	simulation/simulation.cpp
	main.cpp
	draw.cpp
	entry.cpp
)

set(includes
	.
)

set(libraries
	SDL2
	GL
)

include_directories(${includes})

add_executable(missile ${sources})
target_link_libraries(missile ${libraries})
