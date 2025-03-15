/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_skybox.cpp
// Purpose:     BWXSDK; OpenGL, SkyBox
// Author:      Bartosz Warzocha
// Created:     2020-04-10
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#include "bwx_gl_skybox.h"

#include <vector>

namespace bwx_sdk {

	bwxGLSkyBox::bwxGLSkyBox()
	{
		Init(999.0f);
	}

	bwxGLSkyBox::bwxGLSkyBox(int size, std::vector<std::string> faces)
	{
		Init(size);
		LoadCube(faces);
	}

	bwxGLSkyBox::bwxGLSkyBox(int size,
		const std::string& posX,
		const std::string& negX,
		const std::string& posY,
		const std::string& negY,
		const std::string& posZ,
		const std::string& negZ)
	{
		std::vector<std::string> faces;
		faces.push_back(posX.c_str());
		faces.push_back(negX.c_str());
		faces.push_back(posY.c_str());
		faces.push_back(negY.c_str());
		faces.push_back(posZ.c_str());
		faces.push_back(negZ.c_str());

		Init(size);
		LoadCube(faces);
	}

	bwxGLSkyBox::~bwxGLSkyBox()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

		//wxDELETE(shader);
	}

	void bwxGLSkyBox::Init(GLfloat size)
	{
		GLfloat vertices[] = {
			-(size),  (size), -(size), 1.0f,
			-(size), -(size), -(size), 1.0f,
			(size), -(size), -(size), 1.0f,
			(size), -(size), -(size), 1.0f,
			(size),  (size), -(size), 1.0f,
			-(size),  (size), -(size), 1.0f,

			-(size), -(size),  (size), 1.0f,
			-(size), -(size), -(size), 1.0f,
			-(size),  (size), -(size), 1.0f,
			-(size),  (size), -(size), 1.0f,
			-(size),  (size),  (size), 1.0f,
			-(size), -(size),  (size), 1.0f,

			(size), -(size), -(size), 1.0f,
			(size), -(size),  (size), 1.0f,
			(size),  (size),  (size), 1.0f,
			(size),  (size),  (size), 1.0f,
			(size),  (size), -(size), 1.0f,
			(size), -(size), -(size), 1.0f,

			-(size), -(size),  (size), 1.0f,
			-(size),  (size),  (size), 1.0f,
			(size),  (size),  (size), 1.0f,
			(size),  (size),  (size), 1.0f,
			(size), -(size),  (size), 1.0f,
			-(size), -(size),  (size), 1.0f,

			-(size),  (size), -(size), 1.0f,
			(size),  (size), -(size), 1.0f,
			(size),  (size),  (size), 1.0f,
			(size),  (size),  (size), 1.0f,
			-(size),  (size),  (size), 1.0f,
			-(size),  (size), -(size), 1.0f,

			-(size), -(size), -(size), 1.0f,
			-(size), -(size),  (size), 1.0f,
			(size), -(size), -(size), 1.0f,
			(size), -(size), -(size), 1.0f,
			-(size), -(size),  (size), 1.0f,
			(size), -(size),  (size), 1.0f,
		};

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

		glBindVertexArray(0);
	}

	const char* bwxGLSkyBox::DefaultSkyBoxVertexShader()
	{
		return "#version 450 core\n" \
			"layout (location = 0) in vec3 position;\n" \
			"out vec3 TexCoords;\n\n" \
			"uniform mat4 PV;\n" \
			"void main()\n" \
			"{\n" \
			"   vec4 pos = PV * vec4(position, 1.0);\n" \
			"	gl_Position = pos.xyww;\n" \
			"	TexCoords = position;\n" \
			"}\n";
	}

	const char* bwxGLSkyBox::DefaultSkyBoxFragmentShader()
	{
		return "#version 450 core\n" \
			"in vec3 TexCoords;\n" \
			"out vec4 color;\n\n" \
			"uniform samplerCube skybox;\n\n" \
			"void main()\n" \
			"{\n" \
			"    color = texture(skybox, TexCoords);\n" \
			"}\n";
	}

	void bwxGLSkyBox::LoadCube(std::vector<std::string> faces)
	{
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		for (GLuint i = 0; i < faces.size(); i++)
		{
			//bwxGLImgLoader img;
			//img.Load(faces[i]);

			//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, img.Width(), img.Height(), 0, GL_RGB, GL_UNSIGNED_BYTE, img.Data());
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	void bwxGLSkyBox::Render(glm::mat4* PV)
	{
		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);

		//shader->Bind();

		//shader->SetMat4("PV", 1, GL_FALSE, PV[0][0]);

		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		//shader->Unbind();

		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);
	}

	void bwxGLSkyBox::UseDefaultSkyBoxShader()
	{
		//shader = new bwxGLShader(bwxGLSkyBox::DefaultSkyBoxVertexShader(), 0, 0, 0, bwxGLSkyBox::DefaultSkyBoxFragmentShader());
		//shader->Bind();
		//shader->AddUniform("view");
		//shader->AddUniform("projection");
		//shader->Set1i("skybox", 0);
		//shader->Unbind();
	}

}
