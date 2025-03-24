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
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace bwx_sdk {

    std::unordered_map<std::string, std::string> bwx_sdk::bwxGLShaderGenerator::m_shaderCache;

    std::string bwxGLShaderGenerator::GetVertexShader(bool useNormals, bool useTexCoords, bool useLighting) 
    {
        std::string key = "V_" + std::to_string(useNormals) + "_" + std::to_string(useTexCoords) + "_" + std::to_string(useLighting);

        auto it = m_shaderCache.find(key);
        if (it != m_shaderCache.end()) {
            return it->second;
        }

        std::string shader = GenerateVertexShader(useNormals, useTexCoords, useLighting);
        m_shaderCache[key] = shader;
        return shader;
    }

    std::string bwxGLShaderGenerator::GetFragmentShader(bool useTextures, bool useLighting) 
    {
        std::string key = "F_" + std::to_string(useTextures) + "_" + std::to_string(useLighting);

        auto it = m_shaderCache.find(key);
        if (it != m_shaderCache.end()) {
            return it->second;
        }

        std::string shader = GenerateFragmentShader(useTextures, useLighting);
        m_shaderCache[key] = shader;
        return shader;
    }

    std::string bwxGLShaderGenerator::GetDefaultSkyboxVertexShader() 
    {
        return R"GLSL(
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
            )GLSL";
    }

    std::string bwxGLShaderGenerator::GetDefaultSkyboxFragmentShader() 
    {
        return R"GLSL(
                #version 450 core
                in vec3 TexCoords;
                out vec4 FragColor;
                uniform samplerCube uSkybox;
                void main() {
                    FragColor = texture(uSkybox, TexCoords);
                }
            )GLSL";
    }

    std::string bwxGLShaderGenerator::GetDefaultTTFVertexShader()
    {
        return R"GLSL(
                #version 450 core
                layout (location = 0) in vec4 vertex;
                out vec2 TexCoords;
                uniform mat4 projection;
                void main() {
                    gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);
            	    TexCoords = vertex.zw;
                };
            )GLSL";
    }

    std::string bwxGLShaderGenerator::GetDefaultTTFFragmentShader()
    {
        return R"GLSL(
            #version 450 core
            in vec2 TexCoords;
            out vec4 color;
            uniform sampler2D text;
            uniform vec4 textColor;
            void main() {
                vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
                color = textColor * sampled;
            };
        )GLSL";
    }

    /*
        std::string src;
        src += GetLightStructBlock();
        src += "\n";
        src += GetLightCalculationFunction();
        src += "\n";
    */

    std::string bwxGLShaderGenerator::GetLightStructBlock() {
        return R"GLSL(
		#define MAX_LIGHTS 64

		struct Light {
			vec4 position;     // xyz: position, w: type
			vec4 direction;    // xyz: direction, w: inner cone
			vec4 diffuse;      // rgb: color, a: power
			vec4 ambient;      // rgb: ambient, a: range
			vec4 specular;     // rgb: specular, a: outer cone
			vec4 attenuation;  // x: constant, y: linear, z: quadratic, w: unused
		};

		layout(std140, binding = 2) uniform LightBlock {
			Light lights[MAX_LIGHTS];
		};
	)GLSL";
    }

    std::string bwxGLShaderGenerator::GetLightCalculationFunction() {
        return R"GLSL(
		vec3 CalculateLighting(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
		{
			vec3 lightDir = normalize(light.position.xyz - fragPos);
			float diff = max(dot(normal, lightDir), 0.0);

			// Attenuation
			float distance = length(light.position.xyz - fragPos);
			float attenuation = 1.0 / (
				light.attenuation.x +
				light.attenuation.y * distance +
				light.attenuation.z * distance * distance);

			vec3 diffuse = light.diffuse.rgb * diff;
			vec3 ambient = light.ambient.rgb;
			vec3 specular = vec3(0.0); // TODO: specular logic here

			return (ambient + diffuse + specular) * attenuation * light.diffuse.a;
		}
	)GLSL";
    }

    std::string bwxGLShaderGenerator::GenerateVertexShader(bool useNormals, bool useTexCoords, bool useLighting) {
        std::string shader;
        shader += "#version 330 core\n\n";
        shader += "layout(location = 0) in vec3 aPos;\n";
        if (useNormals) shader += "layout(location = 1) in vec3 aNormal;\n";
        if (useTexCoords) shader += "layout(location = 2) in vec2 aTexCoords;\n";

        shader += "uniform mat4 model;\n";
        shader += "uniform mat4 view;\n";
        shader += "uniform mat4 projection;\n\n";

        shader += "out vec3 FragPos;\n";
        if (useNormals) shader += "out vec3 Normal;\n";
        if (useTexCoords) shader += "out vec2 TexCoords;\n";

        shader += "void main() {\n";
        shader += "\tFragPos = vec3(model * vec4(aPos, 1.0));\n";
        if (useNormals)
            shader += "\tNormal = mat3(transpose(inverse(model))) * aNormal;\n";
        if (useTexCoords)
            shader += "\tTexCoords = aTexCoords;\n";
        shader += "\tgl_Position = projection * view * vec4(FragPos, 1.0);\n";
        shader += "}\n";

        return shader;
    }

    std::string bwxGLShaderGenerator::GenerateFragmentShader(bool useTextures, bool useLighting) {
        std::string shader;
        shader += "#version 330 core\n\n";
        shader += "in vec3 FragPos;\n";
        shader += "in vec3 Normal;\n";
        shader += "in vec2 TexCoords;\n";
        shader += "out vec4 FragColor;\n";
        shader += "uniform vec3 viewPos;\n";

        if (useTextures) {
            shader += "uniform sampler2D diffuseMap;\n";
        }
        if (useLighting) {
            shader += GetLightStructBlock();
            shader += GetLightCalculationFunction();
        }

        shader += "\nvoid main() {\n";
        shader += "\tvec3 norm = normalize(Normal);\n";
        shader += "\tvec3 viewDir = normalize(viewPos - FragPos);\n";
        shader += "\tvec3 result = vec3(0.0);\n";

        if (useLighting) {
            shader += "\tfor (int i = 0; i < MAX_LIGHTS; ++i) {\n";
            shader += "\t\tif (lights[i].diffuse.a == 0.0) break;\n";
            shader += "\t\tresult += CalculateLighting(lights[i], norm, FragPos, viewDir);\n";
            shader += "\t}\n";
        }
        else {
            shader += "\tresult = vec3(1.0);\n";
        }

        if (useTextures) {
            shader += "\tvec4 texColor = texture(diffuseMap, TexCoords);\n";
            shader += "\tFragColor = vec4(result, 1.0) * texColor;\n";
        }
        else {
            shader += "\tFragColor = vec4(result, 1.0);\n";
        }
        shader += "}\n";

        return shader;
    }

} // namespace bwx_sdk