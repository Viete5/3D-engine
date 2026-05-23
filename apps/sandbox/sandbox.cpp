#include "sandbox.hpp"

#include "../../engine/include/assets/modelLoader.hpp"
#include "../../engine/include/core/log.hpp"
#include "../../engine/include/core/paths.hpp"
#include "../../engine/include/math/vector.hpp"
#include "../../engine/include/platform/input.hpp"

#include <algorithm>

namespace {

constexpr const char* tiger_model_file = "tiger.obj";

engine::scene::Camera create_default_camera() {
    return engine::scene::Camera(
        engine::math::Vector(0.0f, 0.4f, 3.0f),
        engine::math::Vector(0.0f, 0.0f, -1.0f),
        engine::math::Vector(0.0f, 1.0f, 0.0f),
        45.0f,
        800.0f / 600.0f
    );
}

std::vector<engine::scene::Light> create_default_lights() {
    return {
        engine::scene::Light(
            engine::math::Vector(3.0f, 4.0f, 3.0f),
            engine::math::Vector(1.0f, 1.0f, 1.0f)
        )
    };
}

} // namespace

Sandbox::Sandbox(const engine::core::Paths& paths, engine::core::Logger& logger)
    : Scene({}, create_default_camera(), create_default_lights()),
      shader(
          paths.get_shader_path("sandbox.vs").string().c_str(),
          paths.get_shader_path("sandbox.fs").string().c_str()
      ) {
    engine::assets::ModelLoader model_loader;
    model_loader.set_logger(&logger);

    tiger_model = model_loader.load(paths.get_model_path(tiger_model_file), shader);
    objects.insert(
        objects.end(),
        tiger_model.get_objects().begin(),
        tiger_model.get_objects().end()
    );
}

void Sandbox::update(const engine::scene::SceneUpdateContext& context) {
    handle_camera_input(context);
}

void Sandbox::handle_camera_input(const engine::scene::SceneUpdateContext& context) {
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

    float fov = camera.get_fov();
    if (input.is_key_down(engine::platform::Key::Q)) {
        fov -= 40.0f * delta_time;
    }
    if (input.is_key_down(engine::platform::Key::E)) {
        fov += 40.0f * delta_time;
    }
    camera.set_fov(std::clamp(fov, 10.0f, 60.0f));

    camera.update_angle(input.get_mouse_delta_x(), input.get_mouse_delta_y());
}
