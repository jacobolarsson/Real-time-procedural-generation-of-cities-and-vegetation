# Real-time procedural generation of cities and vegetation
## Overview
Program created using C++ and OpenGL, featuring real-time procedural generation of 3D environments with buildings and trees using L-systems. As users freely navigate the 3D environment, the landscape geometry is dynamically generated in real-time as needed by the user.

## Key features 
- Real-time 3D procedural generation: Scene geometry is procedurally generated using multiple techniques such as a L-system.
- Frustum filling and LRU cache: By using frustum filling, each object's mesh is created only once when needed and then cached in a Least Recently Used (LRU) cache, ensuring high performace and a constant, minimal memory footprint. ​
- Unique and consistent: The parameters to create an object's mesh are the object's position and a unique global random seed, meaning that the created landscape is guaranteed to always be consistent and unique for every program execution.

## Results

https://github.com/user-attachments/assets/96b619e8-207a-4f3b-b9f3-a803c08bab75

## Documentation

[Download project post-mortem](https://github.com/user-attachments/files/18278963/Real-time.procedural.generation.of.cities.and.vegetation.pdf)

[Download project presentation](https://github.com/user-attachments/files/18278965/Real-time.procedural.generation.of.cities.and.vegetation.presentation.pptx)


## Future work
- Texture and skybox support.
- GUI to tweak procedural generation parameters in real time.
- Add other city elements like roads, street lamps and cars.
- Feature to save and load global random seeds.
- Further optimization of the use of the CGAL library.
