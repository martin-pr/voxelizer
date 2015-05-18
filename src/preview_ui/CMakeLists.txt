include_directories(./)
include_directories(../lib)
include_directories(../common)

file(GLOB headers_preview *.h ../common/viewport.h)
file(GLOB sources_preview *.cpp ../common/viewport.cpp)

set(headers ${headers_preview})
set(sources ${sources_preview})

QT4_WRAP_CPP(headers_moc ${headers})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(preview ${sources} ${headers_moc})

target_link_libraries(preview ${LIBS} voxelizer)
