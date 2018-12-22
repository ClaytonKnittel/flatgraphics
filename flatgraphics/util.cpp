//
//  util.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/21/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>
#include "flat.h"

using util::color;

float color::rf() const {
    return r / 255.f;
}

float color::gf() const {
    return g / 255.f;
}

float color::bf() const {
    return b / 255.f;
}

float color::af() const {
    return a / 255.f;
}

color &color::operator=(util::color &c) {
    r = c.r;
    g = c.g;
    b = c.b;
    a = c.a;
    return *this;
}

std::ostream &operator<<(std::ostream &o, util::color *c) {
    return o << "rgb(" << int(c->r) << ", " << int(c->g) << ", " << int(c->b) << ", " << int(c->a) << ")";
}
