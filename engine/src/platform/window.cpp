#include "../include/platform/window.hpp"

namespace engine::platform {

Window::Window() = default;

Window::~Window() {
    destroy();
}

bool Window::create(const char* title, unsigned int scr_width, unsigned int scr_height) {
    window_width = scr_width;
    window_height = scr_height;

    window = glfwCreateWindow(scr_width, scr_height, title, NULL, NULL);
    if (window == NULL) {
        return false;
    }

    glfwMakeContextCurrent(window);
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
    return static_cast<float>(window_width) / static_cast<float>(window_height);
}

GLFWwindow* Window::get_window() const {
    return window;
}

void Window::swap_buffers() {
    glfwSwapBuffers(window);
}

void Window::poll_events() {
    glfwPollEvents();
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

}  // namespace engine::platform
