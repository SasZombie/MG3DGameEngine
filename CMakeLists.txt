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
    src/Particle.cpp
)

# Grab all from external
file(GLOB_RECURSE BACKENDS_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/backends/*.cpp"
)

file(GLOB_RECURSE IAMGUI_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/iamgui/*.cpp"
)

set(SOURCES_NODES
    ${ENGINE_SOURCES}
    ${BACKENDS_SOURCES}
    ${IAMGUI_SOURCES}
    src/main.cpp
)

set(SOURCES_NODES2
    ${ENGINE_SOURCES}
    ${BACKENDS_SOURCES}
    ${IAMGUI_SOURCES}
    src/mainLoad.cpp
)

add_executable(main ${SOURCES_NODES})
add_executable(main2 ${SOURCES_NODES2})

if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /W4 /Zi /Od /EHsc")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /O2 /EHsc")

    set_target_properties(main PROPERTIES ENABLE_EXPORTS ON)
    target_link_libraries(main PRIVATE opengl32.lib glew32s.lib glfw3.lib)

    set_target_properties(main2 PROPERTIES ENABLE_EXPORTS ON)
    target_link_libraries(main2 PRIVATE opengl32.lib glew32s.lib glfw3.lib)

else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Winline -Wundef -Wcast-qual -Wshadow -Wwrite-strings -Wno-unused-parameter -Wfloat-equal -pedantic -fsanitize=undefined -fsanitize=address -fsanitize=leak")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")

    set_target_properties(main PROPERTIES ENABLE_EXPORTS ON)
    target_link_libraries(main PRIVATE glut GL GLU X11 Xxf86vm Xrandr pthread Xi GLEW glfw)

    set_target_properties(main2 PROPERTIES ENABLE_EXPORTS ON)
    target_link_libraries(main2 PRIVATE glut GL GLU X11 Xxf86vm Xrandr pthread Xi GLEW glfw)

endif()

if(CMAKE_BUILD_TYPE STREQUAL "debug")
    target_compile_definitions(main PRIVATE debugMode=1)
    target_compile_definitions(main2 PRIVATE debugMode=1)
else()
    target_compile_definitions(main PRIVATE debugMode=0)
    target_compile_definitions(main2 PRIVATE debugMode=0)
endif()
