#include "Window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void error_callback(int /*error*/, const char* description) {
    fputs(description, stderr);
}

Window::Window() {
    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
        std::exit(1);
    }

    // Specify that we want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window and OpenGL context
    m_window = glfwCreateWindow(win_x, win_y, "OpenGL Car Game", nullptr, nullptr);
    if (m_window == nullptr) {
        fmt::print(stderr, "Failed to create window");
        glfwTerminate();
        exit(1);
    }

    glfwSetWindowUserPointer(m_window, this);

    // Set callbacks key press and mouse press.
    glfwSetKeyCallback(m_window, key_callback);
    glfwSetCursorPosCallback(m_window, mouse_pos_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetFramebufferSizeCallback(m_window, reshape_callback);
    glfwSetScrollCallback(m_window, scroll_callback);

    glfwMakeContextCurrent(m_window);
    glfwSwapInterval(1);

    if (!gladLoadGL()) {
        fmt::print(stderr, "Failed to load OpenGL");
        glfwTerminate();
        exit(1);
    }
    glfwGetFramebufferSize(m_window, &win_x, &win_y);

    // Sets the (background) colour for each time the frame-buffer
    // (colour buffer) is cleared
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* this_window = (Window*)glfwGetWindowUserPointer(window);
    this_window->m_key_callback(window, key, scancode, action, mods);
}

void Window::mouse_pos_callback(GLFWwindow* window, double x, double y) {
    auto* this_window = (Window*)glfwGetWindowUserPointer(window);
    this_window->m_mouse_pos_callback(window, x, y);
}

void Window::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    auto* this_window = (Window*)glfwGetWindowUserPointer(window);
    this_window->m_scroll_callback(window, xoffset, yoffset);
}

void Window::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto* this_window = (Window*)glfwGetWindowUserPointer(window);
    this_window->m_mouse_button_callback(window, button, action, mods);
}

void Window::reshape_callback(GLFWwindow* window, int x, int y) {
    auto* this_window = (Window*)glfwGetWindowUserPointer(window);
    this_window->win_x = x;
    this_window->win_y = y;
    this_window->m_reshape_callback(window, x, y);
}

GLFWwindow* Window::get_window() const {
    return m_window;
}
