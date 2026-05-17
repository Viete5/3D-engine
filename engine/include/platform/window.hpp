#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine::platform {

class Window final{
public:
    Window() = default;
    ~Window();
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool create(const char* title = "", unsigned int scr_width = 800, unsigned int scr_height = 600);
    void destroy();

    bool should_close() const;
    void set_should_close(bool value);

    unsigned int get_width() const;
    unsigned int get_height() const;
    float get_aspect_ratio() const;

    GLFWwindow* get_window() const;

    bool get_is_fullscreen() const;

    void toggle_fullscreen();
    void set_cursor_disabled(bool value);
    void update_framebuffer_size();

    void swap_buffers();
    void poll_events();
private:
    GLFWwindow* window = nullptr;
    unsigned int window_width = 0;
    unsigned int window_height = 0;
    bool is_fullscreen = false;

    int windowed_pos_x = 0;
    int windowed_pos_y = 0;
    int windowed_width = 0;
    int windowed_height = 0;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};






} // engine::platform


#endif // WINDOW_HPP
