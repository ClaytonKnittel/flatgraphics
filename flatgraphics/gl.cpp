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
	
	context::~context() {
		for (auto it = geomap.begin(); it != geomap.end(); it++)
			delete it->second;
	}

    void context::drawTri(float x1, float y1, float x2, float y2, float x3, float y3, util::color *c) {
        triangle *t = new triangle(x1, y1, x2, y2, x3, y3, c);
        renderable r = renderable(t, true);
        r.draw();
		delete t;
    }
	
	void context::drawTri(shape::triangle *t) {
		renderable r = renderable(t);
		r.draw();
	}

    void context::genRenderObj(shape::geom *g) {
        geomap[g] = new renderable(g, true);
    }
    
    void context::genStaticRenderObj(shape::geom *g) {
        geomap[g] = new renderable(g);
    }
	
	void context::deleteRenderObj(shape::geom *g) {
		auto it = geomap.find(g);
		if (it != geomap.end()) {
			delete it->second;
			geomap.erase(it);
		}
	}
	
	
	void context::draw(shape::geom *g) {
		auto it = geomap.find(g);
		if (it == geomap.end()) {
			cout << "shape not given rendering context object, must call genRenderObj or variant first" << endl;
			return;
		}
		renderable *r = it->second;
		r->draw();
	}


    // void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    glfw_window::glfw_window(string title, int width, int height, int v_maj, int v_min) {
        glfwInit();
        
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, v_maj);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, v_min);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        this->_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        
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

	GLFWwindow * glfw_window::glfwWindow() {
        return _window;
    }

    void glfw_window::setBGColor(const util::color *c) {
        this->bg_color = c;
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
        glClearColor(bg_color->rf(), bg_color->gf(), bg_color->bf(), bg_color->af());
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void glfw_window::end_draw() {
        glfwSwapBuffers(this->_window);
    }
    
    double glfw_window::getTime() {
        return glfwGetTime();
    }
	
	void glfw_window::createCursorPosCallback(void(f)(GLFWwindow*, double, double)) {
		glfwSetCursorPosCallback(_window, f);
	}
	
	void glfw_window::createMouseButtonCallback(void(f)(GLFWwindow* w, int button, int action, int mods)) {
		glfwSetMouseButtonCallback(_window, f);
	}
	
	void glfw_window::getMousePos(double *x, double *y) {
		glfwGetCursorPos(_window, x, y);
	}
	
	bool glfw_window::createCustomCursor(unsigned char* data, int w, int h) {
		GLFWimage image;
		image.width = w;
		image.height = h;
		image.pixels = data;
		
		GLFWcursor* cursor = glfwCreateCursor(&image, 0, 0);
		if (cursor == NULL) {
			cout << "Could not create custom cursor" << endl;
			return false;
		}
		glfwSetCursor(_window, cursor);
		glfwSetCursor(_window, NULL);
		return true;
	}
	
	void glfw_window::defaultCursor() {
		glfwSetCursor(_window, NULL);
	}
}
