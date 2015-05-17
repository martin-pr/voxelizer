include_directories(./)
include_directories(../lib)
include_directories(../ui)

file(GLOB headers_preview *.h)
file(GLOB sources_preview *.cpp)
file(GLOB headers_ui ../ui/*.h)
file(GLOB sources_ui ../ui/*.cpp)

set(headers ${headers_preview} ${headers_ui})
set(sources ${sources_preview} ${sources_ui})

QT4_WRAP_CPP(headers_moc ${headers})

include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})

add_executable(preview ${sources} ${headers_moc})

target_link_libraries(preview ${LIBS} voxelizer)
