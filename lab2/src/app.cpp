/// Подключаем файл app.h чтобы app.cpp видел небходимые внешние файлы библиотеки GLFW:
#include "app.h"

/// Данные функции вызываются когда происходят события нажатия на клавиатуру, события с мышью и возмыжные неполадки:
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void mouse_callback(GLFWwindow* window, int a, int b, int c){
    cout << "button a = " << a  << endl;
    cout << "button b = " << b  << endl;
    cout << "button c = " << c  << endl;
}

/// Это основная функция main() которая является точкой входа в наше приложения, откуда мы уже запускаем все другие подсистемы нашего 
/// приложения:
int main(){
	/// Указатель на используемое окно:
	GLFWwindow* window;
	/// Подключаем функцию error_callback для отслеживания возможных ошибок связанных с GL или
	/// конкретно библиотекой GLFW
	glfwSetErrorCallback(error_callback);
	/// Выполняем инициализацию библиотеки GLFW:
	if (!glfwInit()) 
		exit(EXIT_FAILURE);
	/// Включаем 4 кратное сглаживания для устранения эффекта зазубренности изображения:
	glfwWindowHint(GLFW_SAMPLES,4);

	/// Включаем продвинутую свежею версию OpenGL 3.3:
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/// Создаем GLFW окно с нужным размером и заголовком, записываем указатель на область памяти где 
	/// где находится данное окно:
    window = glfwCreateWindow(1024, 768, "Simple example", NULL, NULL);
	
	/// Если окно не создано, то есть указатель содержит NULL - аварийно завершаем наше приложение:
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	/// Подключаем выполняет контекст OpenGL для окна window:
    glfwMakeContextCurrent(window);
	/// Задаем количество экранных  обновлений прежде:
    glfwSwapInterval(1);
	/// Подключаем отслеживания событий мыши и клавиатуры:
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);
	
	// Инициализация GLEW
	glewInit ();
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/// Массив из 3 векторов которые будут представлять 3 вершины
	static const GLfloat gVertexBufferData[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f,  1.0f, 0.0f,
	}; 

	/// Идентификатор вершинного буфера:
	GLuint vertexBuffer;
	/// Сначала генерируем OpenGL буфер и сохраняем указатель на него в vertexBuffer:
	glGenBuffers(1, &vertexBuffer);
	/// Выбираем текущим vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	/// Предоставляем наши вершины в OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// Загружаем шейдер:
	GlslShader shader;
	shader.setup("simpleShader.vert","simpleShader.frag");

	/// Запускаем цикл обновления картинки в окне:
	/// Выполняем цикл до тех пор пока окно не закрывается:
    while (!glfwWindowShouldClose(window))
    {

		/// Выполняем очистки буфера цвета:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f,0.0f,0.5f,0.0);

		/// Начинаем использовать наш шейдер:
		shader.use();

		/// Первый буфер атрибутов: вершины
		glEnableVertexAttribArray(0);
		/// Выбираем текущий вершинный буфер:
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glVertexAttribPointer(
			0,                  // Атрибут 0. Сакрального смысла в нуле нет, но число должно совпадать с числом в шейдере
			3,                  // количество
			GL_FLOAT,           // тип
			GL_FALSE,           // нормализировано ли?
			0,                  // шаг
			(void*)0            // смещение в буфере
		);

		/// Рисуем треугольник !
		glDrawArrays(GL_TRIANGLES, 0, 3); //Начиная с вершины 0 и рисуем 3 штуки. Всего => 1 треугольник 
		glDisableVertexAttribArray(0);
	
		/// Выполняем свап буфера:
        glfwSwapBuffers(window);
		/// И обновления событий окна( в том числе клавиатуры и мыши):
        glfwPollEvents();
    }

	/// Если цикл не выполняется, значит окно закрывается и нужно его удалить из памяти:
    glfwDestroyWindow(window);
	/// Также нужно завершить работу с glfw, высвободить все ресурсы связанные с OpenGL, окном и прочее:
    glfwTerminate();
	/// Выйти из приложении сообщив о нормальном(штатном) завершении работы программы:
    exit(EXIT_SUCCESS);

}