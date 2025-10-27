# 3D Game Engine

## 1. Introduction

This is a very minimalist game engine developed using OpenGL, Dear ImGui and C++.
The purpose of this project was to create a semi-functional engine that can render, have movement capabilities and so on. The build produces 2 executables:
1. Where a scene can be defined
1. Where a scene is loaded into memory

## 2. Building

Though I tried making this library cross-platform, I am not sure at all if it will build on a Windows machine. I tried my best but without proper testing it might fail. To build there are 2 simple choices: cmake or cmake presets.
<br> Option 1:

```
mkdir build
cd build

cmake -DCMAKE_BUILD_TYPE=debug # or release

cmake --build . --config Debug   # for Windows
cmake --build . --config Release # for Windows

cmake --build . # Linux / MacOS
```

<br> Option 2:
```
cmake --preset debug # or release
cmake --build --preset debug # or release

```

## 3. Features

The project fulfills the document specifications:

- 3D api, here OpenGL ✅
- [Internal Format for Meshes, including Textures and normals](include/GameEngine/mesh.hpp) ✅
- [3D object importer](include/GameEngine/meshLoaderObj.hpp) ✅
- [3D math module](include/GameEngine/math.hpp) ✅
- [IO user Interactions](include/GameEngine/main.hpp) ✅
- [FPS Camera Implementation](include/GameEngine/camera.hpp) ✅
- [Illumination](include/GameEngine/shader.hpp) ✅
- [Scene Management](include/GameEngine/SceneNode.hpp) ✅
- [Culling using Octrees](include/GameEngine/OctreeNode.hpp) ✅
- [Custom Format for storage](include/GameEngine/Codes.hpp) ✅
- [2D Gui, object names](include/GameEngine/UI.hpp) ✅
- [Scripting using custom language](include/GameEngine/ScriptInstance.hpp) ✅
- [Collision Detection, Octrees with AABB](include/GameEngine/CollisionObjects.hpp) ✅
- [Particle system](include/GameEngine/Particle.hpp) ✅

## 4. Notes

While the system has the requested features, there are also a some problems:
- Not  is encapsulated inside the GameEngine class.
- Callbacks are limited; I cannot store callable objects in C++ and rely on script paths.
- Asset class should have multiple specializations: Illumination Asset, Moving Asset, SkyBox Asset.
- UI is drawn incorrectly and should be refactored into another Drawable/Asset class.
- The translator requires more work to have a proper implementation

## 4. Credits
[1]: Dr. Iulia-Cristina Stănică, Lecturer, Department of Engineering in Foreign Languages, from National University of Science and Technology POLITEHNICA Bucharest  
[2]: https://www.baeldung.com/cs/quadtrees-and-octrees, https://www.youtube.com/watch?v=9sDlP6glQ0c  
[3]: https://www.youtube.com/watch?v=dn0hUgsok9M