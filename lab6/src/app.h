#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "SimpleCamera.h"
#include "SkyBox.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

// линукс специфичные
#include <unistd.h>
#include <limits.h>
#include <fstream>
#include <cstring>  
#include <cstdlib>
#include <filesystem>

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

using namespace std;
using namespace glm;

class App {
    public:
        void setup(int w, int h, string captionWindow, bool useAntiAliasing);
        void draw();
        void update();
        void exit();
        void setupScene();

        static void error_callback(int error, const char* description);
        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void mouse_callback(GLFWwindow* window, int a, int b, int c);

        static App& getInstance() {
            static App app;
            return app;
        }

        vec3 modelPos;
        vec3 modelScale;
        vec3 axis;
        vec3 angles;
        float moveStep;

        GLFWwindow* getWindow() {
            return window;
        }

		GLuint loadBmp(string filename);
        GLuint loadDDS(string filename);

		GLuint texture1;
        GLuint texture2;
        float mixFactor;

        void drawTextureCube();
        void createTextureCube();
    
    private:
        int width, height;
        GLFWwindow* window;
        GlslShader shader;
        SimpleCamera camera;
        mat4 globalMVP;
        mat4 model;
        GLuint vertexBuffer;
        GLuint uvBuffer;
        GLuint normalBuffer;

        SkyBox skyBox;

        GLuint cubeVAO;
};