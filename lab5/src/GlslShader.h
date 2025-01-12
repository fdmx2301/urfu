#include <glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
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
            glUniformMatrix4fv(uniformID, 1, GL_FALSE, value);
        }
};