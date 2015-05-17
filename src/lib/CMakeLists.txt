include_directories(./)

file(GLOB sources *.cpp)

add_library(voxelizer SHARED ${sources})

target_link_libraries(voxelizer ${LIBS})
