//
//  sandbag.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/24/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include <stdio.h>
#include "sandbag.h"
#include <vector>

using std::vector;
using std::pair;

sandbag::sandbag(unsigned int w, unsigned int h, unsigned int max): w(w), h(h), max(max) {
    g = new unsigned int[w * h];
    for (int i = 0; i < w * h; i++)
        g[i] = 0;
}

sandbag::~sandbag() {
    delete [] g;
}

void sandbag::fill(int i, int j, unsigned int sandAmount) {
    g[i + w * j] += sandAmount;
    spread(i, j);
}

void sandbag::spread(int i, int j) {
    bool *diffuse = new bool[w * h];
    for (int e = 0; e < w * h; e++)
        diffuse[e] = false;
    diffuse[i + j * w] = true;
    vector<pair<int, int>> *wrongs = new vector<pair<int, int>>;
    wrongs->push_back({i, j});
    vector<pair<int, int>> *wrongsb = new vector<pair<int, int>>;
    vector<pair<int, int>> *temp;
    
    while (wrongs->size() > 0) {
        for (auto it = wrongs->begin(); it != wrongs->end(); it++) {
            int x = it->first;
            int y = it->second;
            int tile = x + y * w;
            diffuse[tile] = false;
            int fall = g[tile] / 4;
            g[tile] -= fall * 4;
            if (x > 0) {
                g[tile - 1] += fall;
                if (g[tile - 1] > max) {
                    if (!diffuse[tile - 1]) {
                        wrongsb->push_back({x - 1, y});
                        diffuse[tile - 1] = true;
                    }
                }
            }
            if (x < w - 1) {
                g[tile + 1] += fall;
                if (g[tile + 1] > max) {
                    if (!diffuse[tile + 1]) {
                        wrongsb->push_back({x + 1, y});
                        diffuse[tile + 1] = true;
                    }
                }
            }
            if (y > 0) {
                g[tile - w] += fall;
                if (g[tile - w] > max) {
                    if (!diffuse[tile - w]) {
                        wrongsb->push_back({x, y - 1});
                        diffuse[tile - w] = true;
                    }
                }
            }
            if (y < h - 1) {
                g[tile + w] += fall;
                if (g[tile + w] > max) {
                    if (!diffuse[tile + w]) {
                        wrongsb->push_back({x, y + 1});
                        diffuse[tile + w] = true;
                    }
                }
            }
        }
        wrongs->clear();
        temp = wrongs;
        wrongs = wrongsb;
        wrongsb = temp;
        temp = NULL;
    }
    delete wrongs;
    delete wrongsb;
}

std::ostream &operator<<(std::ostream &o, sandbag &s) {
    for (int j = 0; j < s.h; j++) {
        for (int i = 0; i < s.w; i++) {
            o << s.g[i + j * s.w] << '\t';
        }
        if (j < s.h - 1)
            o << std::endl;
    }
    return o;
}


