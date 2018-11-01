//
//  glfwinit.hpp
//  dungeon_mapper
//
//  Created by Clayton Knittel on 9/19/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef glfwinit_hpp
#define glfwinit_hpp

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "flat.h"

// just so you don't have to include in other files
#include "shader.hpp"

#include <map>
#include <iostream>
#include <vector>

using std::vector;
using std::map;

using namespace util;

typedef color(*colorFunc)(int, int);

namespace gl {
	class context;
	class glfw_window;
}

namespace shape {
    
    struct pt {
        float x, y;
        pt operator+(pt p);
        pt operator-(pt p);
        void operator+=(pt p);
        void operator-=(pt p);
    };
	
	struct geom {
		// offset in the vertex buffer at which
		// this geom stores its data
		unsigned int offset;
		
		// how many vertices there are
		const unsigned int size;
		
		// GL_TRIANGLES, GL_LINES, etc.
		const GLuint renderMode;
		virtual void genData(float *ar) = 0;
		float* genData();
		
		geom(GLuint renderMode, unsigned int size);
		virtual ~geom();
	};
    
    class triangle: public geom {
    public:
        triangle(pt p1, pt p2, pt p3, color c);
        triangle(float x1, float y1, float x2, float y2, float x3, float y3, color c);
        triangle(triangle &t);
        virtual ~triangle();
        
        void operator+=(pt p);
        void operator-=(pt p);
		pt &operator[](const int i);
        
        virtual void genData(float *f);
        
        friend std::ostream &operator<<(std::ostream &o, triangle &t);
        
    private:
        pt *pts;
        color c;
    };
    
    class conglomerate: public geom {
    public:
		conglomerate(int size, geom **&geoms);
		conglomerate(vector<geom*> &geoms);
		virtual ~conglomerate();
		
		virtual void genData(float *f);
    private:
        geom **geoms;
		const int geoms_len;
    };
	
	
	class grid: public geom {
	public:
		grid(float x, float y, float width, float height, int rows, int cols, colorFunc colorFunction);
		virtual ~grid();
		
		virtual void genData(float *f);
	private:
		// talking about center of grid
		float x, y;
		float width, height;
		int rows, cols;
		colorFunc colorFunction;
	};
    
}

namespace detail {
	using namespace shape;
	
	class renderable {
	public:
		~renderable();
		
		void setVisible(bool v);
		void draw();
		
		friend class gl::context;
		
		// called if changes were made to vertex data
		void change() const;
		
	protected:
		bool visible, dynamic;
		// call only once at initialization
		void loadData(unsigned int size, float *data);
		
		// call once at initialization to enable
		// element-indexed drawing
		void loadElementBuffer(unsigned int size, int *data);
		
		// call for dynamic objects each frame
		void loadSubData(float *data);
	private:
		renderable(geom *shape, bool dynamic=false, bool visible=true);
		
		geom *shape;
		
		// vertex array object
		// vertex buffer object
		// element buffer object (optional)
		GLuint vao, vbo, ebo;
		
		// flagged if any changes are made to the geoms
		mutable bool change_flag;
		
		// draw_opt: GL_STATIC_DRAW or GL_STREAM_DRAW
		// render_mode: GL_TRIANGLES, GL_LINES, etc.
		const GLuint draw_opt, render_mode;
		
		// number of points in this shape
		unsigned int numVertices;
		// number of elements in element buffer (optional)
		unsigned int numElements;
	};
}



namespace gl {
	using namespace shape;
	using namespace detail;

	class context {
	public:
		~context();
		void drawTri(float x1, float y1, float x2, float y2, float x3, float y3, color c);
		void drawTri(shape::triangle *t);
		
		void genRenderObj(shape::geom *g);
		void genStaticRenderObj(shape::geom *g);
		void deleteRenderObj(shape::geom *g);
		
		void draw(shape::geom *g, bool updated=false);
		
	private:
		glfw_window *w;
		map<shape::geom*, renderable*> geomap;
		
		context(glfw_window *w);
		friend class glfw_window;
	};

	class glfw_window {
	public:
		glfw_window(int width, int height, int v_maj=3, int v_min=3);
		~glfw_window();
		void addKeyPress(int glfwKey, const std::function<void()> &action);
		int window_width();
		int window_height();
		GLFWwindow * glfwWindow();
		void setBGColor(util::color c);
		int setBGColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		
		context getContext();
		
		bool shouldClose();
		void begin_draw();
		void end_draw();
		
		double getTime();
		
		/**
		 x: x position of cursor
		 y: y position of cursor
		 */
		void createCursorPosCallback(void(GLFWwindow* w, double x, double y));
		/**
		 button: GLFW_MOUSE_BUTTON_LEFT, GLFW_MOUSE_BUTTON_RIGHT, etc.
		 action: GLFW_PRESS or GLFW_RELEASE
		 mods:
		 */
		void createMouseButtonCallback(void(GLFWwindow* w, int button, int action, int mods));
		
		void getMousePos(double *x, double *y);
		
		bool createCustomCursor(unsigned char* data, int w=16, int h=16);
		void defaultCursor();
		
	private:
		GLFWwindow *_window;
		std::map<int, std::function<void()>> keymap;
		int width, height;
		util::color bg_color;
	};
	
}

#endif /* glfwinit_hpp */
