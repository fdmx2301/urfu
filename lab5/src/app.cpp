#include "app.h"

void App::error_callback(int error, const char* description){
	fputs(description, stderr);
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == '1') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	}

	if (key == '2') {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	if (key == '3') getInstance().mixFactor += 0.05;
	if (key == '4') getInstance().mixFactor -= 0.05;

	if (getInstance().mixFactor < 0) getInstance().mixFactor = 0.0;
	if (getInstance().mixFactor > 1) getInstance().mixFactor = 1.0;
}



void App::mouse_callback(GLFWwindow* window, int a, int b, int c){
    cout << "button a = " << a  << endl;
    cout << "button b = " << b  << endl;
    cout << "button c = " << c  << endl;
}

// менял под линукс спецификацию, строим динамический путь через readlink и proc/self/exe
GLuint App::loadBmp(string filename) {
    char exePath[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len == -1) {
        cout << "error retrieving executable path" << endl;
        return 0;
    }
    exePath[len] = '\0';
    string exeDir = string(exePath);
    string::size_type pos = exeDir.find_last_of("/");
    string fullPath = exeDir.substr(0, pos) + "/data/" + filename;

    // на линуксе проще читать бинарные файлы так
    ifstream file(fullPath, ios::binary);
    if (!file) {
        cout << "error loading BMP file: " << fullPath << endl;
        return 0;
    }

    unsigned char header[54];
    file.read(reinterpret_cast<char*>(header), 54);

    if (header[0] != 'B' || header[1] != 'M') {
        cout << "not a valid BMP file" << endl;
        return 0;
    }

    unsigned int dataPos = *(int*)&(header[0x0A]);
    unsigned int imageSize = *(int*)&(header[0x22]);
    unsigned int width = *(int*)&(header[0x12]);
    unsigned int height = *(int*)&(header[0x16]);

    if (imageSize == 0) imageSize = width * height * 3;
    if (dataPos == 0) dataPos = 54;

    unsigned char* data = new unsigned char[imageSize];
    file.read(reinterpret_cast<char*>(data), imageSize);
    file.close();

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    delete[] data;
    return textureID;
}

/* У меня просто черный квадрат, мб дело в том что линукс этот формат сжатия не поддерживает хотя сомневаюсь:

pael@HP-Laptop-15s-eq2xxx ~> glxinfo | grep -i s3tc
GL_EXT_texture_compression_rgtc, GL_EXT_texture_compression_s3tc, 
GL_NV_texture_barrier, GL_NV_vdpau_interop, GL_OES_EGL_image, GL_S3_s3tc
GL_EXT_texture_compression_rgtc, GL_EXT_texture_compression_s3tc, 
GL_OES_EGL_image, GL_OES_read_format, GL_S3_s3tc, 
GL_EXT_texture_compression_s3tc, GL_EXT_texture_compression_s3tc_srgb, 

скорее всего просто битый dds/поврежденный, лень проверять потому что iron.dds работает и следовательно я уверен в своей теории
*/
GLuint App::loadDDS(string filename) {
    // Получаем путь к исполнимому файлу
    char exePath[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
    if (len == -1) {
        cout << "error retrieving executable path" << endl;
        return 0;
    }
    exePath[len] = '\0';

	// пример как можно еще и через файл сус сделать аналогичное верхнему
    filesystem::path exeDir(exePath);
    filesystem::path fullPath = exeDir.parent_path() / "data" / filename;

    FILE* file = fopen(fullPath.c_str(), "rb");
    if (!file) {
        cout << "error loading DDS file: " << fullPath << endl;
        return 0;
    }

    char fileCode[4];
    fread(fileCode, 1, 4, file);
    if (strncmp(fileCode, "DDS ", 4) != 0) {
        fclose(file);
        cout<< "not a valid DDS file" << endl;
        return 0;
    }

    unsigned char header[124];
    fread(&header, 124, 1, file);
    unsigned int height = *(unsigned int*)&(header[8]);
    unsigned int width = *(unsigned int*)&(header[12]);
    unsigned int linearSize = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC = *(unsigned int*)&(header[80]);

    unsigned char* bufferData;
    unsigned int bufsize;
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    bufferData = (unsigned char*)malloc(bufsize * sizeof(unsigned char));

    fread(bufferData, 1, bufsize, file);
    fclose(file);

    unsigned int components = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;

    switch(fourCC) {
		case FOURCC_DXT1:
			format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
			cout << "GL_COMPRESSED_RGBA_S3TC_DXT1_EXT" << endl;
			break;
		case FOURCC_DXT3:
			format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
			cout << "GL_COMPRESSED_RGBA_S3TC_DXT3_EXT" << endl;
			break;
		case FOURCC_DXT5:
			format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			cout << "GL_COMPRESSED_RGBA_S3TC_DXT5_EXT" << endl;
			break;
		default:
			free(bufferData);
			cout << "Unsupported compression format" << endl;
			return 0;
	}

    GLuint textureID;
    glGenTextures(1, &textureID);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level) {
        unsigned int size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height, 0, size, bufferData + offset);
        offset += size;
        width /= 2;
        height /= 2;
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    delete(bufferData);

    return textureID;
}

