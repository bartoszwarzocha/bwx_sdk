/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_utils.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Utils
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <thread>
#include <sstream>
#include <iomanip>
#include <iostream>

#include "bwx_gl_utils.h"

namespace bwx_sdk {

	glm::vec2 bwxGLUtils::GetWindowCoordinates(wxGLCanvas* canvas, glm::vec2 pos) {
		return glm::vec2(pos.x, canvas->GetSize().y - pos.y);
	}

	std::string bwxGLUtils::GetVersion()
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_VERSION)));
	}

	std::string bwxGLUtils::GetVendor()
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
	}

	std::string bwxGLUtils::GetRenderer()
	{
		return std::string(reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}

	wxGLContextAttrs bwxGLUtils::GetDefaultContextAttrs(int major, int minor)
	{
		wxGLContextAttrs ret;
		ret.PlatformDefaults().CoreProfile().OGLVersion(major, minor).EndList();
		return ret;
	}

	wxGLAttributes bwxGLUtils::GetDefaultCanvasAttrs(int depth)
	{
		wxGLAttributes ret;
		ret.PlatformDefaults().RGBA().DoubleBuffer().Depth(depth).EndList();
		return ret;
	}

	std::string bwxGLUtils::GetErrorString(int err) {
		switch (err) {
		case GL_NO_ERROR: return "No error";
		case GL_INVALID_ENUM: return "Invalid enum";
		case GL_INVALID_VALUE: return "Invalid value";
		case GL_INVALID_OPERATION: return "Invalid operation";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "Invalid framebuffer operation";
		case GL_OUT_OF_MEMORY: return "Out of memory";
		default: return "Unknown error";
		}
	}

	void bwxGLUtils::SetDefaultClearColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//static const GLfloat color[] = {r, g, b, a};
		//glClearBufferfv(GL_COLOR, 0, color);
	}

	std::vector<float> bwxGLUtils::GenerateSimpleCubeVertices(bool textured)
	{
		std::vector<float> vertices;

		float baseVerts[] = {
			// positions         // texCoords
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		for (size_t i = 0; i < sizeof(baseVerts) / sizeof(float); i += 5) {
			// Always include position
			vertices.push_back(baseVerts[i]);
			vertices.push_back(baseVerts[i + 1]);
			vertices.push_back(baseVerts[i + 2]);

			// Optionally include texCoords
			if (textured) {
				vertices.push_back(baseVerts[i + 3]);
				vertices.push_back(baseVerts[i + 4]);
			}
		}

		return vertices;
	}

	//-----------------------------------------------------------------
	// bwxGLFPSMonitor
	//-----------------------------------------------------------------
	bwxGLFPSMonitor::bwxGLFPSMonitor()
		: m_lastUpdate(std::chrono::steady_clock::now()),
		m_startTime(m_lastUpdate),
		m_frameStart(m_lastUpdate),
		m_lastDelta(0.0f),
		m_frames(0),
		m_fps(0.0f)
	{
	}

	void bwxGLFPSMonitor::StartFrame()
	{
		using namespace std::chrono;
		auto now = steady_clock::now();
		std::chrono::duration<GLfloat> delta = now - m_frameStart;
		m_lastDelta = delta.count();
		m_frameStart = now;
	}

	void bwxGLFPSMonitor::LimitFPS(int targetFPS)
	{
		if (targetFPS <= 0)
			return;

		using namespace std::chrono;

		const GLfloat targetFrameTime = 1.0f / static_cast<GLfloat>(targetFPS);
		auto now = steady_clock::now();
		std::chrono::duration<GLfloat> frameTime = now - m_frameStart;

		GLfloat sleepTime = targetFrameTime - frameTime.count();

		if (sleepTime > 0.0005f)
		{
			auto sleepMs = static_cast<unsigned long>(sleepTime * 1000.0f);
			if (sleepMs > 0)
				wxMilliSleep(sleepMs);

			while (std::chrono::steady_clock::now() - m_frameStart <= duration<GLfloat>(targetFrameTime)) {}
		}
	}

	GLfloat bwxGLFPSMonitor::GetDelta() const
	{
		return m_lastDelta;
	}

	GLfloat bwxGLFPSMonitor::GetElapsedTime() const
	{
		using namespace std::chrono;
		std::chrono::duration<GLfloat> elapsed = steady_clock::now() - m_startTime;
		return elapsed.count();
	}

	GLfloat bwxGLFPSMonitor::GetFPS(int refresh_ms)
	{
		using namespace std::chrono;

		++m_frames;
		auto now = steady_clock::now();
		auto elapsed = duration_cast<milliseconds>(now - m_lastUpdate).count();

		if (elapsed >= refresh_ms)
		{
			m_fps = m_frames * 1000.0f / elapsed;
			m_frames = 0;
			m_lastUpdate = now;
		}

		return m_fps;
	}

	std::string bwxGLFPSMonitor::GetFPSStr(int refresh_ms)
	{
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(2) << "FPS: " << GetFPS(refresh_ms);
		return oss.str();
	}

} // namespace bwx_sdk