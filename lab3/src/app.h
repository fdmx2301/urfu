#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "GlslShader.h"

using namespace std;
using namespace glm;

vec3 modelPos;
vec3 modelScale;
float angle;
vec3 axis;
const float stepMoving = 0.05;