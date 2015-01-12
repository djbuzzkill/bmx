
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (Dx)


message (STATUS "${BULLET_SDK_ROOT}")

# include_directories (
# 	${BOOST_INCLUDE_DIRS}
# 	)
# 
# 
# link_directories (
# 	)

# target_link_libraries (
# 	)

set (DX_HEADER_FILES
	"Bounding.h"
	"Collision.h"
	"Data.h"
	"Dx.h"
	"GlobalTypes.h"
	"IKSolver2d.h"
	"Interface.h"
	"MatFn.h"
	"MathTypes.h"
	"PhysicsSys.h"
	"QuatFn.h"
	"Render.h"
	"Shapes.h"
	"System.h"
	"Terrain.h"
	"Utility.h"
	"VecFn.h"
	"VecMath.h"	


#	"Anim_biped.h"
#	"BG_renderer.h"
#	"Goby.h"
#	"Horde3D_platform.h"
#	"Scene.h"
	)

set (DX_SOURCE_FILES
	"Collision.cpp"
	"Data.cpp"
	"IKSolver2d.cpp"
	"Interface.cpp"
	"PhysicsSys.cpp"
	"Render.cpp"
	"System.cpp"
	"Terrain.cpp"
	"Utility.cpp"

#	"BG_renderer.cpp"
#	"Goby.cpp"
#	"Anim_biped.cpp"
#	"Horde3D_platform.cpp"
#	"Scene.cpp"
	)


add_library (Dx
	${DX_HEADER_FILES}
	${DX_SOURCE_FILES}
	)







