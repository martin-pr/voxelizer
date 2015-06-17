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
find_package(Boost REQUIRED COMPONENTS filesystem system serialization unit_test_framework program_options)
include_directories(${Boost_INCLUDE_DIR})
set(LIBS ${LIBS} ${Boost_LIBRARIES})

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  set(LINK_FLAGS ${LINK_FLAGS} /LIBPATH:"${Boost_LIBRARY_DIRS}")
  #set(LIBS ${LIBS} /LIBPATH:${Boost_LIBRARY_DIRS})
endif("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")

# looking for OpenGL libraries
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENGL_LIBRARY})

# looking for glut
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

# looking for Alembic library - REQUIRES A C++11 BUILD OF ALEMBIC!
find_path(ALEMBIC_INCLUDE_DIR Alembic/Abc/All.h ${LIBS_PATH}/Alembic/include /usr/include /usr/local/include /usr/local/alembic-1.5.8/include)
find_path(ALEMBIC_LIBRARY_PATH libAlembicAbc.a ${LIBS_PATH}/Alembic/lib /usr/lib /usr/local/lib /usr/local/alembic-1.5.8/lib /usr/local/alembic-1.5.8/lib/static)
# the list of required libraries
set(ALEMBIC_DEPS AlembicAbcCoreHDF5 AlembicAbcCoreAbstract AlembicUtil AlembicAbc AlembicAbcGeom AlembicAbcCollection)

# find them all
set(ALEMBIC_FOUND TRUE)
set(ALEMBIC_LIBRARIES "")
foreach(lib ${ALEMBIC_DEPS})
  find_library(DEP${lib} NAMES ${lib} PATHS ${ALEMBIC_LIBRARY_PATH})
  if(NOT DEP${lib})
    set(ALEMBIC_FOUND FALSE)
    message (FATAL_ERROR "Alembic library not found (${ALEMBIC_INCLUDE_DIR} and ${ALEMBIC_LIBRARIES}).")
  else(NOT DEP${lib})
    set(ALEMBIC_LIBRARIES ${ALEMBIC_LIBRARIES} ${DEP${lib}})
  endif(NOT DEP${lib})
endforeach(lib)
# if found
if(ALEMBIC_INCLUDE_DIR AND ALEMBIC_FOUND)
  message (STATUS "Alembic library found (${ALEMBIC_INCLUDE_DIR} and ${ALEMBIC_LIBRARIES}).")
endif(ALEMBIC_INCLUDE_DIR AND ALEMBIC_FOUND)
# and include the result
include_directories(${ALEMBIC_INCLUDE_DIR})
set(LIBS ${LIBS} ${ALEMBIC_LIBRARIES})

# looking for hdf5 libs
SET(HDF5_LIBRARY_PATHS /usr/lib /usr/local/lib /sw/lib /opt/local/lib $ENV{PROGRAM_FILES}/HDF5/lib/static)
FIND_PATH(HDF5_INCLUDE_DIR H5Cpp.h PATH_SUFFIXES HDF5 /usr/include /usr/local/include /sw/include /opt/local/include)
FIND_LIBRARY(HDF5_LIBRARY NAMES hdf5 PATHS ${HDF5_LIBRARY_PATHS})
FIND_LIBRARY(HDF5HL_LIBRARY NAMES hdf5_hl PATHS ${HDF5_LIBRARY_PATHS})
if (HDF5_INCLUDE_DIR AND HDF5_LIBRARY AND HDF5HL_LIBRARY)
  set (HDF5_FOUND TRUE)
  message (STATUS "HDF5 library found (${HDF5_INCLUDE_DIR} and ${HDF5_LIBRARY}, ${HDF5HL_LIBRARY}).")
else (HDF5_INCLUDE_DIR AND HDF5_LIBRARY AND HDF5HL_LIBRARY)
  message (FATAL_ERROR "HDF5 library not found (${HDF5_INCLUDE_DIR} and ${HDF5_LIBRARY}, ${HDF5HL_LIBRARY}).")
endif (HDF5_INCLUDE_DIR AND HDF5_LIBRARY AND HDF5HL_LIBRARY)
include_directories(${HDF5_INCLUDE_DIR})
set(LIBS ${LIBS} ${HDF5HL_LIBRARY} ${HDF5_LIBRARY})


# looking for openexr libs
SET(OPENEXR_LIBRARY_PATHS /usr/lib /usr/local/lib /sw/lib /opt/local/lib $ENV{PROGRAM_FILES}/OpenEXR/lib/static)
FIND_PATH(OPENEXR_INCLUDE_DIR ImfRgbaFile.h PATH_SUFFIXES OpenEXR /usr/include /usr/local/include /sw/include /opt/local/include)
FIND_LIBRARY(OPENEXR_HALF_LIBRARY NAMES Half PATHS ${OPENEXR_LIBRARY_PATHS})
FIND_LIBRARY(OPENEXR_IEX_LIBRARY NAMES Iex PATHS ${OPENEXR_LIBRARY_PATHS})
if (OPENEXR_INCLUDE_DIR AND OPENEXR_HALF_LIBRARY AND OPENEXR_IEX_LIBRARY)
  set (HDF5_FOUND TRUE)
  message (STATUS "OpenEXR library found (${OPENEXR_INCLUDE_DIR}, ${OPENEXR_IEX_LIBRARY} and ${OPENEXR_HALF_LIBRARY}).")
else (OPENEXR_INCLUDE_DIR AND OPENEXR_HALF_LIBRARY AND OPENEXR_IEX_LIBRARY)
  message (FATAL_ERROR "OpenEXR library not found (${OPENEXR_INCLUDE_DIR}, ${OPENEXR_IEX_LIBRARY} and ${OPENEXR_HALF_LIBRARY}).")
endif (OPENEXR_INCLUDE_DIR AND OPENEXR_HALF_LIBRARY AND OPENEXR_IEX_LIBRARY)
include_directories(${OPENEXR_INCLUDE_DIR})
set(LIBS ${LIBS} ${OPENEXR_HALF_LIBRARY} ${OPENEXR_IEX_LIBRARY})

# blindly adding libssl, should be available on all linux distros
set(LIBS ${LIBS} crypto)

# blindly adding tbb
set(LIBS ${LIBS} tbb)

###########################################################
# BUILD

add_definitions(-std=c++11)

include_directories(include)
add_subdirectory (src bin/demo)
