cmake_minimum_required ( VERSION 2.8)
project ( host_sdl )

set (HOST_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/..")

set (HOST_SRC
	${HOST_ROOT}/source/SDLHost.cpp
	${HOST_ROOT}/source/SDLHostMain.cpp
	${HOST_ROOT}/source/MOAIVideoModesMgr.cpp
)

set (HOST_INCLUDES
	"${MOAI_ROOT}/src"
	"${MOAI_ROOT}/src/moai-config"
	"${MOAI_ROOT}/3rdparty/sdl2-2.0.0/include"
	"${HOST_ROOT}/"
)

include_directories(${HOST_INCLUDES})
add_executable(moai ${HOST_SRC})
target_link_libraries ( moai 
  host-modules
  moai-core
  moai-util
  moai-sim
  SDL2-static
  zlcore
)

if ( BUILD_WINDOWS )
  target_link_libraries ( moai
    winmm
    Shlwapi
  )
endif (BUILD_WINDOWS)