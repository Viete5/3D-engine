#ifndef INPUT_HPP
#define INPUT_HPP

#include <array>
#include <cstddef>

struct GLFWwindow;

namespace engine::platform {

enum class Key {
    W,
    A,
    S,
    D,
    Q,
    E,
    P,
    R,
    Escape,
    Space,
    LeftControl,
    F11,
    Count
};

enum class MouseButton {
    Left,
    Right,
    Middle
};

class Input final {
public:
    Input();

    void attach(GLFWwindow* window);
    void begin_frame();
    void update_key_states();

    bool is_key_down(Key key) const;
    bool is_key_pressed(Key key) const;
    bool is_mouse_button_down(MouseButton button) const;

    float get_mouse_delta_x() const;
    float get_mouse_delta_y() const;

private:
    static void cursor_position_callback(GLFWwindow* window, double x, double y);

    void update_mouse_position(double x, double y);
    static std::size_t get_key_index(Key key);

    GLFWwindow* window = nullptr;

    std::array<bool, static_cast<std::size_t>(Key::Count)> current_keys{};
    std::array<bool, static_cast<std::size_t>(Key::Count)> previous_keys{};

    float delta_x = 0.0f;
    float delta_y = 0.0f;
    float last_x = 0.0f;
    float last_y = 0.0f;
    bool first_mouse_event = true;
};

}  // namespace engine::platform

#endif  // INPUT_HPP
