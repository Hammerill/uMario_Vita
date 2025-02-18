# This file used to faster compile and update game, if you already have asset files (you have to manually copy them to ux0:app/HMRL00002)

cmake_minimum_required(VERSION 2.82)

file(GLOB_RECURSE uMario_SOURCES "../uNext/*.cpp")
add_executable(${CMAKE_PROJECT_NAME} ${uMario_SOURCES})

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(uMario)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APPNAME  "Mario")
set(VITA_TITLEID  "HMRL00002")
set(VITA_VERSION  "01.00")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -pthread -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
	"$ENV{VITASDK}/arm-vita-eabi/include/"
	"$ENV{VITASDK}/arm-vita-eabi/include/SDL2"
)

include_directories(uNext)

target_link_libraries(${CMAKE_PROJECT_NAME}
	SDL2_image
	SDL2_mixer
	SDL2

	vorbisfile
	vorbis
	ogg
	mikmod
	modplug
	mpg123
	FLAC

	SceAudio_stub
	SceAudioIn_stub
	SceCommonDialog_stub
	SceCtrl_stub
	SceDisplay_stub
	SceGxm_stub
	SceHid_stub
	SceSysmodule_stub
	SceTouch_stub
	SceMotion_stub
)

vita_create_self(${CMAKE_PROJECT_NAME}.self ${CMAKE_PROJECT_NAME})

vita_create_vpk(${CMAKE_PROJECT_NAME}.vpk ${VITA_TITLEID} ${CMAKE_PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APPNAME}

#   FILE uNext/files files 

	FILE ../sce_sys/icon0.png sce_sys/icon0.png
	FILE ../sce_sys/pic0.png sce_sys/pic0.png
	FILE ../sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
	FILE ../sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
	FILE ../sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
