cmake_minimum_required(VERSION 3.12)
project(GameEngine)


set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include/GameEngine)

#Full controll over what I write
set(ENGINE_SOURCES
    src/camera.cpp
    src/mesh.cpp
    src/meshLoaderObj.cpp
    src/texture.cpp
    src/UI.cpp
    src/shader.cpp
    src/window.cpp
    src/SceneNode.cpp
    src/Asset.cpp
    src/OctreeNode.cpp
    src/Transform.cpp
    src/GameEngine.cpp
    src/AssetManager.cpp
    src/ScriptTransform.cpp
    src/Globals.cpp
    src/ScriptInstance.cpp
)

# Grab all from external
file(GLOB_RECURSE BACKENDS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/backends/*.cpp"
)

file(GLOB_RECURSE IAMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/iamgui/*.cpp"
)

set(SOURCESNODES
    ${ENGINE_SOURCES}
    ${BACKENDS_SOURCES}
    ${IAMGUI_SOURCES}
    src/main.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -fsanitize=undefined -fsanitize=address -fsanitize=leak")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(main ${SOURCESNODES})

set_target_properties(main PROPERTIES ENABLE_EXPORTS ON)
target_link_libraries(main PRIVATE glut GL GLU X11 Xxf86vm Xrandr pthread Xi GLEW glfw)

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    target_compile_definitions(main PRIVATE debugMode=1)
else()
    target_compile_definitions(main PRIVATE debugMode=0)
endif()
