//
//  glfwinit.cpp
//  dungeon_mapper
//
//  Created by Clayton Knittel on 9/19/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <iostream>

#include "gl.hpp"

namespace gl {
    
    context::context(glfw_window *w) {
        this->w = w;
    }

    void context::drawTri(float x1, float y1, float x2, float y2, float x3, float y3, util::color c) {
        triangle *t = new triangle(x1, y1, x2, y2, x3, y3, c);
        renderable r = renderable(t);
        r.draw();
    }

    renderable* context::genTri(float x1, float y1, float x2, float y2, float x3, float y3, util::color c) {
        triangle *t = new triangle(x1, y1, x2, y2, x3, y3, c);
        return new renderable(t, true);
    }

    renderable* context::genRenderObj(shape::geom *g) {
        return new renderable(g, true);
    }
    
    renderable* context::genStaticRenderObj(shape::geom *g) {
        return new renderable(g);
    }


    // void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    glfw_window::glfw_window(int width, int height, int v_maj, int v_min) {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v_maj);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v_min);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        this->_window = glfwCreateWindow(width, height, "flat", NULL, NULL);
        
        if (_window == nullptr) {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(-1);
        }
        
        int screenWidth, screenHeight;
        glfwGetFramebufferSize(_window, &screenWidth, &screenHeight);
        this->width = screenWidth;
        this->height = screenHeight;
        
        glfwMakeContextCurrent(_window);
        
        glewExperimental = GL_TRUE;
        
        if (GLEW_OK != glewInit()) {
            std::cout << "Failed to initialize GLEW" << std::endl;
        }
        
        glViewport(0, 0, screenWidth, screenHeight);
        
        // enable alpha support
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    glfw_window::~glfw_window() {
        glfwTerminate();
    }

    void glfw_window::addKeyPress(int glfwKey, const std::function<void()> &action) {
        keymap[glfwKey] = action;
    }

    int glfw_window::window_width() {
        return width;
    }
    int glfw_window::window_height() {
        return height;
    }

    GLFWwindow * glfw_window::ref() {
        return _window;
    }

    void glfw_window::setBGColor(util::color c) {
        this->bg_color = c;
    }

    int glfw_window::setBGColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        bg_color.r = r;
        bg_color.g = g;
        bg_color.b = b;
        bg_color.a = a;
        return 0;
    }

    context glfw_window::getContext() {
        context c(this);
        return c;
    }

    bool glfw_window::shouldClose() {
        return glfwWindowShouldClose(this->_window);
    }

    void glfw_window::begin_draw() {
        glfwPollEvents();
        glClearColor(bg_color.r / 255.f, bg_color.g / 255.f, bg_color.b / 255.f, bg_color.a / 255.f);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void glfw_window::end_draw() {
        glfwSwapBuffers(this->_window);
    }
}