void App::setupScene(){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	static const GLfloat gVertexBufferData[] = {
		/// 1 Грань:
		-1.0f,-1.0f,-1.0f, 
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f, 

		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,

		/// 2 Грань:
		1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f, 
		1.0f, 1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 3 Грань:
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,

		/// 4 Грань:
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,	   
		1.0f, 1.0f, 1.0f,
		1.0f,-1.0f,-1.0f,
		1.0f, 1.0f,-1.0f,

		/// 5 Грань:
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f,-1.0f, 1.0f,

		/// 6 Грань:
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,	 
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gVertexBufferData), gVertexBufferData, GL_STATIC_DRAW);

	texture1 = loadDDS("container.DDS");
	texture1 = loadDDS("iron.DDS");

	static const GLfloat gUVBufferData[] = {
		// Первая грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f,  

		// Вторая грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f,  

		// Третья грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f,  

		// Четвертая грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f,  

		// Пятая грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f,  

		// Шестая грань
		1.0f, 0.0f,  
		0.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 0.0f,  
		0.0f, 1.0f,  
		1.0f, 1.0f   
	};

	glGenBuffers(1, &uvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(gUVBufferData), gUVBufferData, GL_STATIC_DRAW);
}

void App::setup(int w, int h, string captionWindow, bool useAntiAliasing){
	glfwSetErrorCallback(error_callback);

	if (!glfwInit()) std::exit(EXIT_FAILURE);
	if (useAntiAliasing) glfwWindowHint(GLFW_SAMPLES,4);

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
	shader.setup("simpleShader.vert","simpleShader.frag");
	getInstance().modelPos.x    = 0;
	getInstance().modelPos.y    = 0;
	getInstance().modelPos.z    = 0;				
	getInstance().modelScale.x  = 1;
	getInstance().modelScale.y  = 1;
	getInstance().modelScale.z  = 1;
	getInstance().angles		= vec3(0,0,0);

	camera.setup(getWindow(),vec3(0,0,5),3.14,0.0f,45.f,3,0.01);
}


void App::update(){
	camera.update();
}


void App::draw(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f,0.0f,0.5f,0.0);

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
	glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
	glVertexAttribPointer(
		1,
		2, 
		GL_FLOAT, 
		GL_FALSE,
		0, 
		(void*)0 
	);


	shader.setUniformMatrix4fv("MVP",&camera.getCameraMatrix()[0][0]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glUniform1i(glGetUniformLocation(shader.getProgramID(), "shaderTexture2"), 1);

	glUniform1f(glGetUniformLocation(shader.getProgramID(), "mixFactor"), getInstance().mixFactor);

	shader.use();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glDrawArrays(GL_TRIANGLES, 0, 12*3); 
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void App::exit(){
    glfwDestroyWindow(window);
    glfwTerminate();
    std::exit(EXIT_SUCCESS);
}

