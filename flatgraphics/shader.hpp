//
//  shader.hpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/21/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef shader_hpp
#define shader_hpp

#include <stdio.h>

#include <string>

#include <GL/glew.h>

class shader {
public:
    GLuint Program;
    shader(const std::string vertexPath, const std::string fragmentPath);
    shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    void use();
};

#endif /* shader_hpp */
