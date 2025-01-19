#pragma once

#include <glew.h>
#define GLFW_INCLUDE_GLU
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdio.h>

#include "GlslShader.h"

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <unistd.h>
#include <string>

using namespace std;
using namespace glm;

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

class loaderImage {
    public:
        unsigned char* loadPixelsFromDDSFile(string fileName, unsigned int& width, unsigned int& height, unsigned int& format, unsigned int& size) {
            char exePath[1024];
            ssize_t len = readlink("/proc/self/exe", exePath, sizeof(exePath) - 1);
            if (len == -1) {
                cerr << "Failed to get executable path!" << endl;
                return nullptr;
            }
            exePath[len] = '\0';

            string exePathStr(exePath);
            size_t pos = exePathStr.find_last_of("/");
            string exeDir = exePathStr.substr(0, pos);
            string fullPath = exeDir + "/data/" + fileName;

            FILE* file = fopen(fullPath.c_str(), "rb");
            if (!file) {
                cerr << "Error: Could not open image file " << fullPath << endl;
                return nullptr;
            }

            char filecode[4];
            fread(filecode, 1, 4, file);
            if (std::string(filecode, 4) != "DDS ") {
                fclose(file);
                cerr << "Error: Invalid file extension or format!" << endl;
                return nullptr;
            }

            unsigned char header[124];
            fread(&header, 124, 1, file);
            height = *(unsigned int*)&(header[8]);
            width = *(unsigned int*)&(header[12]);
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

            switch (fourCC) {
                case FOURCC_DXT1:
                    format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
                    break;
                case FOURCC_DXT3:
                    format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
                    break;
                case FOURCC_DXT5:
                    format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
                    break;
                default:
                    free(bufferData);
                    cerr << "Error: Unsupported DDS format!" << endl;
                    return nullptr;
            }

            unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
            size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

            return bufferData;
        }
};