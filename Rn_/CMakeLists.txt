
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project (Rn)

include_directories (
	${BOOST_INCLUDE_DIRS}
#	${THIRD_PARTY_LIBS_DIR}/openjpeg-1.5.0/src

	# include for _undulate
	${CMAKE_SOURCE_DIR}/externals/WBK_Lib
	)

set (RN_SOURCES
	Render.cpp
	CLoD_terrain.cpp
	)

set (RN_HEADERS
	Render.h
	CLoD_terrain.h
	)

add_library (Rn
	${RN_SOURCES}
	${RN_HEADERS}
	)


