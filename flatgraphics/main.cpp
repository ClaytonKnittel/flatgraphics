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

#include "sandbag.h"

const int WIDTH = 640;
const int HEIGHT = 640; // 480

using std::cout;
using std::endl;

using std::vector;
using std::string;

const std::string BASE_PATH = "/users/claytonknittel/documents/xcode/flatgraphics/flatgraphics/";


const util::color cols[4] = {
	util::black,
	util::red,
	util::green,
	util::cyan
};

#define WID 401

sandbag sa(WID, WID);

util::color get(int i, int j) {
	return cols[sa.g[i + j * sa.w]];
}

int main(int argc, const char * argv[]) {
	sa.fill(WID / 2, WID / 2, 320000);
	
	gl::glfw_window w(WIDTH, HEIGHT, 3, 3);
	w.setBGColor(util::bar_color);
	
	shader s(BASE_PATH + "resources/shaders/core.vs", BASE_PATH + "resources/shaders/core.frag");
	
	gl::context c = w.getContext();
	
	renderable *r = c.genStaticRenderObj(new grid(0, 0, 2.f, 2.f, WID, WID, [](int i, int j) { return get(i, j); }));
	
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
