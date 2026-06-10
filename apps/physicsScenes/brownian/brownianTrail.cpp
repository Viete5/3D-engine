#include "brownianTrail.hpp"

#include "dataStructures/dynamicArray.hpp"

#include <algorithm>

namespace {

constexpr float min_vector_length = 1e-5f;

float clamp01(float value) {
    return std::clamp(value, 0.0f, 1.0f);
}

engine::math::Vector safe_normalize(
    const engine::math::Vector& vector,
    const engine::math::Vector& fallback
) {
    if (vector.dist() <= min_vector_length) {
        return fallback;
    }

    return vector.normalize();
}

} // namespace

BrownianTrail::BrownianTrail()
    : BrownianTrail(BrownianTrailSettings{}) {
}

BrownianTrail::BrownianTrail(const BrownianTrailSettings& settings)
    : settings(settings),
      mesh(
          brownian::data_structures::DynamicArray<engine::render::Vertex>().to_std_vector(),
          brownian::data_structures::DynamicArray<unsigned int>().to_std_vector()
      ) {
}

void BrownianTrail::reset(const engine::math::Vector& position, float current_time) {
    points.clear();
    points.push_back(TrailPoint{position, current_time});
    mesh.update_data({}, {});
}

void BrownianTrail::update(
    const engine::math::Vector& position,
    const engine::math::Vector& camera_position,
    float current_time
) {
    remove_expired_points(current_time);
    add_point_if_needed(position, current_time);
    rebuild_mesh(camera_position, current_time);
}

engine::render::Mesh& BrownianTrail::get_mesh() {
    return mesh;
}

const engine::render::Mesh& BrownianTrail::get_mesh() const {
    return mesh;
}

const BrownianTrailSettings& BrownianTrail::get_settings() const {
    return settings;
}

std::size_t BrownianTrail::get_point_count() const {
    return points.get_size();
}

void BrownianTrail::add_point_if_needed(const engine::math::Vector& position, float current_time) {
    if (points.empty()) {
        points.push_back(TrailPoint{position, current_time});
        return;
    }

    if (points.back().position.dist_between_dots(position) < settings.min_point_distance) {
        return;
    }

    points.push_back(TrailPoint{position, current_time});

    while (points.get_size() > settings.max_points) {
        points.pop_front();
    }
}

void BrownianTrail::remove_expired_points(float current_time) {
    while (
        points.get_size() > 1 &&
        current_time - points.front().time > settings.max_age
    ) {
        points.pop_front();
    }
}

void BrownianTrail::rebuild_mesh(const engine::math::Vector& camera_position, float current_time) {
    if (points.get_size() < 2) {
        mesh.update_data({}, {});
        return;
    }

    brownian::data_structures::DynamicArray<engine::render::Vertex> vertices;
    brownian::data_structures::DynamicArray<unsigned int> indices;

    vertices.reserve(points.get_size() * 2);
    indices.reserve((points.get_size() - 1) * 6);

    for (std::size_t point_index = 0; point_index < points.get_size(); ++point_index) {
        const TrailPoint& point = points[point_index];
        const engine::math::Vector side = get_side_vector(point_index, camera_position);
        const engine::math::Vector view_normal =
            safe_normalize(camera_position - point.position, engine::math::Vector(0.0f, 0.0f, 1.0f));
        const float half_width = get_point_width(current_time, point) * 0.5f;
        const engine::math::Vector offset = side * half_width;
        const engine::math::Vector4 color = get_point_color(current_time, point);

        vertices.push_back(engine::render::Vertex{
            point.position - offset,
            view_normal,
            0.0f,
            0.0f,
            color
        });
        vertices.push_back(engine::render::Vertex{
            point.position + offset,
            view_normal,
            1.0f,
            0.0f,
            color
        });
    }

    for (std::size_t point_index = 0; point_index + 1 < points.get_size(); ++point_index) {
        const unsigned int current_left = static_cast<unsigned int>(point_index * 2);
        const unsigned int current_right = current_left + 1;
        const unsigned int next_left = current_left + 2;
        const unsigned int next_right = current_left + 3;

        indices.push_back(current_left);
        indices.push_back(next_left);
        indices.push_back(current_right);

        indices.push_back(current_right);
        indices.push_back(next_left);
        indices.push_back(next_right);
    }

    mesh.update_data(vertices.to_std_vector(), indices.to_std_vector());
}

engine::math::Vector BrownianTrail::get_tangent(std::size_t point_index) const {
    if (points.get_size() < 2) {
        return engine::math::Vector(1.0f, 0.0f, 0.0f);
    }

    if (point_index == 0) {
        return safe_normalize(points[1].position - points[0].position, engine::math::Vector(1.0f, 0.0f, 0.0f));
    }

    if (point_index + 1 == points.get_size()) {
        return safe_normalize(
            points[point_index].position - points[point_index - 1].position,
            engine::math::Vector(1.0f, 0.0f, 0.0f)
        );
    }

    return safe_normalize(
        points[point_index + 1].position - points[point_index - 1].position,
        engine::math::Vector(1.0f, 0.0f, 0.0f)
    );
}

engine::math::Vector BrownianTrail::get_side_vector(
    std::size_t point_index,
    const engine::math::Vector& camera_position
) const {
    const engine::math::Vector tangent = get_tangent(point_index);
    const engine::math::Vector view_direction =
        safe_normalize(camera_position - points[point_index].position, engine::math::Vector(0.0f, 0.0f, 1.0f));

    engine::math::Vector side = tangent.crossprod(view_direction);
    if (side.dist() > min_vector_length) {
        return side.normalize();
    }

    side = tangent.crossprod(engine::math::Vector(0.0f, 1.0f, 0.0f));
    if (side.dist() > min_vector_length) {
        return side.normalize();
    }

    return engine::math::Vector(1.0f, 0.0f, 0.0f);
}

engine::math::Vector4 BrownianTrail::get_point_color(float current_time, const TrailPoint& point) const {
    const float age = current_time - point.time;
    const float life = clamp01(1.0f - age / settings.max_age);
    const float alpha = settings.color.get_w() * life * life;

    return engine::math::Vector4(
        settings.color.get_x(),
        settings.color.get_y(),
        settings.color.get_z(),
        alpha
    );
}

float BrownianTrail::get_point_width(float current_time, const TrailPoint& point) const {
    const float age = current_time - point.time;
    const float life = clamp01(1.0f - age / settings.max_age);

    return settings.width * (0.35f + 0.65f * life);
}
