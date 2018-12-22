//
//	main.cpp
//	flatgraphics
//
//	Created by Clayton Knittel on 10/20/18.
//	Copyright © 2018 Clayton Knittel. All rights reserved.
//

#define PI 3.14159265358979323846264

#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <cmath>

#include "gl.hpp"
#include "shader.hpp"
#include "flat.h"

const int WIDTH = 640;
const int HEIGHT = 640; // 480

using std::cout;
using std::endl;

using std::vector;
using std::string;


int main(int argc, const char * argv[]) {
	
	gl::glfw_window w("test", WIDTH, HEIGHT, 3, 3);
	w.setBGColor(&util::bar_color);
	
	
	shader s;
	
	gl::context c = w.getContext();
	
	const color* bg = &util::cyan;
	
	shape::triangle t({0, 0}, {.5f, 0}, {.6f, .4f}, bg);
//	c.genStaticRenderObj(&t);
	shape::dynamicDrawer d(10);
	c.genRenderObj(&d);
	
	//grid g(0, 0, 1, 1, 1, 1, nullptr);
	
	double last = w.getTime();
	int frames = 0;
	while (!w.shouldClose()) {
		w.begin_draw();
		s.use();
		
		d.reset();
//		c.draw(&t);
		
		t += {static_cast<float>(std::cos(frames / 100.f * PI)) * .001f, static_cast<float>(std::sin(frames / 100.f * PI)) * .001f};
		d.draw(&t);
		c.draw(&d);
		
		frames++;
		if (w.getTime() - last >= 1.0) {
			cout << frames << " fps" << endl;
			last += 1.0;
			frames = 0;
		}
		
		w.end_draw();
	}
	
	return 0;
}
