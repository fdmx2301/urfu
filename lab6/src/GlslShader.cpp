#include "GlslShader.h"

#include <unistd.h>
#include <limits.h>
#include <sstream> // Для вывода GLSL кода
#define PATH_SEPARATOR "/"

// Функция проверки компиляции шейдера
void checkShaderCompilation(GLuint shaderID, const string& shaderType) {
    GLint success = GL_FALSE;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

    GLint logLength = 0;
    glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {
        vector<char> errorMessage(logLength);
        glGetShaderInfoLog(shaderID, logLength, NULL, &errorMessage[0]);
        cerr << shaderType << " Shader Compilation Error: " << &errorMessage[0] << endl;
    } else if (!success) {
        cerr << shaderType << " Shader Compilation Failed with no detailed log." << endl;
    } else {
        cout << shaderType << " Shader compiled successfully." << endl;
    }
}

// Функция проверки линковки программы
void checkProgramLinking(GLuint programID) {
    GLint success = GL_FALSE;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);

    GLint logLength = 0;
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);

    if (logLength > 0) {
        vector<char> errorMessage(logLength);
        glGetProgramInfoLog(programID, logLength, NULL, &errorMessage[0]);
        cerr << "Program Linking Error: " << &errorMessage[0] << endl;
    } else if (!success) {
        cerr << "Program Linking Failed with no detailed log." << endl;
    } else {
        cout << "Program linked successfully." << endl;
    }
}

/// Основная функция загрузки и компиляции шейдеров
void GlslShader::setup(string vertexShaderPath, string fragmentShaderPath) {
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    string vertexShaderCode;
    string fragmentShaderCode;

    // Проверка пути к вершинному шейдеру
    vertexShaderPath = std::string("Release") + PATH_SEPARATOR + vertexShaderPath;
    ifstream vertexShaderStream(vertexShaderPath.c_str());
    if (vertexShaderStream.is_open()) {
        stringstream buffer;
        buffer << vertexShaderStream.rdbuf();
        vertexShaderCode = buffer.str();
        vertexShaderStream.close();
        cout << "Vertex Shader Code loaded."<< endl;
    } else {
        cerr << "Error: Vertex shader file not found at " << vertexShaderPath << endl;
        return;
    }

    // Проверка пути к фрагментному шейдеру
    fragmentShaderPath = std::string("Release") + PATH_SEPARATOR + fragmentShaderPath;
    ifstream fragmentShaderStream(fragmentShaderPath.c_str());
    if (fragmentShaderStream.is_open()) {
        stringstream buffer;
        buffer << fragmentShaderStream.rdbuf();
        fragmentShaderCode = buffer.str();
        fragmentShaderStream.close();
        cout << "Fragment Shader Code loaded." << endl;
    } else {
        cerr << "Error: Fragment shader file not found at " << fragmentShaderPath << endl;
        return;
    }

    // Компиляция вершинного шейдера
    cout << "Compiling shader: " << vertexShaderPath << endl;
    const char* vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer, NULL);
    glCompileShader(vertexShaderID);
    checkShaderCompilation(vertexShaderID, "Vertex");

    // Компиляция фрагментного шейдера
    cout << "Compiling shader: " << fragmentShaderPath << endl;
    const char* fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer, NULL);
    glCompileShader(fragmentShaderID);
    checkShaderCompilation(fragmentShaderID, "Fragment");

    // Линковка программы
    cout << "Linking program..." << endl;
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    checkProgramLinking(programID);

    // Удаление шейдеров
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void GlslShader::use() {
    glUseProgram(programID);
}
