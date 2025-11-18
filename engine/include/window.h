#pragma once
#include <espch.h>


namespace engine {

    using FbCallBack =void (*)(GLFWwindow*, int, int);
    class Window {
    public:
        Window();
        Window(const std::string& title, 
               unsigned int w, unsigned int h, 
               FbCallBack fb = nullptr,bool set = true);

        glm::vec2 size();

        void maximize();

        operator GLFWwindow*() {
            return window;
        }
    private:
        GLFWwindow* window;
        unsigned int w,h;
        std::string title;
        FbCallBack fb_size_callback;
    };



    void init_glfw();
    void set_context(Window* window);


}
