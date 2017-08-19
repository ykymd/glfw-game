//
//  main.cpp
//  glfw-game
//
//  Created by Yuki Yamada on 2017/08/16.
//  Copyright © 2017年 yukiyama. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLboolean printShaderInfoLog(GLuint shader, const char *str)
{
    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) std::cerr << "Compile Error in " << str << std::endl;
    
    GLsizei bufSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1)
    {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetShaderInfoLog(shader, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    
    return static_cast<GLboolean>(status);
}

GLboolean printProgramInfoLog(GLuint program)
{
    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

    GLsizei bufSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);
    
    if (bufSize > 1)
    {
        std::vector<GLchar> infoLog(bufSize);
        GLsizei length;
        glGetProgramInfoLog(program, bufSize, &length, &infoLog[0]);
        std::cerr << &infoLog[0] << std::endl;
    }
    
    return static_cast<GLboolean>(status);
}

// vsrc: バーテックスシェーダ
// fsrc: フラグメントシェーダ
GLuint createProgram(const char *vsrc, const char *fsrc)
{
    const GLuint program(glCreateProgram());
    
    if (vsrc != NULL)
    {
        const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));
        glShaderSource(vobj, 1, &vsrc, NULL);
        glCompileShader(vobj);
        if (printShaderInfoLog(vobj, "vertex shader"))
            glAttachShader(program, vobj);
        glDeleteShader(vobj);
    }
    
    if (fsrc != NULL)
    {
        const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
        glShaderSource(fobj, 1, &fsrc, NULL);
        glCompileShader(fobj);
        if (printShaderInfoLog(fobj, "fragment shader"))
            glAttachShader(program, fobj);
        glDeleteShader(fobj);
    }
    
    glBindAttribLocation(program, 0, "position");
    glBindFragDataLocation(program, 0, "fragment");
    glLinkProgram(program);
    
    if (printProgramInfoLog(program))
    {
        return program;
    }
    else
    {
        glDeleteProgram(program);
        return 0;
    }
}

bool readShaderSource(const char *name, std::vector<GLchar> &buffer)
{
    if (name == NULL) return false;

    std::ifstream file(name, std::ios::binary);
    if (file.fail())
    {
        std::cerr << "Error: Can't open source file: " << name << std::endl;
        return false;
    }
    
    file.seekg(0L, std::ios::end);
    GLsizei length = static_cast<GLsizei>(file.tellg());
    buffer.resize(length + 1);
    
    file.seekg(0L, std::ios::beg);
    file.read(buffer.data(), length);
    buffer[length] = '\0';

    if (file.fail())
    {
        std::cerr << "Error: Could not read source file: " << name << std::endl;
        file.close();
        return false;
    }
    
    file.close();
    return true;
}

GLuint loadProgram(const char *vert, const char *frag)
{
    std::vector<GLchar> vsrc;
    const bool vstat(readShaderSource(vert, vsrc));
    std::vector<GLchar> fsrc;
    const bool fstat(readShaderSource(frag, fsrc));
    
    return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

int main(int argc, const char * argv[])
{
    // GLFWの初期化
    if (glfwInit() == GL_FALSE)
    {
        // 初期化に失敗
        std::cerr << "GLFW Initialization failed." << std::endl;
        return 1;
    }
    
    // 終了処理の登録
    atexit(glfwTerminate);
    
    // OpenGL Version 3.2 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // ウィンドウを作成する
    GLFWwindow *const window(glfwCreateWindow(640, 480, "Hello!", NULL, NULL));
    if (window == NULL)
    {
        // ウィンドウが作成できなかった
        std::cerr << "Can't create GLFW window." << std::endl;
        glfwTerminate();
        return 1;
    }
    
    // 作成したウィンドウを OpenGL の処理対象にする
    glfwMakeContextCurrent(window);
    
    // GLEWを初期化する
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        // GLEWの初期化に失敗
        std::cerr << "Can't initialize GLEW" << std::endl;
        return 1;
    }
    
    // 垂直同期のタイミングを待つようにする
    glfwSwapInterval(1);
    
    // 背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    const GLuint program(loadProgram("point.vert", "point.frag"));
    
    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        // シェーダプログラムの使用開始
        glUseProgram(program);
        //
        // ここで描画処理を行う
        //
        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
}
