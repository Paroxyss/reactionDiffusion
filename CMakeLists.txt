cmake_minimum_required(VERSION 3.23)
project(gol)

set(CMAKE_BUILD_TYPE RelWithDebInfo)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast")

include_directories("src")

find_library(APPLE_FWK_FOUNDATION Foundation REQUIRED)
find_library(APPLE_FWK_QUARTZ_CORE QuartzCore REQUIRED)
find_library(APPLE_FWK_METAL Metal REQUIRED)

include_directories("/usr/local/Cellar/llvm/17.0.5/include/c++/v1")
link_directories("/usr/local/Cellar/llvm/17.0.5/lib/c++")

include_directories("/opt/homebrew/opt/llvm/include")
link_directories("/opt/homebrew/opt/llvm/lib")

include_directories("/opt/homebrew/opt/sdl2/include/")
link_directories("/opt/homebrew/opt/sdl2/lib/")


#file(GLOB SRC_FILES CONFIGURE_DEPENDS
#        "src/*/*.cpp"
#        "src/*.cpp"
#        )

set(SRC_FILES src/Display.cpp src/Simulation.cpp src/main.cpp)

file(GLOB SRC_SHADERS_FILES CONFIGURE_DEPENDS
        "src/*/*.metal"
        "src/*.metal"
        )

add_custom_command(
  OUTPUT gol.ir
  COMMAND xcrun -sdk macosx metal -o gol.ir -c src/gol.metal
  DEPENDS ${SRC_SHADERS_FILES}
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_custom_command(
  OUTPUT shaders.metallib
  COMMAND xcrun -sdk macosx metallib -o shaders.metallib gol.ir
  DEPENDS gol.ir
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)


add_executable(gol ${SRC_FILES} shaders.metallib)

target_include_directories(gol
  SYSTEM PUBLIC ${CMAKE_SOURCE_DIR}/metal-cpp
)

target_link_libraries(gol
  ${APPLE_FWK_FOUNDATION}
  ${APPLE_FWK_QUARTZ_CORE}
  ${APPLE_FWK_METAL}
  SDL2
)
