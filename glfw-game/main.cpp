//
//  main.cpp
//  glfw-game
//
//  Created by Yuki Yamada on 2017/08/16.
//  Copyright © 2017年 yukiyama. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <GLFW/glfw3.h>

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
    
    // 背景色を指定する
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // ウィンドウが開いている間繰り返す
    while (glfwWindowShouldClose(window) == GL_FALSE)
    {
        // ウィンドウを消去する
        glClear(GL_COLOR_BUFFER_BIT);
        //
        // ここで描画処理を行う
        //
        // カラーバッファを入れ替える
        glfwSwapBuffers(window);
        // イベントを取り出す
        glfwWaitEvents();
    }
}
