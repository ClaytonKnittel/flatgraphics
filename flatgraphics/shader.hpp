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

using std::string;

class shader {
private:
    static const string BASE_PATH;
    static const string DEFAULT_VS;
    static const string DEFAULT_FS;
public:
    GLuint Program;
    shader(const std::string vertexPath=DEFAULT_VS, const std::string fragmentPath=DEFAULT_FS);
    shader(const GLchar *vertexPath, const GLchar *fragmentPath);
    void use();
};

#endif /* shader_hpp */
