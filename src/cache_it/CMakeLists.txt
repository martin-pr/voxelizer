include_directories(./)
include_directories(../lib)

file(GLOB sources *.cpp ../common/config.cpp ../common/common.cpp)

add_executable(cache_it ${sources})

target_link_libraries(cache_it ${LIBS} voxelizer)
