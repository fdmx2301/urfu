#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace std;
using namespace glm;

class SimpleCamera {
    public:
        void setup(GLFWwindow* window, vec3 pos, float hAngle, float vAngle, float fov, float speed, float mouseSpeed);
        void update();

        mat4 getCameraMatrix() {
            return resultMVP;
        }

    private:
        mat4 resultMVP;

        mat4 projectionMatrix;
        mat4 viewMatrix;
        mat4 modelMatrix;

        vec3 pos;

        float hAngle;
        float vAngle;
        float initialFov;
        float speed;
        float mouseSpeed;

        GLFWwindow* window;
        int widthWindow, heightWindow;

        float lastTime;
};