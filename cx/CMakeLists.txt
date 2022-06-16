
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project (Sy)

include_directories (
	${BOOST_INCLUDE_DIRS}
	)

set (SY_SOURCES
	Task.cpp
	System.cpp
	)

set (SY_HEADERS
	Task.h
	System.h
	)

add_library (Sy
	${SY_SOURCES}
	${SY_HEADERS}
	)


