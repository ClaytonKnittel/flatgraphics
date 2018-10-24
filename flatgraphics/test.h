//
//  test.h
//  flatgraphics
//
//  Created by Clayton Knittel on 10/21/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#ifndef test_h
#define test_h

#include "glfwinit.hpp"
#include "shader.hpp"
#include <GL/glew.h>

#include <cmath>


void run() {
    const std::string BASE_PATH = "/users/claytonknittel/documents/xcode/flatgraphics/flatgraphics/";
    
    const int WIDTH = 640, HEIGHT = 480;
    
    glfw_window w(WIDTH, HEIGHT, 3, 3);
    w.setBGColor(util::cyan);
    
    shader s(BASE_PATH + "resources/shaders/core.vs", BASE_PATH + "resources/shaders/core.frag");
    
    float data[24] = {
        -.5f, -.5f, 1, 0, 0, 1,
        .5f, -.5f, 0, 1, 0, 1,
        0.f, 1.f, 0, 0, 1, 1,
        1.f, .9f, 1, 0, 1, 1
    };
    unsigned int indices[6] = {
        0, 1, 2,
        1, 2, 3
    };
    float data2[18] = {
        -.5f, -1.f, 1, 0, 1, 1,
        .5f, -1.f, 1, 1, 0, 1,
        0.f, 0.f, 0, 1, 1, 1
    };
    
    GLuint vao1, vao2, vbo1, vbo2, ebo1;
    
    glGenVertexArrays(1, &vao1);
    
    glBindVertexArray(vao1);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &vbo1);
    glBindBuffer(GL_ARRAY_BUFFER, vbo1);
    
    // stores in the currently bound VAO all of this info about this
    // attribute AND the currently bound VBO, so it knows where it will
    // be getting its data from
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
    
    glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(float), data, GL_STREAM_DRAW);
    
    glGenBuffers(1, &ebo1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(int), indices, GL_STATIC_DRAW);
    
    
    glGenVertexArrays(1, &vao2);
    
    glBindVertexArray(vao2);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
    glGenBuffers(1, &vbo2);
    glBindBuffer(GL_ARRAY_BUFFER, vbo2);
    
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *) (2 * sizeof(GLfloat)));
    
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), data2, GL_STATIC_DRAW);
    
    
    float t = 0;
    while (!w.shouldClose()) {
        w.begin_draw();
        
        s.use();
        glBindVertexArray(vao1);
        
        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        t += .04f;
        data[0] = -.5f + .2f * std::cosf(t);
        data[1] = -.5f + .2f * std::sinf(t);
        glBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(float), data);
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
        
        glBindVertexArray(vao2);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        
        glBindVertexArray(0);
        
        w.end_draw();
    }
    glDeleteVertexArrays(1, &vao1);
    glDeleteVertexArrays(1, &vao2);
    glDeleteBuffers(1, &vbo1);
    glDeleteBuffers(1, &vbo2);
    glDeleteBuffers(1, &ebo1);
}

#endif /* test_h */
