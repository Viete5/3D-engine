#include "../include/platform/input.hpp"

#include <GLFW/glfw3.h>

namespace engine::platform {

namespace {

int to_glfw_key(Key key) {
    switch (key) {
        case Key::W:
            return GLFW_KEY_W;
        case Key::A:
            return GLFW_KEY_A;
        case Key::S:
            return GLFW_KEY_S;
        case Key::D:
            return GLFW_KEY_D;
        case Key::Q:
            return GLFW_KEY_Q;
        case Key::E:
            return GLFW_KEY_E;
        case Key::M:
            return GLFW_KEY_M;
        case Key::O:
            return GLFW_KEY_O;
        case Key::Escape:
            return GLFW_KEY_ESCAPE;
        case Key::P:
            return GLFW_KEY_P;
        case Key::R:
            return GLFW_KEY_R;
        case Key::Z:
            return GLFW_KEY_Z;
        case Key::X:
            return GLFW_KEY_X;
        case Key::C:
            return GLFW_KEY_C;
        case Key::V:
            return GLFW_KEY_V;
        case Key::Space:
            return GLFW_KEY_SPACE;
        case Key::LeftControl:
            return GLFW_KEY_LEFT_CONTROL;
        case Key::F11:
            return GLFW_KEY_F11;
        case Key::Count:
            return GLFW_KEY_UNKNOWN;
    }

    return GLFW_KEY_UNKNOWN;
}

int to_glfw_mouse_button(MouseButton button) {
    switch (button) {
        case MouseButton::Left:
            return GLFW_MOUSE_BUTTON_LEFT;
        case MouseButton::Right:
            return GLFW_MOUSE_BUTTON_RIGHT;
        case MouseButton::Middle:
            return GLFW_MOUSE_BUTTON_MIDDLE;
    }

    return GLFW_MOUSE_BUTTON_LEFT;
}

}  // namespace

Input::Input() = default;

void Input::attach(GLFWwindow* window) {
    this->window = window;
    glfwSetWindowUserPointer(window, this);
    glfwSetCursorPosCallback(window, Input::cursor_position_callback);
}

void Input::begin_frame() {
    previous_keys = current_keys;
    delta_x = 0.0f;
    delta_y = 0.0f;
}

void Input::update_key_states() {
    if (!window) {
        return;
    }

    for (std::size_t index = 0; index < current_keys.size(); ++index) {
        const Key key = static_cast<Key>(index);
        current_keys[index] = glfwGetKey(window, to_glfw_key(key)) == GLFW_PRESS;
    }
}

bool Input::is_key_down(Key key) const {
    if (!window) {
        return false;
    }

    if (key == Key::Count) {
        return false;
    }

    return current_keys[get_key_index(key)];
}

bool Input::is_key_pressed(Key key) const {
    if (!window) {
        return false;
    }

    if (key == Key::Count) {
        return false;
    }

    const std::size_t key_index = get_key_index(key);
    return current_keys[key_index] && !previous_keys[key_index];
}

bool Input::is_mouse_button_down(MouseButton button) const {
    if (!window) {
        return false;
    }

    return glfwGetMouseButton(window, to_glfw_mouse_button(button)) == GLFW_PRESS;
}

float Input::get_mouse_delta_x() const {
    return delta_x;
}

float Input::get_mouse_delta_y() const {
    return delta_y;
}

void Input::cursor_position_callback(GLFWwindow* window, double x, double y) {
    auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));
    if (!input) {
        return;
    }

    input->update_mouse_position(x, y);
}

void Input::update_mouse_position(double x, double y) {
    if (first_mouse_event) {
        last_x = static_cast<float>(x);
        last_y = static_cast<float>(y);
        first_mouse_event = false;
        return;
    }

    delta_x = static_cast<float>(x) - last_x;
    delta_y = last_y - static_cast<float>(y);

    last_x = static_cast<float>(x);
    last_y = static_cast<float>(y);
}

std::size_t Input::get_key_index(Key key) {
    return static_cast<std::size_t>(key);
}

}  // namespace engine::platform
