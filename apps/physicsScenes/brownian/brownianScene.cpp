#include "brownianScene.hpp"

#include "../../../engine/include/core/log.hpp"
#include "../../../engine/include/core/paths.hpp"
#include "../../../engine/include/math/vector.hpp"
#include "../../../engine/include/math/vector4.hpp"
#include "../../../engine/include/platform/input.hpp"
#include "../../../engine/include/render/primitives.hpp"

#include <algorithm>
#include <string>

namespace {

engine::scene::Camera create_default_camera() {
    return engine::scene::Camera(
        engine::math::Vector(0.0f, 0.6f, 7.0f),
        engine::math::Vector(0.0f, 0.0f, -1.0f),
        engine::math::Vector(0.0f, 1.0f, 0.0f),
        45.0f,
        800.0f / 600.0f
    );
}

std::vector<engine::scene::Light> create_default_lights() {
    return {
        engine::scene::Light(
            engine::math::Vector(3.0f, 4.0f, 4.0f),
            engine::math::Vector(1.0f, 1.0f, 1.0f)
        ),
        engine::scene::Light(
            engine::math::Vector(-4.0f, 2.0f, -3.0f),
            engine::math::Vector(0.35f, 0.45f, 0.7f)
        )
    };
}

engine::scene::Transform make_particle_transform(const BrownianParticle& particle) {
    return engine::scene::Transform(
        particle.position,
        engine::math::Vector(0.0f, 0.0f, 0.0f),
        engine::math::Vector(particle.radius, particle.radius, particle.radius)
    );
}

} // namespace

BrownianScene::BrownianScene(const engine::core::Paths& paths, engine::core::Logger& logger)
    : Scene({}, create_default_camera(), create_default_lights()),
      simulation(),
      shader(
          paths.get_shader_path("sandbox.vs").string().c_str(),
          paths.get_shader_path("sandbox.fs").string().c_str()
      ),
      particle_mesh(engine::render::create_sphere_mesh(1.0f, 12, 16)),
      container_mesh(engine::render::create_cube_mesh()),
      small_particle_material(shader),
      large_particle_material(shader),
      container_material(shader) {
    small_particle_material.set_base_color(engine::math::Vector(0.15f, 0.52f, 1.0f));
    large_particle_material.set_base_color(engine::math::Vector(1.0f, 0.67f, 0.18f));
    container_material.set_base_color(engine::math::Vector4(0.3f,0.1f,0.1f,0.2f));

    objects.clear();
    create_particle_objects();
    create_container_object();

    logger.info(
        engine::core::LogCategory::Scene,
        "Brownian scene created: particles=" +
            std::to_string(simulation.get_particles().size()) +
            " temperature=" +
            std::to_string(simulation.get_temperature())
    );
}

void BrownianScene::update(const engine::scene::SceneUpdateContext& context) {
    handle_camera_input(context);
    handle_simulation_input(context);

    simulation.update(context.delta_time);
    sync_particle_objects();
}

void BrownianScene::create_particle_objects() {
    const std::vector<BrownianParticle>& particles = simulation.get_particles();
    objects.reserve(particles.size() + 1);

    for (std::size_t index = 0; index < particles.size(); ++index) {
        objects.emplace_back(
            "small_particle_" + std::to_string(index),
            make_particle_transform(particles[index]),
            &small_particle_material,
            &particle_mesh
        );
    }

    large_particle_object_index = objects.size();
    objects.emplace_back(
        "large_brownian_particle",
        make_particle_transform(simulation.get_large_particle()),
        &large_particle_material,
        &particle_mesh
    );
}

void BrownianScene::create_container_object() {
    engine::math::Vector bounds = simulation.get_settings().bounds;

    engine::scene::Transform container_transform(
        engine::math::Vector(0.0f,0.0f,0.0f),
        engine::math::Vector(0.0f,0.0f,0.0f),
        engine::math::Vector(bounds.get_x()*2, bounds.get_y()*2, bounds.get_z()*2)
    );

    objects.emplace_back(
        "container",
        container_transform,
        &container_material,
        &container_mesh
    );
}

void BrownianScene::sync_particle_objects() {
    const std::vector<BrownianParticle>& particles = simulation.get_particles();
    const std::size_t synced_count = std::min(particles.size(), objects.size());

    for (std::size_t index = 0; index < synced_count; ++index) {
        objects[index].get_transform().set_position(particles[index].position);
    }

    if (large_particle_object_index < objects.size()) {
        objects[large_particle_object_index].get_transform().set_position(
            simulation.get_large_particle().position
        );
    }
}

void BrownianScene::handle_camera_input(const engine::scene::SceneUpdateContext& context) {
    const engine::platform::Input& input = context.input;
    const float delta_time = context.delta_time;

    if (input.is_key_down(engine::platform::Key::W)) {
        camera.process_keyboard(engine::scene::CameraMovement::Forward, delta_time);
    }
    if (input.is_key_down(engine::platform::Key::S)) {
        camera.process_keyboard(engine::scene::CameraMovement::Backward, delta_time);
    }
    if (input.is_key_down(engine::platform::Key::A)) {
        camera.process_keyboard(engine::scene::CameraMovement::Left, delta_time);
    }
    if (input.is_key_down(engine::platform::Key::D)) {
        camera.process_keyboard(engine::scene::CameraMovement::Right, delta_time);
    }
    if (input.is_key_down(engine::platform::Key::Space)) {
        camera.process_keyboard(engine::scene::CameraMovement::Up, delta_time);
    }
    if (input.is_key_down(engine::platform::Key::LeftControl)) {
        camera.process_keyboard(engine::scene::CameraMovement::Down, delta_time);
    }

    camera.update_angle(input.get_mouse_delta_x(), input.get_mouse_delta_y());
}

void BrownianScene::handle_simulation_input(const engine::scene::SceneUpdateContext& context) {
    const engine::platform::Input& input = context.input;
    const float delta_time = context.delta_time;

    float temperature = simulation.get_temperature();
    if (input.is_key_down(engine::platform::Key::Q)) {
        temperature -= delta_time;
    }
    if (input.is_key_down(engine::platform::Key::E)) {
        temperature += delta_time;
    }

    simulation.set_temperature(std::clamp(temperature, 0.1f, 5.0f));
}
