
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project (mtool)


set (MTOOL_HEADER_FILES
	mtool.h
	)
	
set (MTOOL_SOURCE_FILES
	cube.cpp
	)

add_executable (mtool
	${CUBES_HEADER_FILES}
	${CUBES_SOURCE_FILES}
	)


target_link_libraries (mtool
  cx
  SDL2
  SDL2main
  )



