
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

project (cx)


set (CX_SOURCES
	Task.cpp
	System.cpp
	)

set (CX_HEADERS
	Task.h
	System.h
	)

add_library (cx
	${SY_SOURCES}
	${SY_HEADERS}
	)


