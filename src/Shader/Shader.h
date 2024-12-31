#pragma once

#include "../Common.h"

#include <iostream>
#include <fstream>
#include <sstream>

namespace ShaderHelpers
{
    GLuint CreateShaderProgram(const char* vertShader, const char* fragShader);
    GLuint CompileAttachShader(GLuint programHandle, const char* filename, GLuint shaderType);
    GLuint LinkProgram(GLuint programHandle);

} // namespace ShaderHelpers