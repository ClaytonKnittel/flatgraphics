//
//  flat.h
//  flatgraphics
//
//  Created by Clayton Knittel on 10/20/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#pragma once

#ifndef flat_h
#define flat_h

#include <iostream>
#include <GLFW/glfw3.h>

using std::cout;
using std::endl;

namespace util {
	
	struct color {
		color() = delete;
		uint8_t r, g, b, a;
		color &operator=(color &c);
		float rf() const;
		float gf() const;
		float bf() const;
		float af() const;
	};
	
	const color black = {0, 0, 0, 0xff};
	const color white = {0xff, 0xff, 0xff, 0xff};
	const color red = {0xff, 0, 0, 0xff};
	const color lime = {0, 0xff, 0, 0xff};
	const color blue = {0, 0, 0xff, 0xff};
	const color yellow = {0xff, 0xff, 0, 0xff};
	const color cyan = {0, 0xff, 0xff, 0xff};
	const color aqua = cyan;
	const color magenta = {0xff, 0, 0xff, 0xff};
	const color fuchsia = magenta;
	const color silver = {0xc0, 0xc0, 0xc0, 0xff};
	
	const color gray = {0x80, 0x80, 0x80, 0xff};
	const color maroon = {0x80 ,0 ,0, 0xff};
	const color olive = {0x80, 0x80, 0, 0xff};
	const color green = {0, 0x80, 0, 0xff};
	const color purple = {0x80, 0,0x80, 0xff};
	const color teal = {0, 0x80, 0x80, 0xff};
	const color navy = {0, 0, 0x80, 0xff};
	
	const color orange = {0xff, 0xa5, 0, 0xff};
	
	const color bar_color = {0xe4, 0xe3, 0xe4, 0xff};
	const color bar_unfocused_color = {0xf6, 0xf6, 0xf6, 0xff};
}

std::ostream &operator<<(std::ostream &o, util::color *c);


namespace input {
	
	class MouseListener {
		virtual void leftPress() = 0;
		virtual void rightPress() = 0;
		virtual void middlePress() = 0;
	};
	
}

#endif /* flat_h */
