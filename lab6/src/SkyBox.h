#pragma once

#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

#include "GlslShader.h"

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <unistd.h>
#include <string>

using namespace std;
using namespace glm;

#include "GlslShader.h"
#include "loadTexture.h"

class SkyBox {
    public:
        void setup(string folder, float sizeSkyBox);
        void setupTexture(string folder);
        void setupGeometry();

        void update(mat4 MVP, vec3 pos);
        void draw();

    private:
        GLuint textureID;
        GlslShader shader;
        mat4 MVP;
        mat4 modelMatrix;
        GLuint skyboxVAO, skyboxVBO;

        float sizeSkyBox;   
};