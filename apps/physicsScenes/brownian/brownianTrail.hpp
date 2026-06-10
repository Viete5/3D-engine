#ifndef BROWNIAN_TRAIL_HPP
#define BROWNIAN_TRAIL_HPP

#include "dataStructures/deque.hpp"

#include "../../../engine/include/math/vector.hpp"
#include "../../../engine/include/math/vector4.hpp"
#include "../../../engine/include/render/mesh.hpp"

#include <cstddef>

struct BrownianTrailSettings {
    float max_age = 10.0f;
    float min_point_distance = 0.035f;
    float width = 0.055f;
    std::size_t max_points = 256;
    engine::math::Vector4 color = engine::math::Vector4(1.0f, 0.72f, 0.18f, 0.9f);
};

struct TrailPoint {
    engine::math::Vector position;
    float time;
};

class BrownianTrail {
public:
    BrownianTrail();
    BrownianTrail(const BrownianTrailSettings& settings);

    void reset(const engine::math::Vector& position, float current_time);
    void update(
        const engine::math::Vector& position,
        const engine::math::Vector& camera_position,
        float current_time
    );

    engine::render::Mesh& get_mesh();
    const engine::render::Mesh& get_mesh() const;
    const BrownianTrailSettings& get_settings() const;
    std::size_t get_point_count() const;

private:
    BrownianTrailSettings settings;
    brownian::data_structures::Deque<TrailPoint> points;
    engine::render::Mesh mesh;

    void add_point_if_needed(const engine::math::Vector& position, float current_time);
    void remove_expired_points(float current_time);
    void rebuild_mesh(const engine::math::Vector& camera_position, float current_time);

    engine::math::Vector get_tangent(std::size_t point_index) const;
    engine::math::Vector get_side_vector(
        std::size_t point_index,
        const engine::math::Vector& camera_position
    ) const;
    engine::math::Vector4 get_point_color(float current_time, const TrailPoint& point) const;
    float get_point_width(float current_time, const TrailPoint& point) const;
};

#endif // BROWNIAN_TRAIL_HPP
