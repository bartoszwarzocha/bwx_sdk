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

	//-----------------------------------------------------------------
	// bwxGLFPSMonitor
	//-----------------------------------------------------------------
	bwxGLFPSMonitor::bwxGLFPSMonitor()
		: m_lastUpdate(std::chrono::steady_clock::now()),
		m_lastFrame(std::chrono::steady_clock::now()),
		m_frames(0),
		m_fps(0.0f)
	{}

	GLfloat bwxGLFPSMonitor::GetDelta()
	{
		using namespace std::chrono;
		auto now = steady_clock::now();
		duration<GLfloat> delta = now - m_lastFrame;
		m_lastFrame = now;
		return delta.count();
	}

	GLfloat bwxGLFPSMonitor::GetElapsedTime()
	{
		using namespace std::chrono;
		duration<GLfloat> elapsed = steady_clock::now() - m_lastUpdate;
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