
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project (spux)

include_directories (
	${BOOST_INCLUDE_DIRS}
	)

set (SY_SOURCES
	Shared.cpp
	Runnable.cpp
	System.cpp
	)

set (SY_HEADERS
	spux.h
	Shared.h
	Runnable.h
	System.h
	)

add_library (spux
	${SY_SOURCES}
	${SY_HEADERS}
	)

