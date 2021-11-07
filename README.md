# Vertex Welding 
Vertex welding can be used to find duplicate vertices and fix cracks in a mesh. The idea is to replace duplicate vertices by a representant vertex and update the index buffer accordingly. 

A simple method to implement this is to test each vertex against every other vertex. This results in a runtime complexity of O(n²).

## Spatial Hashing
To improve the expected runtime complexity to O(n), spatial hashing can be used. The idea is to utilize an uniform grid and map each vertex into a grid cell. A vertex is then only tested to the vertices stored in neighboring cells.

Note that the worst case runtime complexity is still O(n²) when the hash function maps each vertex into the same cell.

## Example
In this example a triangle fan with n segments is created using single triangles which results in 3*(n+1) vertices. However the center vertex and the neighboring vertices of the triangle fan can be shared among all tringles. The welding therefore reduces the number of vertices to n+1.

## References
M. Teschner, B. Heidelberger, M. Mueller, D. Pomeranets, M. Gross: Optimized Spatial Hashing for Collision Detection of Deformable Objects, VMV, 2003.

Christer Ericson: Real-Time Collision Detection, Morgan Kaufmann, 2005. 