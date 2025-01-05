/// ���������� ���� app.h ����� app.cpp ����� ���������� ������� ����� ���������� GLFW:
#include "app.h"

/// ������ ������� ���������� ����� ���������� ������� ������� �� ����������, ������� � ����� � ��������� ���������:
void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == '1' && action == GLFW_PRESS) {
        modelPos.z += stepMoving;
        cout << "Model Position Z: " << modelPos.z << endl;
    }
    if (key == '2' && action == GLFW_PRESS) {
        modelPos.z -= stepMoving;
        cout << "Model Position Z: " << modelPos.z << endl;
    }

    if (key == '3' && action == GLFW_PRESS) {
        angle += 0.01f;
        cout << "Rotation Angle: " << angle << endl;
    }
    if (key == '4' && action == GLFW_PRESS) {
        angle -= 0.01f;
        cout << "Rotation Angle: " << angle << endl;
    }

    if (key == 'q' || key == 'Q') {
        modelScale.x += stepMoving * 0.1;
        modelScale.y = modelScale.x;
        modelScale.z = modelScale.x;
        cout << "modelScale[" << modelScale.x << "][" << modelScale.y << "][" << modelScale.z << "]" << endl;
    }

    if (key == 'e' || key == 'E') {
        modelScale.x -= stepMoving * 0.1;
        modelScale.y = modelScale.x;
        modelScale.z = modelScale.x;
        cout << "modelScale[" << modelScale.x << "][" << modelScale.y << "][" << modelScale.z << "]" << endl;
    }
}

void mouse_callback(GLFWwindow* window, int a, int b, int c){
    cout << "button a = " << a  << endl;
    cout << "button b = " << b  << endl;
    cout << "button c = " << c  << endl;
}




/// ��� �������� ������� main() ������� �������� ������ ����� � ���� ����������, ������ �� ��� ��������� ��� ������ ���������� ������ 
/// ����������:
int main(){
	/// ��������� �� ������������ ����:
	GLFWwindow* window;
	/// ���������� ������� error_callback ��� ������������ ��������� ������ ��������� � GL ���
	/// ��������� ����������� GLFW
	glfwSetErrorCallback(error_callback);
	/// ��������� ������������� ���������� GLFW:
	if (!glfwInit()) 
		exit(EXIT_FAILURE);
	/// �������� 4 ������� ����������� ��� ���������� ������� ������������� �����������:
	glfwWindowHint(GLFW_SAMPLES,4);

	/// �������� ����������� ������ ������ OpenGL 3.3:
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	/// ������� GLFW ���� � ������ �������� � ����������, ���������� ��������� �� ������� ������ ��� 
	/// ��� ��������� ������ ����:
    window = glfwCreateWindow(1024, 768, "project_sample", NULL, NULL);

	
	/// ���� ���� �� �������, �� ���� ��������� �������� NULL - �������� ��������� ���� ����������:
    if (!window){
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	/// ���������� ��������� �������� OpenGL ��� ���� window:
    glfwMakeContextCurrent(window);
	/// ������ ���������� ��������  ���������� ������:
    glfwSwapInterval(1);
	/// ���������� ������������ ������� ���� � ����������:
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_callback);

	/// �������������� GLEW:
	glewInit ();

	/// ������� VAO � �������� ��� �������:
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	/// ������ �� 3 �������� ������� ����� ������������ 3 �������:
	static const GLfloat gVertexBufferData[] = {
	   -1.0f, -1.0f, 0.0f,
	   1.0f, -1.0f, 0.0f,
	   0.0f,  1.0f, 0.0f,
	};

	/// ������������� ���������� ������:
	GLuint vertexBuffer;

	/// ������� ���������� OpenGL ����� � ��������� ��������� �� ���� � vertexBuffer:
	glGenBuffers(1, &vertexBuffer);

	/// �������� ������� vertexBuffer:
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	/// ������������� ���� ������� � OpenGL:
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	/// ��������� ������:
	GlslShader shader;
	shader.setup("simpleShader.vert","simpleShader.frag");

	modelPos.x = 0;
	modelPos.y = 0;
	modelPos.z = 0;

	modelScale.x = 1;
	modelScale.y = 1;
	modelScale.z = 1;

	angle = 0;

	axis = vec3(0,0,1);


	/// ��������� ���� ���������� �������� � ����:
	/// ��������� ���� �� ��� ��� ���� ���� �� �����������:
    while (!glfwWindowShouldClose(window))
    {
		/// ��������� ������� ������ �����:
		glClearColor(0.0f,0.0f,0.5f,0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 projection = perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
		mat4 view = lookAt(
			vec3(0,0,2),
			vec3(0.0f,0.0f,0.0f),
			vec3(0,1,0)
		);

		mat4 model = mat4(1.0f);

		model = scale(model, modelScale);
		model = glm::rotate(model, angle, axis);
		model = translate(model, modelPos);

		mat4 MVP = projection * view * model;

		shader.setUniformMatrix4fv("MVP", &MVP[0][0]);
		/// �������� ������������ ��� ������:
		shader.use();

		/// ������ ����� ���������: �������
		glEnableVertexAttribArray(0);
		/// �������� ������� ��������� �����:
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		/// ������������� ���������� ������ ��� ������������:
		glVertexAttribPointer(
			0,                  // ������� 0. ����������� ������ � ���� ���, �� ����� ������ ��������� � ������ � �������
			3,                  // ����������
			GL_FLOAT,           // ���
			GL_FALSE,           // ��������������� ��?
			0,                  // ���
			(void*)0            // �������� � ������
		);

		/// ������ ����������� !
		glDrawArrays(GL_TRIANGLES, 0, 3); //������� � ������� 0 � ������ 3 �����. ����� => 1 �����������
		glDisableVertexAttribArray(0);

		/// ��������� ���� ������:
        glfwSwapBuffers(window);
		/// � ���������� ������� ����( � ��� ����� ���������� � ����):
        glfwPollEvents();
		
    }

	/// ���� ���� �� �����������, ������ ���� ����������� � ����� ��� ������� �� ������:
    glfwDestroyWindow(window);
	/// ����� ����� ��������� ������ � glfw, ����������� ��� ������� ��������� � OpenGL, ����� � ������:
    glfwTerminate();
	/// ����� �� ���������� ������� � ����������(�������) ���������� ������ ���������:
    exit(EXIT_SUCCESS);

}