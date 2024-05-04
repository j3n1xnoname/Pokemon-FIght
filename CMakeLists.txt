cmake_minimum_required(VERSION 3.5.0)
project(CMakeStarterCpp VERSION 0.1.0 LANGUAGES C CXX)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
PKG_SEARCH_MODULE(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)
PKG_SEARCH_MODULE(SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)


set(SOURCE_FILES 
src/main.cpp src/LTexture.hpp src/LTexture.cpp src/LTimer.hpp src/LTimer.cpp src/Pokemon.hpp src/Pokemon.cpp 
src/Attack.hpp src/Attack.cpp src/Player.hpp src/Player.cpp)

add_executable(Pokemon ${SOURCE_FILES})

set_target_properties(
            Pokemon PROPERTIES
            CXX_STANDARD 17
            CXX_STANDARD_REQUIRED ON
)


target_include_directories(Pokemon PRIVATE
    ${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})

target_link_libraries(Pokemon PRIVATE
    ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2MIXER_LIBRARIES})
