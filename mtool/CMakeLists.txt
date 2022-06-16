
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project (mtool)


set (MTOOL_HEADER_FILES
  mtool.h
  )
	
set (MTOOL_SOURCE_FILES
  mtool.cpp
  )

add_executable (mtool
  ${MTOOL_HEADER_FILES}
  ${MTOOL_SOURCE_FILES}
  )


target_link_libraries (mtool
  cx
  SDL2
  SDL2main
  )



