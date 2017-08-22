#pragma once

// ウィンドウ関連の処理
class Window
{
    GLFWwindow *const window;

    // ウィンドウのサイズ
    GLfloat size[2];

    // ワールド座標系に対するデバイス座標系の拡大率
    GLfloat s;

    // ワールド座標系に対する正規化デバイス座標系の拡大率
    GLfloat scale[2];

    // 図形の正規化デバイス座標系の拡大率
    GLfloat location[2];
    
    void updateScale()
    {
        scale[0] = s * 2.0f / static_cast<GLfloat>(size[0]);
        scale[1] = s * 2.0f / static_cast<GLfloat>(size[1]);
    }
    
    int keyStatus;
    
public:
    Window(int width = 640, int height = 480, const char *title = "Hello!")
    : window(glfwCreateWindow(width, height, title, NULL, NULL))
    , s(100.0f)
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
        // キーボード
        glfwSetKeyCallback(window, keyboard);
        // このインスタンスのポインタの記録
        glfwSetWindowUserPointer(window, this);
        // ウインドウのサイズ変更（Retinaディスプレイ用にframebuffersizeを使う）
        glfwSetFramebufferSizeCallback(window, resize);
        // マウスホイール
        glfwSetScrollCallback(window, wheel);
        
        // Retina解像度の都合でframebuffer sizeを取得
        GLint w, h;
        glfwGetFramebufferSize(window, &w, &h);
        resize(window, w, h);

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
            location[0] -= scale[0] / s;
        }
        else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
        {
            location[0] += scale[0] / s;
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
        {
            location[1] -= scale[1] / s;
        }
        else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
        {
            location[1] += scale[1] / s;
        }
        
        // マウスカーソルの位置を取得
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
        {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            
            // [0, 1]を[-1, 1]に変換
            location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
            location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
        }
    }
    
    const GLfloat *getSize() const { return size; }
    GLfloat getScale() const { return s; }
    const GLfloat *getLocation() const { return location; }
    
    static void resize(GLFWwindow *const window, int width, int height)
    {
        glViewport(0, 0, width, height);
        std::cout << "width: " << width << " height: " << height << std::endl;
        
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        
        if (instance != NULL)
        {
            instance->size[0] = static_cast<GLfloat>(width);
            instance->size[1] = static_cast<GLfloat>(height);
            
            // ワールド座標系に対する正規化デバイス座標系の拡大率の更新
            instance->updateScale();
        }
    }
    
    static void wheel(GLFWwindow *window, double x, double y)
    {
        std::cout << "wheel" << std::endl;
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->s += static_cast<GLfloat>(y);
        }
    }
    
    static void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods)
    {
        std::cout << "keyboard" << std::endl;
        Window *const instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
        if (instance != NULL)
        {
            instance->keyStatus = action;
        }
    }
};
