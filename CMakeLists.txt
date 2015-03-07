project (animation_twist)

# set(CMAKE_CXX_COMPILER "clang")

cmake_minimum_required (VERSION 2.8.4)
set(CMAKE_LEGACY_CYGWIN_WIN32 1)
# set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/../cmake/")

set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_BUILD_TYPE Release)

set(LIBS "")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR})

add_definitions(-Wall -Werror)

###########################################################
# DEPENDENCIES

# looking for boost
find_package(Boost REQUIRED COMPONENTS filesystem system unit_test_framework)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(LINK_FLAGS ${LINK_FLAGS} /LIBPATH:"${Boost_LIBRARY_DIRS}")
  #set(LIBS ${LIBS} /LIBPATH:${Boost_LIBRARY_DIRS})
endif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

# looking for GTEngine library
find_path(GTE_INCLUDE_DIR GTEngine.h ${LIBS_PATH}/GTEngine/include /usr/include/GTEngine /usr/local/include/GTEngine /mingw/include/GTEngine)
find_library(GTE_LIBRARY NAMES gtengine PATHS /usr/lib /usr/local/lib)
if (GTE_INCLUDE_DIR AND GTE_LIBRARY)
  set (GTE_FOUND TRUE)
  message (STATUS "GTE library found (${GTE_INCLUDE_DIR}).")
else (GTE_INCLUDE_DIR AND GTE_LIBRARY)
  message (FATAL_ERROR "GTE library not found (${GTE_INCLUDE_DIR}).")
endif (GTE_INCLUDE_DIR AND GTE_LIBRARY)
include_directories(${GTE_INCLUDE_DIR})
set(LIBS ${LIBS} ${GTE_LIBRARY})

# looking for OpenGL libraries
find_package(OpenGL)
if (OPENGL_FOUND)
  message (STATUS "OpenGL libraries found (${OPENGL_INCLUDE_DIR} and ${OPENGL_LIBRARY}).")
else (OPENGL_FOUND)
  message(FATAL_ERROR "OpenGL libraries not found!")
endif (OPENGL_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARY})

find_package(GLUT REQUIRED)
include_directories(${GLUT_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLUT_LIBRARY})

# looking for GLEW library
find_path(GLEW_INCLUDE_DIR GL/glew.h ${LIBS_PATH}/glew/include /usr/include /usr/local/include /mingw/include C:/MinGW/include)
find_library(GLEW_LIBRARY NAMES GLEW GLEW32 glew32 PATHS ${LIBS_PATH}/glew/lib /usr/lib /usr/local/lib)
if (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
  set (GLEW_FOUND TRUE)
  message (STATUS "GLEW library found (${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARY}).")
else (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
  message (FATAL_ERROR "GLEW library not found (${GLEW_INCLUDE_DIR} and ${GLEW_LIBRARY}).")
endif (GLEW_INCLUDE_DIR AND GLEW_LIBRARY)
include_directories(${GLEW_INCLUDE_DIR})
set(LIBS ${LIBS} ${GLEW_LIBRARY})

# Qt fun
SET(QT_USE_QTOPENGL TRUE)
FIND_PACKAGE(Qt4 REQUIRED)
include(${QT_USE_FILE})
include_directories(${QT_INCLUDE_DIR})
set(LIBS ${LIBS} ${QT_LIBRARIES})
message(STATUS ${QT_LIBRARIES})

# Qt Framework
# FIND_PACKAGE(QF REQUIRED)
# include_directories(${QF_INCLUDE_DIR})
# set(LIBS ${LIBS} ${QF_LIBRARY})


###########################################################
# BUILD

add_definitions(-std=c++11)

include_directories(include)
add_subdirectory (src bin/demo)
