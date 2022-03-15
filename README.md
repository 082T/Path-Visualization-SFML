# Path-Visualization-SFML

## A*, greedyBFS, Dijkstra path visualization written in C++ using SFML 2.5.1

Buttons:
- A* - executes the Astar algorithm
- greedyBFS - executes the greedy best-first search algorithm
- Dijkstra - executes Dijkstra's algorithm
- Soft Clear - cleans tiles changed by used algorithm
- Clear - cleans all tiles in the grid
- Start - enables choosing the starting block by clicking on the grid
- Obstacle - enables placing obstacle blocks by left-clicking on the grid and removing blocks by right-clicking on the grid
- End - enables choosing the ending block by clicking on the grid

https://user-images.githubusercontent.com/43017115/158460820-cf7cac85-9f48-4b69-abce-ab41543a1c02.mp4

![image](https://user-images.githubusercontent.com/43017115/158481783-db52c3b2-b492-4e9f-b678-124049623158.png)


To compile and run (visual studio 2019):
- Download SFML library
- Copy .dll files in project folder
- Change project mode to Release x64
- Add SFML\include to C/C++ -> General -> Additional Include Directories
- Add SFML\lib to Linker -> General -> Additional Library Directories

