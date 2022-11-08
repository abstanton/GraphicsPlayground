# Graphics Playground
This is a WIP personal project started with the goal to learn real-time graphics programming
and improve my C++ along the way.

![image](https://user-images.githubusercontent.com/23246639/191087627-c93a308f-b2bc-43a4-90d0-21a79213bfed.png)
Demo image

![image](https://user-images.githubusercontent.com/23246639/200562431-6bd16bef-a476-41ba-8846-e9d88edbff31.png)
Space Invaders demo

## Current Progress
- GPU abstraction
- Texture, Shader, Mesh loading
- Entity Component System 
- Materal system for abstracting shader inputs
- Forward renderer with PBR shading
- Shadow maps for directional lights
- Point lights
- Texture tiling

## Skills Learnt
- OpenGL programming
- PBR Theory
- Modern C++ features

## TODO
- [ ] Support shadow mapping for point lights
- [ ] Improve input support
- [ ] Support transparent materials
- [ ] Image Based Lighting
- [ ] AA (FXAA, MSAA... tbd)]
- [ ] Bloom
- [ ] Motion Blur
- [x] SSAO
- [ ] SSR
- [ ] Contact Shadows
- [x] Improve building experience
- [ ] SceneGraph for collision, culling etc
- [ ] SceneHierarchy for proper scene organisation
- [ ] Assimp texture loading
- [ ] Make prettier demo
- [ ] Audio handling

## Building
As one of the Todos says, this will not build on other systems because I have included paths on my machine in the CMakeLists.txt. Hopefully I'll fix that
