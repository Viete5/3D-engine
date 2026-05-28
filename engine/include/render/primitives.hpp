#ifndef PRIMITIVES_HPP
#define PRIMITIVES_HPP

#include "mesh.hpp"

namespace engine::render {

Mesh create_cube_mesh();
Mesh create_sphere_mesh(float radius, int stacks, int slices);
Mesh create_torus_mesh(float major_radius, float minor_radius, int stacks, int slices);

} // namespace engine::render

#endif // PRIMITIVES_HPP
