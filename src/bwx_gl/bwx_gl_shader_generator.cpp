/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader_generator.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Shader Generator
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "bwx_gl_shader_generator.h"

#include <sstream>

namespace bwx_sdk {

    std::string bwxGLShaderGenerator::GenerateVertexShader(bool useNormals, bool useTexCoords, bool useLighting) 
    {
        std::ostringstream shader;
        shader << "#version 450 core\n";

        shader << "layout(location = 0) in vec3 aPosition;\n";
        if (useNormals) shader << "layout(location = 1) in vec3 aNormal;\n";
        if (useTexCoords) shader << "layout(location = 2) in vec2 aTexCoord;\n";

        shader << "out vec3 vFragPos;\n";
        if (useNormals) shader << "out vec3 vNormal;\n";
        if (useTexCoords) shader << "out vec2 vTexCoord;\n";

        shader << "uniform mat4 uModel;\n";
        shader << "uniform mat4 uView;\n";
        shader << "uniform mat4 uProjection;\n";

        shader << "void main() {\n";
        shader << "    vFragPos = vec3(uModel * vec4(aPosition, 1.0));\n";
        if (useNormals) shader << "    vNormal = mat3(transpose(inverse(uModel))) * aNormal;\n";
        if (useTexCoords) shader << "    vTexCoord = aTexCoord;\n";
        shader << "    gl_Position = uProjection * uView * vec4(vFragPos, 1.0);\n";
        shader << "}\n";

        return shader.str();
    }

    std::string bwxGLShaderGenerator::GenerateFragmentShader(bool useTextures, bool useLighting) 
    {
        std::ostringstream shader;
        shader << "#version 450 core\n";

        shader << "in vec3 vFragPos;\n";
        shader << "in vec3 vNormal;\n";
        shader << "in vec2 vTexCoord;\n";

        shader << "out vec4 FragColor;\n";

        if (useTextures) {
            shader << "uniform sampler2D uTexture;\n";
        }

        if (useLighting) {
            shader << "struct Light {\n";
            shader << "    vec3 position;\n";
            shader << "    vec3 color;\n";
            shader << "};\n";
            shader << "uniform Light uLight;\n";
            shader << "uniform vec3 uViewPos;\n";
        }

        shader << "void main() {\n";
        shader << "    vec3 color = vec3(1.0);\n";

        if (useTextures) {
            shader << "    color *= texture(uTexture, vTexCoord).rgb;\n";
        }

        if (useLighting) {
            shader << "    vec3 norm = normalize(vNormal);\n";
            shader << "    vec3 lightDir = normalize(uLight.position - vFragPos);\n";
            shader << "    float diff = max(dot(norm, lightDir), 0.0);\n";
            shader << "    vec3 diffuse = diff * uLight.color;\n";

            shader << "    vec3 viewDir = normalize(uViewPos - vFragPos);\n";
            shader << "    vec3 reflectDir = reflect(-lightDir, norm);\n";
            shader << "    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);\n";
            shader << "    vec3 specular = spec * uLight.color;\n";

            shader << "    color *= (diffuse + specular);\n";
        }

        shader << "    FragColor = vec4(color, 1.0);\n";
        shader << "}\n";

        return shader.str();
    }

    std::string bwxGLShaderGenerator::GenerateSkyboxVertexShader() 
    {
        return R"(
                #version 450 core
                layout(location = 0) in vec3 aPosition;
                out vec3 TexCoords;
                uniform mat4 uView;
                uniform mat4 uProjection;
                void main() {
                    TexCoords = aPosition;
                    vec4 pos = uProjection * uView * vec4(aPosition, 1.0);
                    gl_Position = pos.xyww;
                }
            )";
    }

    std::string bwxGLShaderGenerator::GenerateSkyboxFragmentShader() 
    {
        return R"(
                #version 450 core
                in vec3 TexCoords;
                out vec4 FragColor;
                uniform samplerCube uSkybox;
                void main() {
                    FragColor = texture(uSkybox, TexCoords);
                }
            )";
    }

} // namespace bwx_sdk