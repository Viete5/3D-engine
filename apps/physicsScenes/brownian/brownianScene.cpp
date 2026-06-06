#include "brownianScene.hpp"

#include "../../../engine/include/core/log.hpp"
#include "../../../engine/include/core/paths.hpp"
#include "../../../engine/include/math/vector.hpp"
#include "../../../engine/include/math/vector4.hpp"
#include "../../../engine/include/platform/input.hpp"
#include "../../../engine/include/render/font.hpp"
#include "../../../engine/include/render/primitives.hpp"
#include "../../../engine/include/render/textRenderer.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

namespace {

constexpr float fps_update_interval = 0.25f;

engine::scene::Camera create_default_camera() {
    return engine::scene::Camera(
        engine::math::Vector(0.0f, 0.7f, 8.0f),
        engine::math::Vector(0.0f, 0.0f, -1.0f),
        engine::math::Vector(0.0f, 1.0f, 0.0f),
        45.0f,
        800.0f / 600.0f
    );
}

std::vector<engine::scene::Light> create_default_lights() {
    return {
        engine::scene::Light(
            engine::math::Vector(3.5f, 4.5f, 4.0f),
            engine::math::Vector(1.0f, 0.95f, 0.85f)
        ),
        engine::scene::Light(
            engine::math::Vector(-4.0f, 2.4f, -3.5f),
            engine::math::Vector(0.35f, 0.55f, 0.95f)
        ),
        engine::scene::Light(
            engine::math::Vector(0.0f, -3.0f, 4.5f),
            engine::math::Vector(0.35f, 0.9f, 0.65f)
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

std::string format_percent(float value) {
    std::ostringstream stream;
    stream << std::fixed << std::setprecision(0) << value * 100.0f << "%";
    return stream.str();
}

} // namespace

BrownianScene::BrownianScene(const engine::core::Paths& paths, engine::core::Logger& logger)
    : Scene({}, create_default_camera(), create_default_lights()),
      large_particle_simulation(),
      gas_mixing_simulation(),
      shader(
          paths.get_shader_path("sandbox.vs").string().c_str(),
          paths.get_shader_path("sandbox.fs").string().c_str()
      ),
      particle_mesh(engine::render::create_sphere_mesh(1.0f, 12, 16)),
      container_mesh(engine::render::create_cube_mesh()),
      large_particle_trail(),
      small_particle_material(shader),
      large_particle_material(shader),
      gas_a_material(shader),
      gas_b_material(shader),
      container_material(shader),
      partition_material(shader),
      trail_material(shader) {
    small_particle_material.set_base_color(engine::math::Vector(0.15f, 0.52f, 1.0f));
    large_particle_material.set_base_color(engine::math::Vector(1.0f, 0.67f, 0.18f));
    gas_a_material.set_base_color(engine::math::Vector4(0.08f, 0.52f, 1.0f, 1.0f));
    gas_b_material.set_base_color(engine::math::Vector4(1.0f, 0.26f, 0.12f, 1.0f));
    container_material.set_base_color(engine::math::Vector4(0.12f, 0.38f, 0.78f, 0.11f));
    trail_material.set_base_color(engine::math::Vector4(1.0f, 0.78f, 0.22f, 0.9f));

    large_particle_trail.reset(large_particle_simulation.get_large_particle().position, 0.0f);
    rebuild_scene_objects();

    logger.info(
        engine::core::LogCategory::Scene,
        "Brownian scene created: default mode=large-particle particles=" +
            std::to_string(large_particle_simulation.get_particles().size())
    );
}

void BrownianScene::update(const engine::scene::SceneUpdateContext& context) {
    update_fps(context.delta_time);
    handle_camera_input(context);
    handle_simulation_input(context);

    if (mode == Mode::LargeParticle) {
        large_particle_simulation.update(context.delta_time);
        large_particle_trail.update(
            large_particle_simulation.get_large_particle().position,
            camera.get_position(),
            context.elapsed_time
        );
        sync_large_particle_objects();
    } else {
        gas_mixing_simulation.update(context.delta_time);
        sync_gas_particle_objects();
        update_partition_visual();
    }
}

void BrownianScene::render_ui(
    engine::render::TextRenderer& text_renderer,
    const engine::render::Font& font,
    unsigned int viewport_width,
    unsigned int viewport_height
) const {
    if (mode == Mode::LargeParticle) {
        render_large_particle_ui(text_renderer, font, viewport_width, viewport_height);
    } else {
        render_gas_mixing_ui(text_renderer, font, viewport_width, viewport_height);
    }
}

void BrownianScene::set_mode(Mode new_mode, float elapsed_time) {
    if (mode == new_mode) {
        return;
    }

    mode = new_mode;
    if (mode == Mode::LargeParticle) {
        large_particle_trail.reset(large_particle_simulation.get_large_particle().position, elapsed_time);
    }

    rebuild_scene_objects();
}

void BrownianScene::rebuild_scene_objects() {
    objects.clear();
    particle_object_count = 0;
    large_particle_object_index = 0;

    if (mode == Mode::LargeParticle) {
        create_large_particle_objects();
        create_container_object(large_particle_simulation.get_settings().bounds, "large_particle_container");
        create_trail_object();
    } else {
        create_gas_particle_objects();
        create_container_object(gas_mixing_simulation.get_settings().bounds, "gas_mixing_container");
        create_partition_object();
    }
}

void BrownianScene::create_large_particle_objects() {
    const std::vector<BrownianParticle>& particles = large_particle_simulation.get_particles();
    particle_object_count = particles.size();
    objects.reserve(particle_object_count + 3);

    for (std::size_t index = 0; index < particle_object_count; ++index) {
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
        make_particle_transform(large_particle_simulation.get_large_particle()),
        &large_particle_material,
        &particle_mesh
    );

    synced_large_particle_revision = large_particle_simulation.get_particle_revision();
}

void BrownianScene::create_gas_particle_objects() {
    const std::vector<BrownianParticle>& particles = gas_mixing_simulation.get_particles();
    particle_object_count = particles.size();
    objects.reserve(particles.size() + 2);

    for (std::size_t index = 0; index < particles.size(); ++index) {
        objects.emplace_back(
            "gas_particle_" + std::to_string(index),
            make_particle_transform(particles[index]),
            get_gas_particle_material(particles[index].gas_type),
            &particle_mesh
        );
    }

    synced_gas_particle_revision = gas_mixing_simulation.get_particle_revision();
}

void BrownianScene::create_container_object(const engine::math::Vector& bounds, const std::string& name) {
    engine::scene::Transform container_transform(
        engine::math::Vector(0.0f, 0.0f, 0.0f),
        engine::math::Vector(0.0f, 0.0f, 0.0f),
        engine::math::Vector(bounds.get_x() * 2.0f, bounds.get_y() * 2.0f, bounds.get_z() * 2.0f)
    );

    objects.emplace_back(
        name,
        container_transform,
        &container_material,
        &container_mesh
    );
}

void BrownianScene::create_partition_object() {
    const GasMixingSimulationSettings& settings = gas_mixing_simulation.get_settings();
    const engine::math::Vector bounds = settings.bounds;

    engine::scene::Transform partition_transform(
        engine::math::Vector(0.0f, 0.0f, 0.0f),
        engine::math::Vector(0.0f, 0.0f, 0.0f),
        engine::math::Vector(
            settings.partition_thickness,
            bounds.get_y() * 2.0f,
            bounds.get_z() * 2.0f
        )
    );

    objects.emplace_back(
        "mixing_partition",
        partition_transform,
        &partition_material,
        &container_mesh
    );

    update_partition_visual();
}

void BrownianScene::create_trail_object() {
    objects.emplace_back(
        "large_particle_trail",
        engine::scene::Transform(),
        &trail_material,
        &large_particle_trail.get_mesh()
    );
}

void BrownianScene::sync_large_particle_objects() {
    if (synced_large_particle_revision != large_particle_simulation.get_particle_revision()) {
        rebuild_scene_objects();
    }

    const std::vector<BrownianParticle>& particles = large_particle_simulation.get_particles();
    const std::size_t synced_count = std::min(particles.size(), particle_object_count);

    for (std::size_t index = 0; index < synced_count; ++index) {
        objects[index].get_transform().set_position(particles[index].position);
    }

    if (large_particle_object_index < objects.size()) {
        objects[large_particle_object_index].get_transform().set_position(
            large_particle_simulation.get_large_particle().position
        );
    }
}

void BrownianScene::sync_gas_particle_objects() {
    if (synced_gas_particle_revision != gas_mixing_simulation.get_particle_revision()) {
        rebuild_scene_objects();
    }

    const std::vector<BrownianParticle>& particles = gas_mixing_simulation.get_particles();
    const std::size_t synced_count = std::min(particles.size(), particle_object_count);

    for (std::size_t index = 0; index < synced_count; ++index) {
        objects[index].get_transform().set_position(particles[index].position);
    }
}

void BrownianScene::update_partition_visual() {
    if (gas_mixing_simulation.get_valve_open()) {
        partition_material.set_base_color(engine::math::Vector4(0.15f, 1.0f, 0.55f, 0.08f));
    } else {
        partition_material.set_base_color(engine::math::Vector4(1.0f, 0.82f, 0.18f, 0.30f));
    }
}

void BrownianScene::update_fps(float delta_time) {
    if (delta_time <= 0.0f) {
        return;
    }

    fps_time_accumulator += delta_time;
    ++fps_frame_count;

    if (fps_time_accumulator >= fps_update_interval) {
        current_fps = static_cast<float>(fps_frame_count) / fps_time_accumulator;
        fps_time_accumulator = 0.0f;
        fps_frame_count = 0;
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

    if (input.is_key_pressed(engine::platform::Key::M)) {
        set_mode(mode == Mode::LargeParticle ? Mode::GasMixing : Mode::LargeParticle, context.elapsed_time);
        return;
    }

    if (mode == Mode::LargeParticle) {
        handle_large_particle_input(context);
    } else {
        handle_gas_mixing_input(context);
    }
}

void BrownianScene::handle_large_particle_input(const engine::scene::SceneUpdateContext& context) {
    const engine::platform::Input& input = context.input;
    const float delta_time = context.delta_time;

    float temperature = large_particle_simulation.get_temperature();
    if (input.is_key_down(engine::platform::Key::Q)) {
        temperature -= delta_time;
    }
    if (input.is_key_down(engine::platform::Key::E)) {
        temperature += delta_time;
    }
    if (input.is_key_pressed(engine::platform::Key::P)) {
        large_particle_simulation.toggle_pause();
    }
    if (input.is_key_pressed(engine::platform::Key::R)) {
        large_particle_simulation.reset();
        large_particle_trail.reset(large_particle_simulation.get_large_particle().position, context.elapsed_time);
        rebuild_scene_objects();
    }
    if (input.is_key_pressed(engine::platform::Key::Z)) {
        large_particle_simulation.change_particle_count(-large_particle_simulation.get_settings().density_step);
    }
    if (input.is_key_pressed(engine::platform::Key::X)) {
        large_particle_simulation.change_particle_count(large_particle_simulation.get_settings().density_step);
    }

    large_particle_simulation.set_temperature(std::clamp(temperature, 0.1f, 5.0f));
}

void BrownianScene::handle_gas_mixing_input(const engine::scene::SceneUpdateContext& context) {
    const engine::platform::Input& input = context.input;
    const float delta_time = context.delta_time;
    const GasMixingSimulationSettings& settings = gas_mixing_simulation.get_settings();

    float temperature = gas_mixing_simulation.get_temperature();
    if (input.is_key_down(engine::platform::Key::Q)) {
        temperature -= delta_time;
    }
    if (input.is_key_down(engine::platform::Key::E)) {
        temperature += delta_time;
    }

    if (input.is_key_pressed(engine::platform::Key::O)) {
        gas_mixing_simulation.toggle_valve();
    }
    if (input.is_key_pressed(engine::platform::Key::P)) {
        gas_mixing_simulation.toggle_pause();
    }
    if (input.is_key_pressed(engine::platform::Key::R)) {
        gas_mixing_simulation.reset();
        rebuild_scene_objects();
    }
    if (input.is_key_pressed(engine::platform::Key::Z)) {
        gas_mixing_simulation.change_gas_particle_count(BrownianGasType::GasA, -settings.density_step);
    }
    if (input.is_key_pressed(engine::platform::Key::X)) {
        gas_mixing_simulation.change_gas_particle_count(BrownianGasType::GasA, settings.density_step);
    }
    if (input.is_key_pressed(engine::platform::Key::C)) {
        gas_mixing_simulation.change_gas_particle_count(BrownianGasType::GasB, -settings.density_step);
    }
    if (input.is_key_pressed(engine::platform::Key::V)) {
        gas_mixing_simulation.change_gas_particle_count(BrownianGasType::GasB, settings.density_step);
    }

    gas_mixing_simulation.set_temperature(std::clamp(temperature, 0.1f, 5.0f));
}

void BrownianScene::render_large_particle_ui(
    engine::render::TextRenderer& text_renderer,
    const engine::render::Font& font,
    unsigned int viewport_width,
    unsigned int viewport_height
) const {
    std::ostringstream hud;
    hud << std::fixed << std::setprecision(2)
        << "Brownian Motion Demo\n"
        << "Mode: large particle\n"
        << "FPS: " << current_fps << "\n"
        << "Temperature: " << large_particle_simulation.get_temperature() << "\n"
        << "Particles: " << large_particle_simulation.get_particles().size() << "\n"
        << "Paused: " << (large_particle_simulation.get_paused() ? "yes" : "no") << "\n\n"
        << "M - switch to gas mixing\n"
        << "Z/X - particle density\n"
        << "Q/E - temperature\n"
        << "P - pause   R - reset\n"
        << "WASD/Space/Ctrl + Mouse - camera\n"
        << "F11 - fullscreen   Esc - exit";

    engine::render::TextLayoutSettings settings;
    settings.font_size = 18.0f;
    settings.line_spacing = 1.08f;
    settings.vertical_align = engine::render::TextVerticalAlign::Top;
    settings.color = engine::math::Vector4(0.92f, 0.96f, 1.0f, 0.95f);

    text_renderer.draw_text(
        font,
        hud.str(),
        20.0f,
        static_cast<float>(viewport_height) - 20.0f,
        viewport_width,
        viewport_height,
        settings
    );
}

void BrownianScene::render_gas_mixing_ui(
    engine::render::TextRenderer& text_renderer,
    const engine::render::Font& font,
    unsigned int viewport_width,
    unsigned int viewport_height
) const {
    const GasMixingStats stats = gas_mixing_simulation.get_gas_stats();

    std::ostringstream hud;
    hud << std::fixed << std::setprecision(2)
        << "Brownian Gas Mixing Lab\n"
        << "Mode: gas mixing\n"
        << "FPS: " << current_fps << "\n"
        << "Valve: " << (gas_mixing_simulation.get_valve_open() ? "OPEN" : "CLOSED") << "\n"
        << "Mixing: " << format_percent(stats.mixing_factor) << "\n"
        << "Temperature: " << gas_mixing_simulation.get_temperature() << "\n"
        << "Paused: " << (gas_mixing_simulation.get_paused() ? "yes" : "no") << "\n\n"
        << "Gas A blue: " << stats.gas_a_total
        << "  L/R " << stats.gas_a_left << "/" << stats.gas_a_right << "\n"
        << "Gas B orange: " << stats.gas_b_total
        << "  L/R " << stats.gas_b_left << "/" << stats.gas_b_right << "\n\n"
        << "M - switch to large particle\n"
        << "O - open/close valve\n"
        << "Z/X - gas A density\n"
        << "C/V - gas B density\n"
        << "Q/E - temperature\n"
        << "P - pause   R - reset\n"
        << "WASD/Space/Ctrl + Mouse - camera\n"
        << "F11 - fullscreen   Esc - exit";

    engine::render::TextLayoutSettings settings;
    settings.font_size = 18.0f;
    settings.line_spacing = 1.08f;
    settings.vertical_align = engine::render::TextVerticalAlign::Top;
    settings.color = engine::math::Vector4(0.92f, 0.97f, 1.0f, 0.96f);

    text_renderer.draw_text(
        font,
        hud.str(),
        20.0f,
        static_cast<float>(viewport_height) - 20.0f,
        viewport_width,
        viewport_height,
        settings
    );
}

engine::render::Material* BrownianScene::get_gas_particle_material(BrownianGasType gas_type) {
    return gas_type == BrownianGasType::GasA ? &gas_a_material : &gas_b_material;
}
