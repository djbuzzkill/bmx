

SET (PROG_NAME gfxlife)
project (${PROG_NAME})


FIND_PACKAGE (Boost REQUIRED COMPONENTS system date_time filesystem)
#FIND_PACKAGE (TBB)


message (STATUS "${PROG_NAME} CMake" )
    

message (STATUS "${PROG_NAME}::GLM_ROOT : $ENV{GLM_ROOT}" )
    
message (STATUS "SPUX_ROOT : $ENV{SPUX_ROOT}" )
message (STATUS "SPUX_SDL2_ROOT : $ENV{SPUX_SDL2_ROOT}" )


    

# target_include_directories (
#   ${PROG_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
#   )

# install(TARGETS ntf_inflater DESTINATION "." COMPONENT Server)
# set_target_properties (ntf_inflater PROPERTIES FOLDER "Microservices")

IF (NOT WIN32)
	target_link_libraries (${PROG_NAME} pthread)
ENDIF()

  
add_executable (

    ${PROG_NAME}
    main.cpp

    gfxlife.h
    gfxlife.cpp

    vkx00.h 
    vkx00.cpp 
    )


target_link_directories( 

  ${PROG_NAME}

  PUBLIC

    $ENV{SDL2_SDK_ROOT}/lib/x64
    $ENV{VULKAN_SDK}/Lib
)

target_include_directories (

  ${PROG_NAME}

  PUBLIC

    $ENV{GLM_ROOT}

    $ENV{SPUX_ROOT}
    $ENV{SPUX_SDL2_ROOT}

    $ENV{SDL2_SDK_ROOT}/include
    $ENV{VULKAN_SDK}/Include

    ${Boost_INCLUDE_DIRS}
  )

target_link_libraries (

  ${PROG_NAME}

    SDL2
    vulkan-1
    shaderc_combined
    VkLayer_utils
    spux
    spux_SDL2

    ${Boost_LIBRARIES}
  )