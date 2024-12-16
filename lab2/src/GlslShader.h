#pragma once
#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <vector>

using namespace std;

class GlslShader{
	public:	
        /// Вызвав эту функцию мы начинаем применять данный шейдер в отрисовке:
        void use();
        /// Основная функнция загрузки и компиляции вершинного и фрагментного шейдера и линковки в одну шейдерную программу GL:
        void setup(string vertexShaderPath, string fragmentShaderPath);
        
	private:

        /// Зарегистрированный ID нашей шейдерной программы в GL, или проще говоря идентификатор:
        GLuint programID;
        /// Получить путь к расположению файла нашего приложения вместе с именем exe:
        string exePathWithName();
        /// Тоже самое но уже без имени файла:
        string exePath();	
};