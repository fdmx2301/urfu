#pragma once
#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class GlslShader {
    public: 
        void use();
        void setup(string vertexShaderPath, string fragmentShaderPath);

        GLuint programID;

        GLuint& getProgramID() {
            return programID;
        }

        void setUniformMatrix4fv(string name, GLfloat* value) {
            GLuint uniformID = glGetUniformLocation(getProgramID(), name.c_str());
            if (uniformID != -1) {
                glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
            } else {
                cerr << "Uniform " << name << " not found!" << endl;
            }
        }

        void setUniform1f(string name, float value) {
            GLuint uniformID = glGetUniformLocation(getProgramID(), name.c_str());
            if (uniformID != -1) {
                glUniform1f(uniformID, value);
            } else {
                cerr << "Uniform " << name << " not found!" << endl;
            }
        }
};
