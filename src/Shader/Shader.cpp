#pragma once
#include "Shader.h"

namespace ShaderHelpers
{
    GLuint CreateShaderProgram(const char* vertShader, const char* fragShader)
    {
        GLuint handle = glCreateProgram();
        if (handle == 0)
        {
            std::cout << "Unable to create shader program." << std::endl;
            return 0;
        }

        // compile vertex shader
        if (CompileAttachShader(handle, vertShader, GL_VERTEX_SHADER) == 0)
        {
            return 0;
        }

        // compile fragment shader
        if (CompileAttachShader(handle, fragShader, GL_FRAGMENT_SHADER) == 0)
        {
            return 0;
        }

        if (LinkProgram(handle) == 0)
        {
            return 0;
        }

        return handle;
    }

    GLuint CompileAttachShader(GLuint programHandle, const char* filename, GLuint shaderType)
    {
        // load shader file
        std::ifstream inFile(filename, std::ios::in);
        if (!inFile)
        {
            std::cout << "File not found." << std::endl;
            return 0;
        }

        std::ostringstream codeStream;
        while (inFile.good())
        {
            int c = inFile.get();
            if (!inFile.eof())
                codeStream << (char)c;
        }
        inFile.close();

        // Create shader
        GLuint shaderhandle = 0;
        shaderhandle = glCreateShader(shaderType);

        // Set source to shader
        std::string  code = codeStream.str();
        const char* c_code = code.c_str();
        glShaderSource(shaderhandle, 1, &c_code, NULL);

        // Compile the shader
        glCompileShader(shaderhandle);

        // Check for errors
        int result;
        glGetShaderiv(shaderhandle, GL_COMPILE_STATUS, &result);
        if (GL_FALSE == result)
        {
            // Compile failed, store log and return false
            int length = 0;

            glGetShaderiv(shaderhandle, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char* c_log = new char[static_cast<size_t>(length) + 1];
                int    written = 0;
                glGetShaderInfoLog(shaderhandle, length, &written, c_log);
                std::cout << c_log << std::endl;
                delete[] c_log;
            }

            return 0;
        }
        else
        {
            // Compile succeeded, attach shader and return true
            glAttachShader(programHandle, shaderhandle);

            glDeleteShader(shaderhandle);
            return programHandle;
        }
    }

    GLuint LinkProgram(GLuint programHandle)
    {
        if (programHandle <= 0)
            return 0;

        // Link shader program
        glLinkProgram(programHandle);

        // Check for errors
        int status = 0;
        glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
        if (GL_FALSE == status)
        {
            // Store log and return false
            int length = 0;

            glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &length);
            if (length > 0)
            {
                char* c_log = new char[static_cast<size_t>(length) + 1];
                int    written = 0;
                glGetProgramInfoLog(programHandle, length, &written, c_log);
                std::cout << c_log << std::endl;
                delete[] c_log;
            }

            return 0;
        }

        return programHandle;
    }
} // namespace ShaderHelpers