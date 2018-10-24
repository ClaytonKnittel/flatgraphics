//
//  sandbag.h
//  flatgraphics
//
//  Created by Clayton Knittel on 10/24/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef sandbag_h
#define sandbag_h

#include <iostream>

struct sandbag {
    unsigned int *g;
    const unsigned int max;
    unsigned int w, h;
    
    sandbag(unsigned int w, unsigned int h, unsigned int max=3);
    ~sandbag();
    void fill(int i, int j, unsigned int sandAmount);
    
private:
    void spread(int i, int j);
};

std::ostream &operator<<(std::ostream &o, sandbag &s);


#endif /* sandbag_h */
