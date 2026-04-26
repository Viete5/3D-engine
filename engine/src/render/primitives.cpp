#include "../include/render/primitives.hpp"

#include "../include/math/vector.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>

namespace engine::render {

namespace {

constexpr float pi = 3.14159265358979323846f;

Vertex make_vertex(
    float x,
    float y,
    float z,
    float tex_u,
    float tex_v,
    float normal_x,
    float normal_y,
    float normal_z
) {
    return Vertex{
        engine::math::Vector(x, y, z),
        engine::math::Vector(normal_x, normal_y, normal_z),
        tex_u,
        tex_v
    };
}

engine::math::Vector calculate_torus_point(
    float major_radius,
    float minor_radius,
    float angle_xy,
    float angle_z
) {
    const float x = (major_radius + minor_radius * std::cos(angle_z)) * std::cos(angle_xy);
    const float y = (major_radius + minor_radius * std::cos(angle_z)) * std::sin(angle_xy);
    const float z = minor_radius * std::sin(angle_z);
    return engine::math::Vector(x, y, z);
}

engine::math::Vector calculate_torus_normal(float angle_xy, float angle_z) {
    const float x = std::cos(angle_z) * std::cos(angle_xy);
    const float y = std::cos(angle_z) * std::sin(angle_xy);
    const float z = std::sin(angle_z);
    return engine::math::Vector(x, y, z);
}

} // namespace

Mesh create_cube_mesh() {
    std::vector<Vertex> vertices = {
        // Front
        make_vertex(-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f),
        make_vertex( 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f),
        make_vertex( 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f),
        make_vertex(-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f),

        // Back
        make_vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f),
        make_vertex( 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f),
        make_vertex( 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f),
        make_vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f),

        // Up
        make_vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f),
        make_vertex( 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f),
        make_vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f),
        make_vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f),

        // Down
        make_vertex( 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f),
        make_vertex( 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f),
        make_vertex(-0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f),
        make_vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f),

        // Right
        make_vertex( 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f),
        make_vertex( 0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f),
        make_vertex( 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f),
        make_vertex( 0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f),

        // Left
        make_vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f),
        make_vertex(-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f),
        make_vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f),
        make_vertex(-0.5f,  0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f)
    };

    std::vector<unsigned int> indices = {
        0, 1, 2, 2, 3, 0,
        4, 7, 6, 6, 5, 4,
        8, 9, 10, 9, 11, 10,
        14, 13, 12, 14, 15, 13,
        16, 17, 18, 19, 18, 17,
        20, 22, 23, 23, 21, 20
    };

    return Mesh(vertices, indices);
}

Mesh create_torus_mesh(float major_radius, float minor_radius, int stacks, int slices) {
    if (major_radius <= 0.0f || minor_radius <= 0.0f) {
        throw std::runtime_error("Torus radius must be positive");
    }

    if (stacks <= 0 || slices <= 0) {
        throw std::runtime_error("Torus stacks and slices must be positive");
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    vertices.reserve(static_cast<std::size_t>(stacks + 1) * static_cast<std::size_t>(slices + 1));
    indices.reserve(static_cast<std::size_t>(stacks) * static_cast<std::size_t>(slices) * 6);

    const float delta_u = 2.0f * pi / static_cast<float>(slices);
    const float delta_v = 2.0f * pi / static_cast<float>(stacks);

    for (int i = 0; i <= stacks; ++i) {
        for (int j = 0; j <= slices; ++j) {
            const float angle_xy = static_cast<float>(j) * delta_u;
            const float angle_z = static_cast<float>(i) * delta_v;

            const engine::math::Vector position =
                calculate_torus_point(major_radius, minor_radius, angle_xy, angle_z);
            const engine::math::Vector normal = calculate_torus_normal(angle_xy, angle_z);

            vertices.push_back(Vertex{
                position,
                normal,
                static_cast<float>(j) / static_cast<float>(slices),
                static_cast<float>(i) / static_cast<float>(stacks)
            });
        }
    }

    for (int i = 0; i < stacks; ++i) {
        for (int j = 0; j < slices; ++j) {
            const unsigned int base_index = static_cast<unsigned int>(i * (slices + 1) + j);
            const unsigned int point_a = base_index;
            const unsigned int point_b = base_index + 1;
            const unsigned int point_c = base_index + static_cast<unsigned int>(slices + 1);
            const unsigned int point_d = point_c + 1;

            indices.push_back(point_a);
            indices.push_back(point_b);
            indices.push_back(point_c);

            indices.push_back(point_d);
            indices.push_back(point_c);
            indices.push_back(point_b);
        }
    }

    return Mesh(vertices, indices);
}

} // namespace engine::render
