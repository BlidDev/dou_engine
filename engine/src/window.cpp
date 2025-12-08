#include "window.h"


namespace engine {

    static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    Window::Window() {
        window = nullptr;
        w = 0; h = 0;
        title = "UNDEFINED";
        fb_size_callback = nullptr;
    }

    Window::Window(const std::string& title, 
               unsigned int w, unsigned int h, 
               FbCallBack fb, bool set) 
    {
        window = glfwCreateWindow(w,h, title.c_str(), NULL, NULL);
        this->w = w;
        this->h = h;
        this->title = title;

        if (!fb)
            fb_size_callback = &framebuffer_size_callback;

        if (set)
            set_context(this);
        DU_ASSERT(!window, "Failed to create GLFW window \"{}\"", title);
    }

    Window::~Window() {
    }

    glm::vec2 Window::size() {
        glfwGetWindowSize(window, &w, &h);
        glm::vec2 tmp = {(float)w, (float)h};
        return tmp;
    }

    void Window::maximize() {
        glfwMaximizeWindow(window);
    }

    void init_glfw() {

        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    }

    void set_context(Window *window) {
        glfwMakeContextCurrent(*window);
        DU_ASSERT(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress), "Failed to initialize GLAD");
    }
}
