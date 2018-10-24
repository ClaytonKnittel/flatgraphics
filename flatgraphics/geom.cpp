//
//  geom.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/21/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>
#include "flat.h"

std::ostream &operator<<(std::ostream &o, geom::rect r) {
    return o << "Rect at (" << r.x + r.w / 2 << ", " << r.y + r.h / 2 << ") of dim " << r.w << " x " << r.h;
}

std::ostream &operator<<(std::ostream &o, geom::circle c) {
    return o << "Circle at (" << c.x + c.r / 2 << ", " << c.y + c.r / 2 << ") of radius " << c.r;
}

std::ostream &operator<<(std::ostream &o, geom::triangle t) {
    return o << "Triangle ((" << t.x1 << ", " << t.y1 << "), (" << t.x2 << ", " << t.y2 << "), (" << t.x3 << ", " << t.y3 << "))";
}
