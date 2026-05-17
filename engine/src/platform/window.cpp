#include "../include/platform/window.hpp"

namespace engine::platform {

Window::~Window() {
    destroy();
}

bool Window::create(const char* title, unsigned int scr_width, unsigned int scr_height) {
    window_width = scr_width;
    window_height = scr_height;
    windowed_width = static_cast<int>(scr_width);
    windowed_height = static_cast<int>(scr_height);

    window = glfwCreateWindow(scr_width, scr_height, title, nullptr, nullptr);
    if (window == nullptr) {
        return false;
    }

    glfwMakeContextCurrent(window);
    glfwGetWindowPos(window, &windowed_pos_x, &windowed_pos_y);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    return true;
}

void Window::destroy() {
    if (!window) {
        return;
    }

    glfwDestroyWindow(window);
    window = nullptr;
}

bool Window::should_close() const {
    return glfwWindowShouldClose(window);
}

void Window::set_should_close(bool value) {
    glfwSetWindowShouldClose(window, value ? GLFW_TRUE : GLFW_FALSE);
}

unsigned int Window::get_height() const {
    return window_height;
}

unsigned int Window::get_width() const {
    return window_width;
}

float Window::get_aspect_ratio() const {
    if (window_height == 0) {
        return 1.0f;
    }

    return static_cast<float>(window_width) / static_cast<float>(window_height);
}

GLFWwindow* Window::get_window() const {
    return window;
}

bool Window::get_is_fullscreen() const {
    return is_fullscreen;
}

void Window::toggle_fullscreen() {
    if (!window) {
        return;
    }

    if (!is_fullscreen) {
        glfwGetWindowPos(window, &windowed_pos_x, &windowed_pos_y);
        glfwGetWindowSize(window, &windowed_width, &windowed_height);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor) {
            return;
        }

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) {
            return;
        }

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        window_width = static_cast<unsigned int>(mode->width);
        window_height = static_cast<unsigned int>(mode->height);
        is_fullscreen = true;
        glViewport(0, 0, mode->width, mode->height);
        return;
    }

    glfwSetWindowMonitor(window, nullptr, windowed_pos_x, windowed_pos_y, windowed_width, windowed_height, 0);
    window_width = static_cast<unsigned int>(windowed_width);
    window_height = static_cast<unsigned int>(windowed_height);
    is_fullscreen = false;
    glViewport(0, 0, windowed_width, windowed_height);
}

void Window::set_cursor_disabled(bool value) {
    if (!window) {
        return;
    }

    glfwSetInputMode(window, GLFW_CURSOR, value ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
}

void Window::update_framebuffer_size() {
    if (!window) {
        return;
    }

    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);

    if (width <= 0 || height <= 0) {
        return;
    }

    window_width = static_cast<unsigned int>(width);
    window_height = static_cast<unsigned int>(height);
    glViewport(0, 0, width, height);
}

void Window::swap_buffers() {
    glfwSwapBuffers(window);
}

void Window::poll_events() {
    glfwPollEvents();
    update_framebuffer_size();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

}  // namespace engine::platform
