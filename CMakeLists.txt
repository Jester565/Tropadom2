cmake_minimum_required ( VERSION 3.1 )
set (T_PROJECT_NAME Tropadom2)

project(${T_PROJECT_NAME})

set(CMAKE_BUILD_TYPE "Release" CACHE STRING "")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_GENERATOR_PLATFORM x64)

find_package(Protobuf REQUIRED)

set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost 1.66.0 COMPONENTS thread regex REQUIRED)

set(allegro false CACHE BOOL "Link to allegro")
if (allegro)
    set(allegincludedir "" CACHE STRING "The directory of allegro includes")
    set(alleglibdir "" CACHE STRING "The directories of allegro's libraries")
    if (UNIX)
        set(alleglib "liballegro.a" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "liballegro_ttf.a" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "liballegro_image.a" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "liballegro_primitives.a" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "liballegro_font.a" CACHE STRING "The path of the font allegro extension relative to allegdir")
    else()
        set(alleglib "allegro-static.lib" CACHE STRING "The path of the main allegro library relative to allegdir")
        set(ttflib "allegro_ttf-static.lib" CACHE STRING "The path of the ttf allegro extension relative to allegdir")
        set(imglib "allegro_image-static.lib" CACHE STRING "The path of the img allegro extension relative to allegdir")
        set(primlib "allegro_primitives-static.lib" CACHE STRING "The path of the primtive allegro extension relative to allegdir")
        set(fontlib "allegro_font-static.lib" CACHE STRING "The path of the font allegro extension relative to allegdir")
    endif()
endif()

set(boxincludedir "" CACHE STRING "Include directory of Box2D")
set(boxlibdir "" CACHE STRING "Directory of Box2D libraries")
if (UNIX)
    set(boxlib "Box2D.a" CACHE STRING "Name of Box2D library")
else()
    set(boxlib "Box2D.lib" CACHE STRING "Name of Box2D library")
endif()

set(allegextbuild true CACHE BOOL "Add AllegroExt as subproject")
set(allegextincludedir "${CMAKE_CURRENT_SOURCE_DIR}/AllegroExt/allegroext" CACHE STRING "Include directory of AllegroExt")
set(allegextlibdir "" CACHE STRING "Directory of AllegroExt")
set(allegextlib "AllegroExt" CACHE STRING "Name of AllegroExt library")

set(lightbuild true CACHE BOOL "Add Lighting4 as subproject")
set(lightincludedir "${CMAKE_CURRENT_SOURCE_DIR}/Lighting4/Lighting4" CACHE STRING "Include directory of Lighting4")
set(lightlibdir "" CACHE STRING "Directory of Lighting4 libraries")
set(lightlib "Lighting4" CACHE STRING "Name of Lighting4 library")

set(cppclientbuild true CACHE BOOL "Add CPP_Client2 as subproject")
set(cppclientincludedir "${CMAKE_CURRENT_SOURCE_DIR}/CPP_Client2/cppclient" CACHE STRING "Include directory of CPP_Client2")
set(cppclientlibdir "" CACHE STRING "Directory of CPP_Client2 libraries")
set(cppclientlib "CppClient2" CACHE STRING "Name of CPP_Client2 library")

set(HEADER_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tropadom2)
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)

set(HEADERS
	${HEADER_DIR}/AniJoint.h
    ${HEADER_DIR}/AniPart.h
    ${HEADER_DIR}/Block.h
    ${HEADER_DIR}/BlockColumn.h
    ${HEADER_DIR}/BlockCreator.h
    ${HEADER_DIR}/BoxCore.h
    ${HEADER_DIR}/CaveManager.h
    ${HEADER_DIR}/ChatBox.h
    ${HEADER_DIR}/CobbleStone.h
    ${HEADER_DIR}/DebugBox.h
    ${HEADER_DIR}/Dirt.h
    ${HEADER_DIR}/GameTest.h
    ${HEADER_DIR}/Grass.h
    ${HEADER_DIR}/InternetManager.h
    ${HEADER_DIR}/IronOre.h
    ${HEADER_DIR}/LightV4.h
    ${HEADER_DIR}/MainMenu.h
    ${HEADER_DIR}/Peer.h
    ${HEADER_DIR}/PeerManager.h
    ${HEADER_DIR}/PerlinManager.h
    ${HEADER_DIR}/PhysicsBody.h
    ${HEADER_DIR}/PhysicsBodyLight.h
    ${HEADER_DIR}/PhysicsCircle.h
    ${HEADER_DIR}/PhysicsImg.h
    ${HEADER_DIR}/PhysicsImgLight.h
    ${HEADER_DIR}/PhysicsJoint.h
    ${HEADER_DIR}/PhysicsSquare.h
    ${HEADER_DIR}/PhysicsSquareLight.h
    ${HEADER_DIR}/Pinger.h
    ${HEADER_DIR}/Player.h
    ${HEADER_DIR}/Profile.h
    ${HEADER_DIR}/ShadeBlock.h
    ${HEADER_DIR}/StoneBrick.h
    ${HEADER_DIR}/TerrainConstants.h
    ${HEADER_DIR}/TerrainLBC.h
    ${HEADER_DIR}/TerrainManager.h
    ${HEADER_DIR}/WorldContactListener.h
    ${HEADER_DIR}/WorldDebugDraw.h
    ${HEADER_DIR}/WorldManager.h)

