#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;

uniform mat4 MVP;  // Мы добавляем uniform переменную для матрицы MV

void main() {
    vec4 v = vec4(vertexPosition_modelspace, 1)
    gl_Position = MVP * v
}
