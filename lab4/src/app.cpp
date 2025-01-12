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

    // Поворот по 3 осям
    if (mods == 0 && (key == 'x' || key == 'X')) getInstance().angles.x += moveStep;
    if (mods == 0 && (key == 'y' || key == 'Y')) getInstance().angles.y += moveStep;
    if (mods == 0 && (key == 'z' || key == 'Z')) getInstance().angles.z += moveStep;

    if (mods == 1 && (key == 'x' || key == 'X')) getInstance().angles.x -= moveStep;
    if (mods == 1 && (key == 'y' || key == 'Y')) getInstance().angles.y -= moveStep;
    if (mods == 1 && (key == 'z' || key == 'Z')) getInstance().angles.z -= moveStep;
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
        -1.0f,-1.0f,-1.0f, // начало 1 треугольника
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, // конец первого треугольника
        1.0f, 1.0f,-1.0f, // начало второго треугольника
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f, // конец второго треугольника
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

    static const GLfloat gСolorBufferData[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gСolorBufferData), gСolorBufferData, GL_STATIC_DRAW);
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

    getInstance().modelPos.x            = 0;
    getInstance().modelPos.y            = 0;
    getInstance().modelPos.z            = 0;
    getInstance().modelScale.x          = 1;
    getInstance().modelScale.y          = 1;
    getInstance().modelScale.z          = 1;
    getInstance().angles                = vec3(0, 0, 0);
    getInstance().axis                  = vec3(0, 0, 1);

    camera.setup(
        getWindow(), 
        vec3(0,0,5), 
        3.14, 
        0.0f, 
        45.f, 
        3, 
        0.01
    );
}

void App::update() {
    camera.update();

    float fovRad = 45.0f * (3.14f / 180.0f);
    mat4 projection = perspective(fovRad, 4.0f / 3.0f, 0.1f, 100.0f);

    mat4 view = lookAt(
        vec3(0.0, 0, 10), 
        vec3(0.0f, 0.0f, 0.0f), 
        vec3(0, 1, 0)
    );

    mat4 model = mat4(1.0f);
    model = scale(model, getInstance().modelScale);

    model = glm::rotate(model, getInstance().angles.x, vec3(1, 0, 0));
    model = glm::rotate(model, getInstance().angles.y, vec3(0, 1, 0));
    model = glm::rotate(model, getInstance().angles.z, vec3(0, 0, 1));

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

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        (void*)0
    );

    shader.setUniformMatrix4fv("MVP",&camera.getCameraMatrix()[0][0]);
    shader.use();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glDrawArrays(GL_TRIANGLES, 0, 12*3);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void App::exit() {
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}