set(SOURCES
	${SOURCE_DIR}/AniJoint.cpp
    ${SOURCE_DIR}/AniPart.cpp
    ${SOURCE_DIR}/Block.cpp
    ${SOURCE_DIR}/BlockColumn.cpp
    ${SOURCE_DIR}/BlockCreator.cpp
    ${SOURCE_DIR}/BoxCore.cpp
    ${SOURCE_DIR}/CaveManager.cpp
    ${SOURCE_DIR}/ChatBox.cpp
    ${SOURCE_DIR}/CobbleStone.cpp
    ${SOURCE_DIR}/DebugBox.cpp
    ${SOURCE_DIR}/Dirt.cpp
    ${SOURCE_DIR}/GameTest.cpp
    ${SOURCE_DIR}/Grass.cpp
    ${SOURCE_DIR}/InternetManager.cpp
    ${SOURCE_DIR}/IronOre.cpp
    ${SOURCE_DIR}/MainMenu.cpp
    ${SOURCE_DIR}/Peer.cpp
    ${SOURCE_DIR}/PeerManager.cpp
    ${SOURCE_DIR}/PerlinManager.cpp
    ${SOURCE_DIR}/PhysicsBody.cpp
    ${SOURCE_DIR}/PhysicsBodyLight.cpp
    ${SOURCE_DIR}/PhysicsCircle.cpp
    ${SOURCE_DIR}/PhysicsImg.cpp
    ${SOURCE_DIR}/PhysicsImgLight.cpp
    ${SOURCE_DIR}/PhysicsJoint.cpp
    ${SOURCE_DIR}/PhysicsSquare.cpp
    ${SOURCE_DIR}/PhysicsSquareLight.cpp
    ${SOURCE_DIR}/Pinger.cpp
    ${SOURCE_DIR}/Player.cpp
    ${SOURCE_DIR}/Profile.cpp
    ${SOURCE_DIR}/ShadeBlock.cpp
    ${SOURCE_DIR}/Source.cpp
    ${SOURCE_DIR}/StoneBrick.cpp
    ${SOURCE_DIR}/TerrainLBC.cpp
    ${SOURCE_DIR}/TerrainManager.cpp
    ${SOURCE_DIR}/WorldContactListener.cpp
    ${SOURCE_DIR}/WorldDebugDraw.cpp
    ${SOURCE_DIR}/WorldManager.cpp)

set (PROTOS 
    ${PROTO_DIR}/Trop.proto)

include_directories(
    ${HEADER_DIR})

if (cppclientbuild)
    add_subdirectory(CPP_Client2)
endif()
if (allegextbuild)
    add_subdirectory(AllegroExt)
endif()
if (lightbuild)
    add_subdirectory(Lighting4)
endif()

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${PROTOBUF_INCLUDE_DIRS}
    ${allegincludedir}
    ${allegextincludedir}
    ${lightincludedir}
    ${cppclientincludedir}
    ${boxincludedir})

#Include protobuf pb.cc and pb.h in binary
include_directories(${CMAKE_CURRENT_BINARY_DIR})

#Generate pb.cc and pb.h from proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTOS})

add_executable(${T_PROJECT_NAME} ${HEADERS} ${SOURCES} ${PROTO_HDRS} ${PROTO_SRCS})

target_link_libraries(${T_PROJECT_NAME}
    ${Boost_LIBRARIES}
    ${Protobuf_LIBRARIES}
    ${alleglibdir}${alleglib}
    ${alleglibdir}${ttflib}
    ${alleglibdir}${imglib}
    ${alleglibdir}${primlib}
    ${alleglibdir}${fontlib}
    ${allegextlibdir}${allegextlib}
    ${boxlibdir}${boxlib}
    ${lightlibdir}${lightlib}
    ${cppclientlibdir}${cppclientlib})

set_property(TARGET ${T_PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${T_PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED 11)

add_custom_command(TARGET ${T_PROJECT_NAME} PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_CURRENT_SOURCE_DIR}/wdir ${CMAKE_CURRENT_BINARY_DIR})