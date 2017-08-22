#pragma once

// ウィンドウ関連の処理
class Window
{
    GLFWwindow *const window;
    GLfloat size[2];
    GLfloat scale;
    GLfloat scaleArray[2];
    GLfloat location[2];
    
    void updateScale()
    {
        scaleArray[0] = scale * 2.0f / static_cast<GLfloat>(size[0]);
        scaleArray[1] = scale * 2.0f / static_cast<GLfloat>(size[1]);
    }
    
    int keyStatus;
    
public:
    Window(int width = 640, int height = 480, const char *title = "Hello!")
    : window(glfwCreateWindow(width, height, title, NULL, NULL))
    , scale(100.0f)
    , keyStatus(GLFW_RELEASE)
    {
        if (window == NULL)
        {
            std::cerr << "Can't create GLFW window." << std::endl;
            exit(1);
        }
        
        glfwMakeContextCurrent(window);
        
        glewExperimental = GL_TRUE;
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Can't initialize GLEW" << std::endl;
            exit(1);
        }
        
        glfwSwapInterval(1);
        
        glfwSetKeyCallback(window, keyboard);
        
        glfwSetWindowUserPointer(window, this);
        
        glfwSetWindowSizeCallback(window, resize);
        
        glfwSetScrollCallback(window, wheel);
        
        resize(window, width, height);
        
        location[0] = location[1] = 0.0f;
    }
    
    virtual ~Window()
    {
        glfwDestroyWindow(window);
    }
    
    int shouldClose() const
    {
        return glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE);
    }
    
    void swapBuffers()
    {
        glfwSwapBuffers(window);
        if (keyStatus == GLFW_RELEASE)
        {
            glfwWaitEvents();
        }
        else
        {
            glfwPollEvents();
        }
        
        if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
        {
            location[0] -= scaleArray[0] / scale;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
        {
            location[0] += scaleArray[0] / scale;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
        {
            location[1] -= scaleArray[1] / scale;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
        {
            location[1] += scaleArray[1] / scale;
        }
        
        // マウスカーソルの位置を取得
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }
    }
    
    const GLfloat *getSize() const { return size; }
    GLfloat getScale() const { return scale; }
    const GLfloat *getLocation() const { return location; }
    
    static void resize(GLFWwindow *const window, int width, int height)
    {
        glViewport(0, 0, width, height);
        
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        
        if (instance != NULL)
        {
            instance->size[0] = width;
            instance->size[1] = height;
            
            instance->updateScale();
        }
    }
    
    static void wheel(GLFWwindow *window, double x, double y)
    {
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->scale += static_cast<GLfloat>(y);
        }
    }
    
    static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->keyStatus = action;
        }
    }
};
