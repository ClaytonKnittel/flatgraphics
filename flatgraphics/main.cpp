//
//	main.cpp
//	flatgraphics
//
//	Created by Clayton Knittel on 10/20/18.
//	Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>

#include "gl.hpp"
#include "shader.hpp"
#include "flat.h"

const int WIDTH = 640;
const int HEIGHT = 480;

using std::cout;
using std::endl;

using std::vector;
using std::string;

const std::string BASE_PATH = "/users/claytonknittel/documents/xcode/flatgraphics/flatgraphics/";


int main(int argc, const char * argv[]) {
	gl::glfw_window w(WIDTH, HEIGHT, 3, 3);
	w.setBGColor(util::bar_color);
	
	shader s(BASE_PATH + "resources/shaders/core.vs", BASE_PATH + "resources/shaders/core.frag");
	
	gl::context c = w.getContext();
	
	using shape::triangle;
	
//	vector<shape::geom*> ge;
//	float l = .05f;
//	float st = 1.f;
//	for (float x = -st; x < st; x += l)
//		for (float y = -st; y < st; y += l) {
//			ge.push_back(new triangle(x, y, x + l, y, x + l, y + l, util::cyan));
//			ge.push_back(new triangle(x, y, x, y + l, x + l, y + l, util::green));
//		}
//	conglomerate *gg = new conglomerate(ge);
//	renderable *r = c.genRenderObj(gg);
//	ge.clear();
	renderable *r = c.genStaticRenderObj(new grid(0, 0, 1.8f, 1.8f, 128, 128, [](int i, int j) { return (((i + j) & 1) == 0) ? util::green : util::cyan; }));
	
	double last = glfwGetTime();
	int frames = 0;
	while (!w.shouldClose()) {
		w.begin_draw();
		s.use();
		
		r->draw();
		
		frames++;
		if (glfwGetTime() - last >= 1.0) {
			cout << frames << " fps" << endl;
			last += 1.0;
			frames = 0;
		}
		
		w.end_draw();
	}
	delete r;
	
	return 0;
}
