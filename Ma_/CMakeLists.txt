CMAKE_MINIMUM_REQUIRED(VERSION 2.6)


PROJECT(Ma)


SET(THIRD_PARTY_LIBS_DIR "C:/3rdPartyLibs")

INCLUDE_DIRECTORIES (
	${BOOST_INCLUDE_DIRS}
#	${THIRD_PARTY_LIBS_DIR}/openjpeg-1.5.0/src

	# include for _undulate
	${CMAKE_SOURCE_DIR}/externals/WBK_Lib
	)



SET(MA_SOURCES
	VecMath.cpp
	)

SET(MA_HEADERS
  MathFns.h
  MathTypes.h
  MatrixFns.h
  QuatFns.h
  VecFns.h
  VecMath.h
)

ADD_LIBRARY (Ma
  ${MA_SOURCES}
  ${MA_HEADERS}
  
 	)


