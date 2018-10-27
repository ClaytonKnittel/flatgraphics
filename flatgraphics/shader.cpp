//
//  shader.cpp
//  flatgraphics
//
//  Created by Clayton Knittel on 10/21/18.
//  Copyright © 2018 Clayton Knittel. All rights reserved.
//

#include "shader.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using std::ifstream;
using std::stringstream;
using std::cout;
using std::endl;

const string shader::BASE_PATH = "/users/claytonknittel/documents/xcode/flatgraphics/flatgraphics/";
const string shader::DEFAULT_VS = BASE_PATH + "resources/shaders/core.vs";
const string shader::DEFAULT_FS = BASE_PATH + "resources/shaders/core.frag";

shader::shader(const std::string vertexPath, const std::string fragmentPath): shader(vertexPath.c_str(), fragmentPath.c_str()) {}

shader::shader(const GLchar *vertexPath, const GLchar *fragmentPath) {
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;
    
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    
    if (!vShaderFile.is_open()) {
        cout << "unable to open vertex shader file" << endl;
    }
    if (!fShaderFile.is_open()) {
        cout << "unable to open fragment shader file" << endl;
    }
    
    stringstream vShaderStream, fShaderStream;
    // read file's buffer contents into streams
    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();
    // close file handlers
    vShaderFile.close();
    fShaderFile.close();
    // Convert stream into string
    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
    
    const GLchar *vShaderCode = vertexCode.c_str();
    const GLchar *fShaderCode = fragmentCode.c_str();
    // compile shaders
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[512];
    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print any compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
    }
    
    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print any compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
    }
    
    // Shader program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // print any linking errors
    glGetProgramiv(this->Program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->Program, 512, NULL, infoLog);
        cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << endl;
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void shader::use() {
    glUseProgram(this->Program);
}
