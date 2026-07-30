# CMakeList.txt : CMake project for Raytracing, include source and define
# project specific logic here.
#
find_package(SDL3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_executable (Raytracing src/main.cpp)
# Add source to this project's executable.
file(GLOB_RECURSE Raytracing_SOURCES CONFIGURE_DEPENDS
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"

)
target_sources(Raytracing PUBLIC ${Raytracing_SOURCES})

target_include_directories(Raytracing PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/src
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Raytracing PROPERTY CXX_STANDARD 20)
endif()




target_link_libraries(Raytracing PRIVATE SDL3::SDL3-shared fmt::fmt)

# TODO: Add tests and install targets if needed.
