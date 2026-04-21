#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace engine::platform {

class Window final{
public:
    Window();
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

    void swap_buffers();
    void poll_events();
private:
    GLFWwindow* window = nullptr;
    unsigned int window_width = 0;
    unsigned int window_height = 0;

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
};






} // engine::platform


#endif // WINDOW_HPP
