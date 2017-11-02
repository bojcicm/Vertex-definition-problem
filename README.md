# Problem with Vertex definition
---
## What I've done
* New vertex type `VertexPositionColorExtended` has been defined in [FILE](https://github.com/bojcicm/Vertex-definition-problem/blob/master/Engine/Third%20Party/DirectX%20Tool%20Kit/VertexTypes.h) Line 372.
* Input elements are defined in [FILE](https://github.com/bojcicm/Vertex-definition-problem/blob/master/Engine/Third%20Party/DirectX%20Tool%20Kit/VertexTypes.cpp) Line 175
* Added `Cube` definition that is using new vertex type [FILE](https://github.com/bojcicm/Vertex-definition-problem/blob/master/Engine/Models/Basic%20Shapes/Cubes.h) Line 20
  * Set up default values for `ValueX` and `ValueY` to be 1337, 1338
---
## How to reproduce
* In `Renderer.cpp` when we are updating `_model` we are reading list of vertices and expecting for `ValueX` to be 1337 or `ValueY` to be 1338
* Value present is **0** or something random from memory