#include "app.h"

void App::error_callback(int error, const char* description) {
    fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    float moveStep = 0.05f;

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

    if (key == '1') getInstance().modelPos.x += moveStep;
    if (key == '2') getInstance().modelPos.x -= moveStep;
    if (key == '3') getInstance().modelPos.y += moveStep;
    if (key == '4') getInstance().modelPos.y -= moveStep;
    if (key == '5') getInstance().modelPos.z += moveStep;
    if (key == '6') getInstance().modelPos.z -= moveStep;
}

void App::mouse_callback(GLFWwindow* window, int a, int b, int c) {
    cout << "button a = " << a << endl;
    cout << "button b = " << b << endl;
    cout << "button c = " << c << endl;
}

void App::setupScene() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    static const GLfloat gVertexBufferData[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);
}

void App::setup(int w, int h, string captionWindow, bool useAntiAliasing) {
    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) std::exit(EXIT_FAILURE);
    if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(w, h, captionWindow.c_str(), NULL, NULL);
    if (!window){
        glfwTerminate();
        std::exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

    glewInit();
    setupScene();
    shader.setup("simpleShader.vert", "simpleShader.frag");

    getInstance().modelPos.x = 0;
    getInstance().modelPos.y = 0;
    getInstance().modelPos.z = 0;
    getInstance().modelScale.x = 1;
    getInstance().modelScale.y = 1;
    getInstance().modelScale.z = 1;
    getInstance().angle = 0;
    getInstance().axis = vec3(0, 0, 1);
}

void App::update() {
    float fovRad = 45.0f * (3.14f / 180.0f);
    mat4 projection = perspective(fovRad, 4.0f / 3.0f, 0.1f, 100.0f);

    mat4 view = lookAt(
        vec3(0.0, 0, 10), 
        vec3(0.0f, 0.0f, 0.0f), 
        vec3(0, 1, 0)
    );

    mat4 model = mat4(1.0f);
    model = scale(model, getInstance().modelScale);
    model = glm::rotate(model, getInstance().angle, getInstance().axis);
    model = translate(model, getInstance().modelPos);

    globalMVP = projection * view * model;
}

void App::draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.5f, 0.0f);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    shader.setUniformMatrix4fv("MVP", &globalMVP[0][0]);
    shader.use();

    glDrawArrays(GL_TRIANGLES, 0, 3);
    glDisableVertexAttribArray(0);
}

void App::exit() {
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}