#include "GlslShader.h"

#include <unistd.h>
#include <limits.h>
#define PATH_SEPARATOR "/"

/// Основная функция загрузки и компиляции вершинного и фрагментного шейдера и линковки в одну шейдерную программу GL:
void GlslShader::setup(string vertexShaderPath, string fragmentShaderPath){
    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    string vertexShaderCode;
    // Получаем полный путь к шейдеру, добавляя подкаталог "shaders" к exePath()
    vertexShaderPath = std::string("Release") + PATH_SEPARATOR + vertexShaderPath;

    ifstream vertexShaderStream(vertexShaderPath.c_str(), std::ios::in);
    if(vertexShaderStream.is_open()){
        string Line = "";
        while(getline(vertexShaderStream, Line))
            vertexShaderCode += "\n" + Line;
        vertexShaderStream.close();
    }

    string fragmentShaderCode;
    // Аналогично для фрагментного шейдера
    fragmentShaderPath = std::string("Release") + PATH_SEPARATOR + fragmentShaderPath;
    ifstream fragmentShaderStream(fragmentShaderPath.c_str(), std::ios::in);
    if(fragmentShaderStream.is_open()){
        string Line = "";
        while(getline(fragmentShaderStream, Line))
            fragmentShaderCode += "\n" + Line;
        fragmentShaderStream.close();
    }

    GLint result = GL_FALSE;
    int infoLogLength;

    // Компилируем вершинный шейдер
    cout << "Compiling shader : " << vertexShaderPath << endl;
    char const * vertexSourcePointer = vertexShaderCode.c_str();
    glShaderSource(vertexShaderID, 1, &vertexSourcePointer , NULL);
    glCompileShader(vertexShaderID);

    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(vertexShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    vector<char> vertexShaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(vertexShaderID, infoLogLength, NULL, &vertexShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &vertexShaderErrorMessage[0]);

    // Компилируем фрагментный шейдер
    printf("Compiling shader : %s\n", fragmentShaderPath.c_str());
    char const * fragmentSourcePointer = fragmentShaderCode.c_str();
    glShaderSource(fragmentShaderID, 1, &fragmentSourcePointer , NULL);
    glCompileShader(fragmentShaderID);

    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(fragmentShaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
    vector<char> fragmentShaderErrorMessage(infoLogLength);
    glGetShaderInfoLog(fragmentShaderID, infoLogLength, NULL, &fragmentShaderErrorMessage[0]);
    fprintf(stdout, "%s\n", &fragmentShaderErrorMessage[0]);

    // Создаем шейдерную программу
    fprintf(stdout, "Linking program\n");
    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);    
    glLinkProgram(programID);

    glGetProgramiv(programID, GL_LINK_STATUS, &result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
    vector<char> programErrorMessage(max(infoLogLength, int(1)));
    glGetProgramInfoLog(programID, infoLogLength, NULL, &programErrorMessage[0]);
    fprintf(stdout, "%s\n", &programErrorMessage[0]);

    // Удаляем локальные шейдеры
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void GlslShader::use() {
    glUseProgram(programID);
}
